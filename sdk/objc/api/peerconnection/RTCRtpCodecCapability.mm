/*
 *  Copyright 2023 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#import "RTCRtpCodecCapability+Private.h"

#import "RTCRtpReceiver+Private.h"

#import "RTCMediaStreamTrack.h"
#import "helpers/NSString+StdString.h"

#include "media/base/media_constants.h"
#include "rtc_base/checks.h"

@implementation RTC_OBJC_TYPE (RTCRtpCodecCapability)

@synthesize nativeCodecCapability = _nativeCodecCapability;

- (instancetype)init {
  return [self initWithNativeCodecCapability:webrtc::RtpCodecCapability()];
}

- (instancetype)initWithNativeCodecCapability:
    (const webrtc::RtpCodecCapability &)nativeCodecCapability {
  if (self = [super init]) {
    _nativeCodecCapability = nativeCodecCapability;
  }

  return self;
}

- (NSString *)mimeType {
  return [NSString stringWithUTF8String:_nativeCodecCapability.mime_type().c_str()];
}

- (NSString *)name {
  return [NSString stringWithUTF8String:_nativeCodecCapability.name.c_str()];
}

- (void)setName:(NSString *)name {
  _nativeCodecCapability.name = std::string([name UTF8String]);
}

- (RTCRtpMediaType)kind {
  return [RTCRtpReceiver mediaTypeForNativeMediaType:_nativeCodecCapability.kind];
}

- (void)setKind:(RTCRtpMediaType)kind {
  _nativeCodecCapability.kind = [RTCRtpReceiver nativeMediaTypeForMediaType:kind];
}

- (NSNumber *)clockRate {
  if (!_nativeCodecCapability.clock_rate) {
    return nil;
  }

  return [NSNumber numberWithInt:*_nativeCodecCapability.clock_rate];
}

- (void)setClockRate:(NSNumber *)clockRate {
  if (clockRate == nil) {
    _nativeCodecCapability.clock_rate = absl::optional<int>();
    return;
  }

  _nativeCodecCapability.clock_rate = absl::optional<int>(clockRate.intValue);
}

- (NSNumber *)preferredPayloadType {
  if (!_nativeCodecCapability.preferred_payload_type) {
    return nil;
  }

  return [NSNumber numberWithInt:*_nativeCodecCapability.preferred_payload_type];
}

- (void)setPreferredPayloadType:(NSNumber *)preferredPayloadType {
  if (preferredPayloadType == nil) {
    _nativeCodecCapability.preferred_payload_type = absl::optional<int>();
    return;
  }

  _nativeCodecCapability.preferred_payload_type =
      absl::optional<int>(preferredPayloadType.intValue);
}

- (NSNumber *)numChannels {
  if (!_nativeCodecCapability.num_channels) {
    return nil;
  }

  return [NSNumber numberWithInt:*_nativeCodecCapability.num_channels];
}

- (void)setNumChannels:(NSNumber *)numChannels {
  if (numChannels == nil) {
    _nativeCodecCapability.num_channels = absl::optional<int>();
    return;
  }

  _nativeCodecCapability.num_channels = absl::optional<int>(numChannels.intValue);
}

- (NSDictionary<NSString *, NSString *> *)parameters {
  NSMutableDictionary *result = [NSMutableDictionary dictionary];
  auto _parameters = _nativeCodecCapability.parameters;
  for (auto it = _parameters.begin(); it != _parameters.end(); ++it) {
    [result setObject:[NSString stringForStdString:it->second]
               forKey:[NSString stringForStdString:it->first]];
  }

  return result;
}

- (void)setParameters:(NSDictionary<NSString *, NSString *> *)parameters {
  std::map<std::string, std::string> _parameters;
  for (NSString *paramKey in parameters.allKeys) {
    std::string key = [NSString stdStringForString:paramKey];
    std::string value = [NSString stdStringForString:parameters[paramKey]];
    _parameters[key] = value;
  }

  _nativeCodecCapability.parameters = _parameters;
}

@end
