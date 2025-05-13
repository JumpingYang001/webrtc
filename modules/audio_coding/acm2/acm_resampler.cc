/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "modules/audio_coding/acm2/acm_resampler.h"

#include <string.h>

#include <cstdint>

#include "api/audio/audio_frame.h"
#include "api/audio/audio_view.h"
#include "rtc_base/checks.h"
#include "rtc_base/logging.h"
#include "rtc_base/numerics/safe_conversions.h"

namespace webrtc {
namespace acm2 {

ACMResampler::ACMResampler() {}

ACMResampler::~ACMResampler() {}

int ACMResampler::Resample10Msec(const int16_t* in_audio,
                                 int in_freq_hz,
                                 int out_freq_hz,
                                 size_t num_audio_channels,
                                 size_t out_capacity_samples,
                                 int16_t* out_audio) {
  InterleavedView<const int16_t> src(
      in_audio, SampleRateToDefaultChannelSize(in_freq_hz), num_audio_channels);
  InterleavedView<int16_t> dst(out_audio,
                               SampleRateToDefaultChannelSize(out_freq_hz),
                               num_audio_channels);
  RTC_DCHECK_GE(out_capacity_samples, dst.size());
  Resample10Msec(src, in_freq_hz, dst, out_freq_hz);
  return dst.samples_per_channel();
}

void ACMResampler::Resample10Msec(InterleavedView<const int16_t> src,
                                  int in_freq_hz,
                                  InterleavedView<int16_t> dst,
                                  int out_freq_hz) {
  RTC_DCHECK_EQ(src.num_channels(), dst.num_channels());
  if (in_freq_hz == out_freq_hz) {
    CopySamples(dst, src);
  } else {
    resampler_.Resample(src, dst);
  }
}

ResamplerHelper::ResamplerHelper() {
  ClearSamples(last_audio_buffer_);
}

bool ResamplerHelper::MaybeResample(int desired_sample_rate_hz,
                                    AudioFrame* audio_frame) {
  const int current_sample_rate_hz = audio_frame->sample_rate_hz_;
  RTC_DCHECK_NE(current_sample_rate_hz, 0);

  // Update if resampling is required.
  const bool need_resampling =
      (desired_sample_rate_hz != -1) &&
      (current_sample_rate_hz != desired_sample_rate_hz);

  if (need_resampling && !resampled_last_output_frame_) {
    // Prime the resampler with the last frame.
    int16_t temp_output[AudioFrame::kMaxDataSizeSamples];
    int samples_per_channel_int = resampler_.Resample10Msec(
        last_audio_buffer_.data(), current_sample_rate_hz,
        desired_sample_rate_hz, audio_frame->num_channels_,
        AudioFrame::kMaxDataSizeSamples, temp_output);
    if (samples_per_channel_int < 0) {
      RTC_LOG(LS_ERROR) << "AcmReceiver::GetAudio - "
                           "Resampling last_audio_buffer_ failed.";
      return false;
    }
  }

  // TODO(bugs.webrtc.org/3923) Glitches in the output may appear if the output
  // rate from NetEq changes.
  if (need_resampling) {
    // TODO(yujo): handle this more efficiently for muted frames.
    int samples_per_channel_int = resampler_.Resample10Msec(
        audio_frame->data(), current_sample_rate_hz, desired_sample_rate_hz,
        audio_frame->num_channels_, AudioFrame::kMaxDataSizeSamples,
        audio_frame->mutable_data());
    if (samples_per_channel_int < 0) {
      RTC_LOG(LS_ERROR)
          << "AcmReceiver::GetAudio - Resampling audio_buffer_ failed.";
      return false;
    }
    audio_frame->samples_per_channel_ =
        static_cast<size_t>(samples_per_channel_int);
    audio_frame->sample_rate_hz_ = desired_sample_rate_hz;
    RTC_DCHECK_EQ(audio_frame->sample_rate_hz_,
                  dchecked_cast<int>(audio_frame->samples_per_channel_ * 100));
    resampled_last_output_frame_ = true;
  } else {
    resampled_last_output_frame_ = false;
    // We might end up here ONLY if codec is changed.
  }

  // Store current audio in `last_audio_buffer_` for next time.
  // TODO: b/335805780 - Use CopySamples().
  memcpy(last_audio_buffer_.data(), audio_frame->data(),
         sizeof(int16_t) * audio_frame->samples_per_channel_ *
             audio_frame->num_channels_);

  return true;
}

}  // namespace acm2
}  // namespace webrtc
