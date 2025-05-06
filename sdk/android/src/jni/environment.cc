/*
 *  Copyright 2025 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#include "api/environment/environment.h"

#include <string>

#include "api/environment/environment_factory.h"
#include "api/field_trials.h"
#include "sdk/android/generated_environment_jni/Environment_jni.h"
#include "sdk/android/native_api/jni/java_types.h"
#include "third_party/jni_zero/jni_zero.h"

namespace webrtc {
namespace jni {

void JNI_Environment_Free(JNIEnv* /*env*/, jlong webrtc_env) {
  delete reinterpret_cast<Environment*>(webrtc_env);
}

jlong JNI_Environment_Create(
    JNIEnv* jni,
    const jni_zero::JavaParamRef<jstring>& j_field_trials) {
  std::unique_ptr<FieldTrialsView> field_trials;
  if (!j_field_trials.is_null()) {
    field_trials =
        FieldTrials::CreateNoGlobal(JavaToNativeString(jni, j_field_trials));
  }
  return NativeToJavaPointer(
      new Environment(CreateEnvironment(std::move(field_trials))));
}

}  // namespace jni
}  // namespace webrtc
