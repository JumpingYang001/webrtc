/*
 *  Copyright 2016 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef API_TEST_MOCK_RTPRECEIVER_H_
#define API_TEST_MOCK_RTPRECEIVER_H_

#include <string>
#include <vector>

#include "api/rtp_receiver_interface.h"
#include "test/gmock.h"

namespace webrtc {

class MockRtpReceiver : public RtpReceiverInterface {
 public:
  static rtc::scoped_refptr<MockRtpReceiver> Create() {
    return rtc::make_ref_counted<MockRtpReceiver>();
  }

  MOCK_METHOD(rtc::scoped_refptr<MediaStreamTrackInterface>,
              track,
              (),
              (const, override));
  MOCK_METHOD(std::vector<rtc::scoped_refptr<MediaStreamInterface>>,
              streams,
              (),
              (const, override));
  MOCK_METHOD(cricket::MediaType, media_type, (), (const, override));
  MOCK_METHOD(std::string, id, (), (const, override));
  MOCK_METHOD(RtpParameters, GetParameters, (), (const, override));
  MOCK_METHOD(void, SetObserver, (RtpReceiverObserverInterface*), (override));
  MOCK_METHOD(void,
              SetJitterBufferMinimumDelay,
              (absl::optional<double>),
              (override));
  MOCK_METHOD(std::vector<RtpSource>, GetSources, (), (const, override));

 protected:
  MockRtpReceiver() = default;
};

static_assert(!std::is_abstract_v<rtc::RefCountedObject<MockRtpReceiver>>);

}  // namespace webrtc

#endif  // API_TEST_MOCK_RTPRECEIVER_H_
