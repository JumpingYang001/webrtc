/*
 *  Copyright 2025 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "api/crypto/crypto_options.h"

#include <cstdint>
#include <set>
#include <vector>

#include "rtc_base/openssl_stream_adapter.h"
#include "test/gtest.h"
#include "test/scoped_key_value_config.h"

TEST(EphemeralKeyExchangeCipherGroupsTest, GetSupported) {
  std::set<uint16_t> expected = {
#ifdef SSL_GROUP_SECP224R1
      SSL_GROUP_SECP224R1,
#endif
#ifdef SSL_GROUP_SECP256R1
      SSL_GROUP_SECP256R1,
#endif
#ifdef SSL_GROUP_SECP384R1
      SSL_GROUP_SECP384R1,
#endif
#ifdef SSL_GROUP_SECP521R1
      SSL_GROUP_SECP521R1,
#endif
#ifdef SSL_GROUP_X25519
      SSL_GROUP_X25519,
#endif
#ifdef SSL_GROUP_X25519_MLKEM768
      SSL_GROUP_X25519_MLKEM768,
#endif
  };
  auto supported =
      webrtc::CryptoOptions::EphemeralKeyExchangeCipherGroups::GetSupported();
  for (auto group : expected) {
    EXPECT_TRUE(supported.contains(group));
  }
}

TEST(EphemeralKeyExchangeCipherGroupsTest, GetEnabled) {
  std::vector<uint16_t> expected = {
#ifdef SSL_GROUP_X25519
      SSL_GROUP_X25519,
#endif
#ifdef SSL_GROUP_SECP256R1
      SSL_GROUP_SECP256R1,
#endif
#ifdef SSL_GROUP_SECP384R1
      SSL_GROUP_SECP384R1,
#endif
  };
  webrtc::CryptoOptions::EphemeralKeyExchangeCipherGroups groups;
  EXPECT_EQ(groups.GetEnabled(), expected);
}

TEST(EphemeralKeyExchangeCipherGroupsTest, Update) {
  std::vector<uint16_t> expected = {
#ifdef SSL_GROUP_X25519_MLKEM768
      SSL_GROUP_X25519_MLKEM768,
#endif
#ifdef SSL_GROUP_SECP256R1
      SSL_GROUP_SECP256R1,
#endif
#ifdef SSL_GROUP_SECP384R1
      SSL_GROUP_SECP384R1,
#endif
  };

  std::vector<uint16_t> disable = {
#ifdef SSL_GROUP_X25519
      SSL_GROUP_X25519,
#endif
  };

  webrtc::CryptoOptions::EphemeralKeyExchangeCipherGroups groups;
  webrtc::test::ScopedKeyValueConfig field_trials(
      "WebRTC-EnableDtlsPqc/Enabled/");
  groups.Update(&field_trials, &disable);
  EXPECT_EQ(groups.GetEnabled(), expected);
}
