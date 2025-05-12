/*
 *  Copyright 2022 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "api/field_trials.h"


#include "rtc_base/containers/flat_set.h"
#include "system_wrappers/include/field_trial.h"
#include "test/field_trial.h"
#include "test/gtest.h"

namespace webrtc {
namespace {

using field_trial::FieldTrialsAllowedInScopeForTesting;
using test::ScopedFieldTrials;

TEST(FieldTrialsTest, EmptyStringHasNoEffect) {
  FieldTrialsAllowedInScopeForTesting k({"MyCoolTrial"});
  FieldTrials f("");
  f.RegisterKeysForTesting({"MyCoolTrial"});

  EXPECT_FALSE(f.IsEnabled("MyCoolTrial"));
  EXPECT_FALSE(f.IsDisabled("MyCoolTrial"));
}

TEST(FieldTrialsTest, EnabledDisabledMustBeFirstInValue) {
  FieldTrials f(
      "MyCoolTrial/EnabledFoo/"
      "MyUncoolTrial/DisabledBar/"
      "AnotherTrial/BazEnabled/");
  f.RegisterKeysForTesting({"MyCoolTrial", "MyUncoolTrial", "AnotherTrial"});

  EXPECT_TRUE(f.IsEnabled("MyCoolTrial"));
  EXPECT_TRUE(f.IsDisabled("MyUncoolTrial"));
  EXPECT_FALSE(f.IsEnabled("AnotherTrial"));
}

TEST(FieldTrialsTest, FieldTrialsDoesNotReadGlobalString) {
  FieldTrialsAllowedInScopeForTesting k({"MyCoolTrial", "MyUncoolTrial"});
  ScopedFieldTrials g("MyCoolTrial/Enabled/MyUncoolTrial/Disabled/");
  FieldTrials f("");
  f.RegisterKeysForTesting({"MyCoolTrial", "MyUncoolTrial"});

  EXPECT_FALSE(f.IsEnabled("MyCoolTrial"));
  EXPECT_FALSE(f.IsDisabled("MyUncoolTrial"));
}

TEST(FieldTrialsTest, FieldTrialsInstanceDoesNotModifyGlobalString) {
  FieldTrialsAllowedInScopeForTesting k({"SomeString"});
  FieldTrials f("SomeString/Enabled/");
  f.RegisterKeysForTesting({"SomeString"});

  EXPECT_TRUE(f.IsEnabled("SomeString"));
  EXPECT_FALSE(field_trial::IsEnabled("SomeString"));
}

TEST(FieldTrialsTest, FieldTrialsSupportSimultaneousInstances) {
  FieldTrials f1("SomeString/Enabled/");
  FieldTrials f2("SomeOtherString/Enabled/");
  f1.RegisterKeysForTesting({"SomeString", "SomeOtherString"});
  f2.RegisterKeysForTesting({"SomeString", "SomeOtherString"});

  EXPECT_TRUE(f1.IsEnabled("SomeString"));
  EXPECT_FALSE(f1.IsEnabled("SomeOtherString"));

  EXPECT_FALSE(f2.IsEnabled("SomeString"));
  EXPECT_TRUE(f2.IsEnabled("SomeOtherString"));
}

TEST(FieldTrialsTest, GlobalAndNonGlobalFieldTrialsAreDisjoint) {
  FieldTrialsAllowedInScopeForTesting k({"SomeString", "SomeOtherString"});
  ScopedFieldTrials g("SomeString/Enabled/");
  FieldTrials f("SomeOtherString/Enabled/");

  f.RegisterKeysForTesting({"SomeString", "SomeOtherString"});

  EXPECT_TRUE(field_trial::IsEnabled("SomeString"));
  EXPECT_FALSE(field_trial::IsEnabled("SomeOtherString"));

  EXPECT_FALSE(f.IsEnabled("SomeString"));
  EXPECT_TRUE(f.IsEnabled("SomeOtherString"));
}

}  // namespace
}  // namespace webrtc
