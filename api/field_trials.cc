/*
 *  Copyright (c) 2020 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "api/field_trials.h"

#include <string>
#include <utility>

#include "absl/strings/string_view.h"
#include "rtc_base/checks.h"
#include "rtc_base/containers/flat_map.h"

namespace webrtc {

FieldTrials::FieldTrials(absl::string_view s) {
  while (!s.empty()) {
    absl::string_view::size_type separator_pos = s.find('/');
    RTC_CHECK_NE(separator_pos, absl::string_view::npos)
        << "Missing separator '/' after field trial key.";
    RTC_CHECK_GT(separator_pos, 0) << "Field trial key cannot be empty.";
    absl::string_view key = s.substr(0, separator_pos);
    s.remove_prefix(separator_pos + 1);

    RTC_CHECK(!s.empty())
        << "Missing value after field trial key. String ended.";
    separator_pos = s.find('/');
    RTC_CHECK_NE(separator_pos, absl::string_view::npos)
        << "Missing terminating '/' in field trial string.";
    RTC_CHECK_GT(separator_pos, 0) << "Field trial value cannot be empty.";
    absl::string_view value = s.substr(0, separator_pos);
    s.remove_prefix(separator_pos + 1);

    // If a key is specified multiple times, only the value linked to the first
    // key is stored.
    key_value_map_.emplace(key, value);
  }
}

std::string FieldTrials::GetValue(absl::string_view key) const {
  auto it = key_value_map_.find(key);
  if (it != key_value_map_.end()) {
    return it->second;
  }

  return "";
}

}  // namespace webrtc
