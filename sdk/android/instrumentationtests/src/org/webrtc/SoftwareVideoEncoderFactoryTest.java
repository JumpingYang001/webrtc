/*
 *  Copyright 2022 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

package org.webrtc;

import static org.junit.Assert.assertEquals;

import androidx.annotation.Nullable;
import androidx.test.filters.SmallTest;
import java.util.ArrayList;
import java.util.HashMap;
import org.junit.Before;
import org.junit.Test;

/** Unit tests for {@link SoftwareVideoEncoderFactory}. */
public class SoftwareVideoEncoderFactoryTest {
  @Before
  public void setUp() {
    NativeLibrary.initialize(new NativeLibrary.DefaultLoader(), TestConstants.NATIVE_LIBRARY);
  }

  @SmallTest
  @Test
  public void getSupportedCodecs_returnsDefaultCodecs() {
    VideoEncoderFactory factory = new SoftwareVideoEncoderFactory();
    VideoCodecInfo[] codecs = factory.getSupportedCodecs();
    assertEquals(3, codecs.length);
    assertEquals("VP8", codecs[0].name);
    assertEquals("AV1", codecs[1].name);
    assertEquals("VP9", codecs[2].name);
  }
}
