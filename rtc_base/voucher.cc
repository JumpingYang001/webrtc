/*
 *  Copyright 2023 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "rtc_base/voucher.h"

#include <memory>
#include <utility>

#include "api/make_ref_counted.h"

namespace webrtc {
namespace {

rtc::FinalRefCountedObject<Voucher>*& CurrentVoucherStorage() {
  static thread_local rtc::FinalRefCountedObject<Voucher>* storage = nullptr;
  return storage;
}

}  // namespace

Voucher::ScopedSetter::ScopedSetter(Ptr voucher)
    : old_current_(Voucher::Current()) {
  Voucher::SetCurrent(std::move(voucher));
}

Voucher::ScopedSetter::~ScopedSetter() {
  Voucher::SetCurrent(std::move(old_current_));
}

Voucher::Attachment::Id Voucher::Attachment::GetNextId() {
  static std::atomic<Voucher::Attachment::Id> current_id = 0;
  auto id = current_id.fetch_add(1);
  RTC_CHECK(id < Voucher::kAttachmentCapacity);
  return id;
}

Voucher::Ptr Voucher::CurrentOrCreateForCurrentTask() {
  auto& storage = CurrentVoucherStorage();
  Voucher::Ptr result(storage);
  if (!result) {
    result = rtc::make_ref_counted<Voucher>();
    storage = result.get();
    storage->AddRef();
  }
  return result;
}

Voucher::Ptr Voucher::Current() {
  auto& storage = CurrentVoucherStorage();
  Voucher::Ptr result(storage);
  return result;
}

Voucher::Voucher() : attachments_(Voucher::kAttachmentCapacity) {}

void Voucher::SetCurrent(Voucher::Ptr value) {
  auto& storage = CurrentVoucherStorage();
  if (value.get() != storage) {
    if (storage) {
      storage->Release();
    }
    storage = value.release();
  }
}

void Voucher::SetAttachment(Attachment::Id id,
                            std::unique_ptr<Attachment> attachment) {
  RTC_CHECK(id < kAttachmentCapacity);
  MutexLock lock(&mu_);
  attachments_[id] = std::move(attachment);
}

}  // namespace webrtc
