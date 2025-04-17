/*
 *  Copyright 2024 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "p2p/dtls/dtls_utils.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "absl/container/flat_hash_set.h"
#include "api/array_view.h"
#include "rtc_base/buffer.h"
#include "rtc_base/byte_buffer.h"
#include "rtc_base/checks.h"
#include "rtc_base/crc32.h"

namespace {
// https://datatracker.ietf.org/doc/html/rfc5246#appendix-A.1
const uint8_t kDtlsChangeCipherSpecRecord = 20;
const uint8_t kDtlsHandshakeRecord = 22;

// https://www.rfc-editor.org/rfc/rfc9147.html#section-4
const uint8_t kFixedBitmask = 0b00100000;
const uint8_t kConnectionBitmask = 0b00010000;
const uint8_t kSequenceNumberBitmask = 0b00001000;
const uint8_t kLengthPresentBitmask = 0b00000100;
}  // namespace

namespace webrtc {

bool IsDtlsPacket(ArrayView<const uint8_t> payload) {
  const uint8_t* u = payload.data();
  return (payload.size() >= kDtlsRecordHeaderLen && (u[0] > 19 && u[0] < 64));
}

bool IsDtlsClientHelloPacket(ArrayView<const uint8_t> payload) {
  if (!IsDtlsPacket(payload)) {
    return false;
  }
  const uint8_t* u = payload.data();
  return payload.size() > 17 && u[0] == kDtlsHandshakeRecord && u[13] == 1;
}

bool IsDtlsHandshakePacket(ArrayView<const uint8_t> payload) {
  if (!IsDtlsPacket(payload)) {
    return false;
  }
  // change cipher spec is not a handshake packet. This used
  // to work because it was aggregated with the session ticket
  // which is no more. It is followed by the encrypted handshake
  // message which starts with a handshake record (22) again.
  return payload.size() > 17 && (payload[0] == kDtlsHandshakeRecord ||
                                 payload[0] == kDtlsChangeCipherSpecRecord);
}

// Returns a (unsorted) list of (msg_seq) received as part of the handshake.
std::optional<std::vector<uint16_t>> GetDtlsHandshakeAcks(
    ArrayView<const uint8_t> dtls_packet) {
  std::vector<uint16_t> acks;
  ByteBufferReader record_buf(dtls_packet);
  // https://datatracker.ietf.org/doc/html/rfc6347#section-4.1
  while (record_buf.Length() >= kDtlsRecordHeaderLen) {
    uint8_t content_type;
    uint64_t epoch_and_seq;
    uint16_t len;
    // Read content_type(1).
    if (!record_buf.ReadUInt8(&content_type)) {
      return std::nullopt;
    }

    // DTLS 1.3 rules:
    // https://www.rfc-editor.org/rfc/rfc9147.html#section-4.1
    if ((content_type & kFixedBitmask) == kFixedBitmask) {
      // Interpret as DTLSCipherText:
      // https://www.rfc-editor.org/rfc/rfc9147.html#appendix-A.1
      // We assume no connection id is used so C must be 0.
      if ((content_type & kConnectionBitmask) != 0) {
        return std::nullopt;
      }
      // Skip sequence_number(1 or 2 bytes depending on S bit).
      if (!record_buf.Consume((content_type & kSequenceNumberBitmask) ==
                                      kSequenceNumberBitmask
                                  ? 2
                                  : 1)) {
        return std::nullopt;
      }
      // If the L bit is set, consume the 16 bit length field.
      if ((content_type & kLengthPresentBitmask) == kLengthPresentBitmask) {
        if (!(record_buf.ReadUInt16(&len) && record_buf.Consume(len))) {
          return std::nullopt;
        }
      }
      // DTLSCipherText is encrypted so we can not read it.
      continue;
    }
    // Skip version(2), read epoch+seq(2+6), read len(2)
    if (!(record_buf.Consume(2) && record_buf.ReadUInt64(&epoch_and_seq) &&
          record_buf.ReadUInt16(&len) && record_buf.Length() >= len)) {
      return std::nullopt;
    }
    if (content_type != kDtlsHandshakeRecord) {
      record_buf.Consume(len);
      continue;
    }
    // Epoch 1+ is encrypted so we can not parse it.
    if (epoch_and_seq >> 6 != 0) {
      record_buf.Consume(len);
      continue;
    }

    // https://www.rfc-editor.org/rfc/rfc6347.html#section-4.2.2
    ByteBufferReader handshake_buf(record_buf.DataView().subview(0, len));
    while (handshake_buf.Length() > 0) {
      uint16_t msg_seq;
      uint32_t fragment_len;
      uint32_t fragment_offset;
      // Skip msg_type(1) and length(3), read msg_seq(2), skip
      // fragment_offset(3), read fragment_length(3) and consume it.
      if (!(handshake_buf.Consume(1 + 3) &&
            handshake_buf.ReadUInt16(&msg_seq) &&
            handshake_buf.ReadUInt24(&fragment_offset) &&
            handshake_buf.ReadUInt24(&fragment_len) &&
            handshake_buf.Consume(fragment_len))) {
        return std::nullopt;
      }
      acks.push_back(msg_seq);
      // Advance outer buffer.
      record_buf.Consume(12 + fragment_len);
    }
    RTC_DCHECK(handshake_buf.Length() == 0);
  }

  // Should have consumed everything.
  if (record_buf.Length() != 0) {
    return std::nullopt;
  }
  return acks;
}

uint32_t ComputeDtlsPacketHash(ArrayView<const uint8_t> dtls_packet) {
  return webrtc::ComputeCrc32(dtls_packet.data(), dtls_packet.size());
}

bool PacketStash::AddIfUnique(rtc::ArrayView<const uint8_t> packet) {
  uint32_t h = ComputeDtlsPacketHash(packet);
  for (const auto& [hash, p] : packets_) {
    if (h == hash) {
      return false;
    }
  }
  packets_.push_back({.hash = h,
                      .buffer = std::make_unique<webrtc::Buffer>(
                          packet.data(), packet.size())});
  return true;
}

void PacketStash::Add(rtc::ArrayView<const uint8_t> packet) {
  packets_.push_back({.hash = ComputeDtlsPacketHash(packet),
                      .buffer = std::make_unique<webrtc::Buffer>(
                          packet.data(), packet.size())});
}

void PacketStash::Prune(const absl::flat_hash_set<uint32_t>& hashes) {
  if (hashes.empty()) {
    return;
  }
  uint32_t before = packets_.size();
  packets_.erase(std::remove_if(packets_.begin(), packets_.end(),
                                [&](const auto& val) {
                                  return hashes.contains(val.hash);
                                }),
                 packets_.end());
  uint32_t after = packets_.size();
  uint32_t removed = before - after;
  if (pos_ >= removed) {
    pos_ -= removed;
  }
}

void PacketStash::Prune(uint32_t max_size) {
  auto size = packets_.size();
  if (size <= max_size) {
    return;
  }
  auto removed = size - max_size;
  packets_.erase(packets_.begin(), packets_.begin() + removed);
  if (pos_ <= removed) {
    pos_ = 0;
  } else {
    pos_ -= removed;
  }
}

rtc::ArrayView<const uint8_t> PacketStash::GetNext() {
  RTC_DCHECK(!packets_.empty());
  auto pos = pos_;
  pos_ = (pos + 1) % packets_.size();
  const auto& buffer = packets_[pos].buffer;
  return rtc::ArrayView<const uint8_t>(buffer->data(), buffer->size());
}

}  // namespace webrtc
