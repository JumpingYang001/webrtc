/*
 *  Copyright 2023 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef RTC_BASE_VOUCHER_H_
#define RTC_BASE_VOUCHER_H_

#include <memory>

#include "absl/container/inlined_vector.h"
#include "rtc_base/ref_counted_object.h"
#include "rtc_base/synchronization/mutex.h"
#include "rtc_base/system/rtc_export.h"

namespace webrtc {

// A voucher is associated with a currently running task tree. Whenever tasks
// are posted, the current voucher is inherited and set as current in the new
// task. The voucher exists for as long as there are direct and indirect
// tasks running that descend from the task where the voucher was created.
class RTC_EXPORT Voucher {
 public:
  static constexpr size_t kAttachmentCapacity = 4;

  using Ptr = rtc::scoped_refptr<rtc::FinalRefCountedObject<Voucher>>;

  // Vouchers aggregate attachments, which are application-specific attachments
  // that have logic unrelated to the mechanics of Voucher progression.
  class Attachment {
   public:
    using Id = size_t;

    // Attachments should call this function one to get an ID to use with
    // SetAttachment.
    static Attachment::Id GetNextId();

    virtual ~Attachment() = default;
  };

  // Scoped setter that saves the current voucher on stack and instates a new
  // one, until the scope exits.
  class ScopedSetter {
   public:
    explicit ScopedSetter(Ptr voucher);
    ~ScopedSetter();

   private:
    Ptr old_current_;
  };

  static Ptr Current();
  static Ptr CurrentOrCreateForCurrentTask();

  // For Attachments: stores an attachment into a voucher. If one is already
  // present, it gets replaced.
  void SetAttachment(Attachment::Id id, std::unique_ptr<Attachment> attachment);

 private:
  friend class rtc::FinalRefCountedObject<webrtc::Voucher>;
  Voucher();

  friend class ScopedSetter;
  static void SetCurrent(Ptr ptr);

  Mutex mu_;
  absl::InlinedVector<std::unique_ptr<Attachment>, kAttachmentCapacity>
      attachments_ RTC_GUARDED_BY(&mu_);
};

}  // namespace webrtc

#endif  // RTC_BASE_VOUCHER_H_
