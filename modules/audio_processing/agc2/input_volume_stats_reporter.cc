/*
 *  Copyright (c) 2021 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "modules/audio_processing/agc2/input_volume_stats_reporter.h"

#include <cmath>

#include "rtc_base/logging.h"
#include "rtc_base/numerics/safe_minmax.h"
#include "system_wrappers/include/metrics.h"

namespace webrtc {
namespace {

using InputVolumeType = InputVolumeStatsReporter::InputVolumeType;

constexpr int kFramesIn60Seconds = 6000;
constexpr int kMinInputVolume = 0;
constexpr int kMaxInputVolume = 255;
constexpr int kMaxUpdate = kMaxInputVolume - kMinInputVolume;

float ComputeAverageUpdate(int sum_updates, int num_updates) {
  RTC_DCHECK_GE(sum_updates, 0);
  RTC_DCHECK_LE(sum_updates, kMaxUpdate * kFramesIn60Seconds);
  RTC_DCHECK_GE(num_updates, 0);
  RTC_DCHECK_LE(num_updates, kFramesIn60Seconds);
  if (num_updates == 0) {
    return 0.0f;
  }
  return std::round(static_cast<float>(sum_updates) /
                    static_cast<float>(num_updates));
}

metrics::Histogram* CreateRateHistogram(absl::string_view name) {
  return metrics::HistogramFactoryGetCounts(
      name, /*min=*/1, /*max=*/kFramesIn60Seconds, /*bucket_count=*/50);
}

metrics::Histogram* CreateAverageHistogram(absl::string_view name) {
  return metrics::HistogramFactoryGetCounts(name, /*min=*/1, /*max=*/kMaxUpdate,
                                            /*bucket_count=*/50);
}

metrics::Histogram* CreateDecreaseRateHistogram(
    InputVolumeType input_volume_type) {
  switch (input_volume_type) {
    case InputVolumeType::kApplied:
      return CreateRateHistogram(
          "WebRTC.Audio.Apm.AppliedInputVolume.DecreaseRate");
    case InputVolumeType::kRecommended:
      return CreateRateHistogram(
          "WebRTC.Audio.Apm.RecommendedInputVolume.DecreaseRate");
  }
}

metrics::Histogram* CreateDecreaseAverageHistogram(
    InputVolumeType input_volume_type) {
  switch (input_volume_type) {
    case InputVolumeType::kApplied:
      return CreateAverageHistogram(
          "WebRTC.Audio.Apm.AppliedInputVolume.DecreaseAverage");
    case InputVolumeType::kRecommended:
      return CreateAverageHistogram(
          "WebRTC.Audio.Apm.RecommendedInputVolume.DecreaseAverage");
  }
}

metrics::Histogram* CreateIncreaseRateHistogram(
    InputVolumeType input_volume_type) {
  switch (input_volume_type) {
    case InputVolumeType::kApplied:
      return CreateRateHistogram(
          "WebRTC.Audio.Apm.AppliedInputVolume.IncreaseRate");
    case InputVolumeType::kRecommended:
      return CreateRateHistogram(
          "WebRTC.Audio.Apm.RecommendedInputVolume.IncreaseRate");
  }
}

metrics::Histogram* CreateIncreaseAverageHistogram(
    InputVolumeType input_volume_type) {
  switch (input_volume_type) {
    case InputVolumeType::kApplied:
      return CreateAverageHistogram(
          "WebRTC.Audio.Apm.AppliedInputVolume.IncreaseAverage");
    case InputVolumeType::kRecommended:
      return CreateAverageHistogram(
          "WebRTC.Audio.Apm.RecommendedInputVolume.IncreaseAverage");
  }
}

metrics::Histogram* CreateUpdateRateHistogram(
    InputVolumeType input_volume_type) {
  switch (input_volume_type) {
    case InputVolumeType::kApplied:
      return CreateRateHistogram(
          "WebRTC.Audio.Apm.AppliedInputVolume.UpdateRate");
    case InputVolumeType::kRecommended:
      return CreateRateHistogram(
          "WebRTC.Audio.Apm.RecommendedInputVolume.UpdateRate");
  }
}

metrics::Histogram* CreateUpdateAverageHistogram(
    InputVolumeType input_volume_type) {
  switch (input_volume_type) {
    case InputVolumeType::kApplied:
      return CreateAverageHistogram(
          "WebRTC.Audio.Apm.AppliedInputVolume.UpdateAverage");
    case InputVolumeType::kRecommended:
      return CreateAverageHistogram(
          "WebRTC.Audio.Apm.RecommendedInputVolume.UpdateAverage");
  }
}

}  // namespace

InputVolumeStatsReporter::InputVolumeStatsReporter(
    InputVolumeType input_volume_type)
    : decrease_rate_histogram_(CreateDecreaseRateHistogram(input_volume_type)),
      decrease_average_histogram_(
          CreateDecreaseAverageHistogram(input_volume_type)),
      increase_rate_histogram_(CreateIncreaseRateHistogram(input_volume_type)),
      increase_average_histogram_(
          CreateIncreaseAverageHistogram(input_volume_type)),
      update_rate_histogram_(CreateUpdateRateHistogram(input_volume_type)),
      update_average_histogram_(
          CreateUpdateAverageHistogram(input_volume_type)) {}

InputVolumeStatsReporter::~InputVolumeStatsReporter() = default;

void InputVolumeStatsReporter::UpdateStatistics(int input_volume) {
  RTC_DCHECK_GE(input_volume, kMinInputVolume);
  RTC_DCHECK_LE(input_volume, kMaxInputVolume);
  if (previous_input_volume_.has_value() &&
      input_volume != previous_input_volume_.value()) {
    const int volume_change = input_volume - previous_input_volume_.value();
    if (volume_change < 0) {
      ++volume_update_stats_.num_decreases;
      volume_update_stats_.sum_decreases -= volume_change;
    } else {
      ++volume_update_stats_.num_increases;
      volume_update_stats_.sum_increases += volume_change;
    }
  }
  // Periodically log input volume change metrics.
  if (++log_volume_update_stats_counter_ >= kFramesIn60Seconds) {
    LogVolumeUpdateStats();
    volume_update_stats_ = {};
    log_volume_update_stats_counter_ = 0;
  }
  previous_input_volume_ = input_volume;
}

void InputVolumeStatsReporter::LogVolumeUpdateStats() const {
  const float average_decrease = ComputeAverageUpdate(
      volume_update_stats_.sum_decreases, volume_update_stats_.num_decreases);
  const float average_increase = ComputeAverageUpdate(
      volume_update_stats_.sum_increases, volume_update_stats_.num_increases);
  const int num_updates =
      volume_update_stats_.num_decreases + volume_update_stats_.num_increases;
  const float average_update = ComputeAverageUpdate(
      volume_update_stats_.sum_decreases + volume_update_stats_.sum_increases,
      num_updates);
  metrics::HistogramAdd(decrease_rate_histogram_,
                        volume_update_stats_.num_decreases);
  if (volume_update_stats_.num_decreases > 0) {
    metrics::HistogramAdd(decrease_average_histogram_, average_decrease);
  }
  metrics::HistogramAdd(increase_rate_histogram_,
                        volume_update_stats_.num_increases);
  if (volume_update_stats_.num_increases > 0) {
    metrics::HistogramAdd(increase_average_histogram_, average_increase);
  }
  metrics::HistogramAdd(update_rate_histogram_, num_updates);
  if (num_updates > 0) {
    metrics::HistogramAdd(update_average_histogram_, average_update);
  }
}

}  // namespace webrtc
