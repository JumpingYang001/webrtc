/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef API_CALL_TRANSPORT_H_
#define API_CALL_TRANSPORT_H_

#include <stdint.h>

#include "api/array_view.h"

namespace webrtc {

// TODO(holmer): Look into unifying this with the PacketOptions in
// asyncpacketsocket.h.
struct PacketOptions {
  PacketOptions();
  PacketOptions(const PacketOptions&);
  ~PacketOptions();

  // Negative ids are invalid and should be interpreted
  // as packet_id not being set.
  int64_t packet_id = -1;
  // Whether this is an audio or video packet, excluding retransmissions.
  // Defaults to `false` which is the more common case.
  bool is_media = false;
  bool included_in_feedback = false;
  bool included_in_allocation = false;
  bool send_as_ect1 = false;
  // Whether this packet can be part of a packet batch at lower levels.
  bool batchable = false;
  // Whether this packet is the last of a batch.
  bool last_packet_in_batch = false;
};

class Transport {
 public:
  virtual bool SendRtp(ArrayView<const uint8_t> packet,
                       const PacketOptions& options) = 0;
  // TODO: bugs.webrtc.org/42225697 - Make virtual when downstream is updated.
  virtual bool SendRtcp(ArrayView<const uint8_t> packet,
                        const PacketOptions& /*options*/) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    return SendRtcp(packet);
#pragma clang diagnostic pop
  }
  [[deprecated("Pass PacketOptions")]]
  // TODO: bugs.webrtc.org/42225697 - Remove when downstream is updated.
  virtual bool SendRtcp(ArrayView<const uint8_t> packet) {
    return SendRtcp(packet, /*packet_options=*/{});
  }

 protected:
  virtual ~Transport() {}
};

}  // namespace webrtc

#endif  // API_CALL_TRANSPORT_H_
