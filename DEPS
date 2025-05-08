# This file contains dependencies for WebRTC.

gclient_gn_args_file = 'src/build/config/gclient_args.gni'
gclient_gn_args = [
  'generate_location_tags',
]

vars = {
  # By default, we should check out everything needed to run on the main
  # chromium waterfalls. More info at: crbug.com/570091.
  'checkout_configuration': 'default',
  'checkout_instrumented_libraries': 'checkout_linux and checkout_configuration == "default"',
  'chromium_revision': 'a543dd4ebed1c7abc66739418691b8392fc9cca7',

  # Fetch the prebuilt binaries for llvm-cov and llvm-profdata. Needed to
  # process the raw profiles produced by instrumented targets (built with
  # the gn arg 'use_clang_coverage').
  'checkout_clang_coverage_tools': False,

  # Fetch clangd into the same bin/ directory as our clang binary.
  'checkout_clangd': False,

  # Fetch clang-tidy into the same bin/ directory as our clang binary.
  'checkout_clang_tidy': False,

  # Fetch libraries required to compile and run fuzzer tests.
  'checkout_fuzzer': False,

  'chromium_git': 'https://chromium.googlesource.com',

  # Keep the Chromium default of generating location tags.
  'generate_location_tags': True,

  # ResultDB version
  'resultdb_version': 'git_revision:ebc74d10fa0d64057daa6f128e89f3672eeeec95',

  # By default, download the fuchsia sdk from the public sdk directory.
  'fuchsia_sdk_cipd_prefix': 'fuchsia/sdk/core/',
  'fuchsia_version': 'version:28.20250508.3.1',
  # By default, download the fuchsia images from the fuchsia GCS bucket.
  'fuchsia_images_bucket': 'fuchsia',
  'checkout_fuchsia': False,
  # Since the images are hundreds of MB, default to only downloading the image
  # most commonly useful for developers. Bots and developers that need to use
  # other images can override this with additional images.
  'checkout_fuchsia_boot_images': "terminal.x64",
  'checkout_fuchsia_product_bundles': '"{checkout_fuchsia_boot_images}" != ""',

  # Fetch configuration files required for the 'use_remoteexec' gn arg
  'download_remoteexec_cfg': False,
  # RBE instance to use for running remote builds
  'rbe_instance': 'projects/rbe-webrtc-developer/instances/default_instance',
  # reclient CIPD package version
  'reclient_version': 're_client_version:0.177.1.e58c0145-gomaip',
  # siso CIPD package version.
  'siso_version': 'git_revision:17b491ae74e86312a96239ff88e852acee83c135',

  # ninja CIPD package.
  'ninja_package': 'infra/3pp/tools/ninja/',

  # ninja CIPD package version
  # https://chrome-infra-packages.appspot.com/p/infra/3pp/tools/ninja
  'ninja_version': 'version:3@1.12.1.chromium.4',

  # condition to allowlist deps for non-git-source processing.
  'non_git_source': 'True',

  # This can be overridden, e.g. with custom_vars, to build clang from HEAD
  # instead of downloading the prebuilt pinned revision.
  'llvm_force_head_revision': False,
}

deps = {
  # TODO(kjellander): Move this to be Android-only.
  'src/base':
    'https://chromium.googlesource.com/chromium/src/base@d20957e106d7e5274e7d635c42844ed07651822f',
  'src/build':
    'https://chromium.googlesource.com/chromium/src/build@d9c3040247e3b793c2a053d50fe2376e7596cd95',
  'src/buildtools':
    'https://chromium.googlesource.com/chromium/src/buildtools@077a66f30fcf281b066fafb6dfc60818c238efb6',
  # Gradle 6.6.1. Used for testing Android Studio project generation for WebRTC.
  'src/examples/androidtests/third_party/gradle': {
    'url': 'https://chromium.googlesource.com/external/github.com/gradle/gradle.git@f2d1fb54a951d8b11d25748e4711bec8d128d7e3',
    'condition': 'checkout_android',
  },
  'src/ios': {
    'url': 'https://chromium.googlesource.com/chromium/src/ios@0ec92f3b3d6944e8c440720453be2d0ec4c79a3d',
    'condition': 'checkout_ios',
  },
  'src/testing':
    'https://chromium.googlesource.com/chromium/src/testing@3d17983c5bfeafd053c8310e709b3ec9c0b3a332',
  'src/third_party':
    'https://chromium.googlesource.com/chromium/src/third_party@cb05a5b3b1f60b4d4e2d60a9f4f8c1503122df94',

  'src/buildtools/linux64': {
    'packages': [
      {
        'package': 'gn/gn/linux-${{arch}}',
        'version': 'git_revision:487f8353f15456474437df32bb186187b0940b45',
      }
    ],
    'dep_type': 'cipd',
    'condition': 'checkout_linux',
  },
  'src/buildtools/mac': {
    'packages': [
      {
        'package': 'gn/gn/mac-${{arch}}',
        'version': 'git_revision:487f8353f15456474437df32bb186187b0940b45',
      }
    ],
    'dep_type': 'cipd',
    'condition': 'checkout_mac',
  },
  'src/buildtools/win': {
    'packages': [
      {
        'package': 'gn/gn/windows-amd64',
        'version': 'git_revision:487f8353f15456474437df32bb186187b0940b45',
      }
    ],
    'dep_type': 'cipd',
    'condition': 'checkout_win',
  },
  'src/buildtools/reclient': {
    'packages': [
      {
         # https://chrome-infra-packages.appspot.com/p/infra/rbe/client/
        'package': 'infra/rbe/client/${{platform}}',
        'version': Var('reclient_version'),
      }
    ],
    'dep_type': 'cipd',
    # Reclient doesn't have linux-arm64 package.
    'condition': 'not (host_os == "linux" and host_cpu == "arm64")',
  },

  'src/third_party/llvm-build/Release+Asserts': {
    'dep_type': 'gcs',
    'bucket': 'chromium-browser-clang',
    'condition': 'not llvm_force_head_revision',
    'objects': [
      {
        # The Android libclang_rt.builtins libraries are currently only included in the Linux clang package.
        'object_name': 'Linux_x64/clang-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': 'ecd163a9ba1a622fd761ff426c73b6ff95b3697cb18baa9183a4b414072d8633',
        'size_bytes': 55589720,
        'generation': 1746525620539676,
        'condition': '(host_os == "linux" or checkout_android) and non_git_source',
      },
      {
        'object_name': 'Linux_x64/clang-tidy-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': '2cb4311b17b400ae969d691e90576d1e110d555cb366e0a007b01272f2231e4e',
        'size_bytes': 13570756,
        'generation': 1746525620814582,
        'condition': 'host_os == "linux" and checkout_clang_tidy and non_git_source',
      },
      {
        'object_name': 'Linux_x64/clangd-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': '4758d370c079cb70da49ba510e434871c2f4f2cb96ad5e4eeb523d669ccd016b',
        'size_bytes': 13824764,
        'generation': 1746525620948078,
        'condition': 'host_os == "linux" and checkout_clangd and non_git_source',
      },
      {
        'object_name': 'Linux_x64/llvm-code-coverage-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': '97d1fe6e3081f938c0bd691197f1326e804c5859644a3550259fef223c856138',
        'size_bytes': 2295328,
        'generation': 1746525621371092,
        'condition': 'host_os == "linux" and checkout_clang_coverage_tools and non_git_source',
      },
      {
        'object_name': 'Linux_x64/llvmobjdump-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': 'df8518b635e60716b430321800cbbb083dde39a3fa856b0a865aefd7dd915a81',
        'size_bytes': 5699744,
        'generation': 1746525621090221,
        'condition': '((checkout_linux or checkout_mac or checkout_android) and host_os == "linux") and non_git_source',
      },
      {
        'object_name': 'Mac/clang-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': '220dfa6acb4468dfa83f353215ff741ca2b30738f028dc2f8a7b3b9c5f45bb14',
        'size_bytes': 52096988,
        'generation': 1746525623855538,
        'condition': 'host_os == "mac" and host_cpu == "x64"',
      },
      {
        'object_name': 'Mac/clang-mac-runtime-library-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': '41f00c834c5f4f81dfa07477d79a64fff17531759d6df7256a72111590645ae8',
        'size_bytes': 993908,
        'generation': 1746525645453067,
        'condition': 'checkout_mac and not host_os == "mac"',
      },
      {
        'object_name': 'Mac/clang-tidy-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': '4160f7a53e99c7c6fe4e589d20874b12a068625d92209bafdf3ccb1edeec37cf',
        'size_bytes': 13670156,
        'generation': 1746525624439549,
        'condition': 'host_os == "mac" and host_cpu == "x64" and checkout_clang_tidy',
      },
      {
        'object_name': 'Mac/clangd-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': '04a3d9cb8e8742c5807d302b6f46e1dcfcdd36ad55ae8f9894c558f28db93f08',
        'size_bytes': 15092288,
        'generation': 1746525624237513,
        'condition': 'host_os == "mac" and host_cpu == "x64" and checkout_clangd',
      },
      {
        'object_name': 'Mac/llvm-code-coverage-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': 'effe34835ea45e43b21d9aab0790f21b0291966614ddda23ab37eeea26a188ff',
        'size_bytes': 2270324,
        'generation': 1746525624667951,
        'condition': 'host_os == "mac" and host_cpu == "x64" and checkout_clang_coverage_tools',
      },
      {
        'object_name': 'Mac_arm64/clang-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': 'f0c7db116dd1dc93f8935baf3ee130f4f2eb72c4953d0e132dea473a030194ba',
        'size_bytes': 44103784,
        'generation': 1746525647078692,
        'condition': 'host_os == "mac" and host_cpu == "arm64"',
      },
      {
        'object_name': 'Mac_arm64/clang-tidy-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': '32e49f6d1339be7c0387e404930aafb25c460b9396f1f5801a6fdd107fcbacb0',
        'size_bytes': 11823236,
        'generation': 1746525647324075,
        'condition': 'host_os == "mac" and host_cpu == "arm64" and checkout_clang_tidy',
      },
      {
        'object_name': 'Mac_arm64/clangd-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': '7f77dbff038c35c2a688b4c8d05fa2b22cecc7d175659dbe88c88f952fa63fbd',
        'size_bytes': 12089948,
        'generation': 1746525647467078,
        'condition': 'host_os == "mac" and host_cpu == "arm64" and checkout_clangd',
      },
      {
        'object_name': 'Mac_arm64/llvm-code-coverage-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': '71c858f90180069f7ef0b16f8e7869ed319bae00c984179cbdd25956fa6c73ff',
        'size_bytes': 1979552,
        'generation': 1746525647890816,
        'condition': 'host_os == "mac" and host_cpu == "arm64" and checkout_clang_coverage_tools',
      },
      {
        'object_name': 'Win/clang-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': 'd04ead6c2da3e38558d6bb19e06b05aa6dfa6b4b82b678c25295c803bb378c86',
        'size_bytes': 47099456,
        'generation': 1746525671111178,
        'condition': 'host_os == "win"',
      },
      {
        'object_name': 'Win/clang-tidy-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': '3b026fb8d3363b04a7434d0df710c2ebf8074e9aca7a236447cc0efb0718ec82',
        'size_bytes': 13470524,
        'generation': 1746525671342028,
        'condition': 'host_os == "win" and checkout_clang_tidy',
      },
      {
        'object_name': 'Win/clang-win-runtime-library-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': 'b6a49e302e5c9d562e575e1c1431777d90d4950dc7e6bba9b1c21d0ceba93c29',
        'size_bytes': 2484356,
        'generation': 1746525692368377,
        'condition': 'checkout_win and not host_os == "win"',
      },
      {
        'object_name': 'Win/clangd-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': '51953564440fd82547a27b1fe0cd76d9a0c200162ee7530fe14d805511613be7',
        'size_bytes': 13886984,
        'generation': 1746525671497511,
        'condition': 'host_os == "win" and checkout_clangd',
      },
      {
        'object_name': 'Win/llvm-code-coverage-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': 'e5e5a685fde6a4ddd211913ccf6f8e62ac7e6567fec26d68d5bff3aeac42c330',
        'size_bytes': 2376076,
        'generation': 1746525671928280,
        'condition': 'host_os == "win" and checkout_clang_coverage_tools',
      },
      {
        'object_name': 'Win/llvmobjdump-llvmorg-21-init-10851-gcd6c4b61-1.tar.xz',
        'sha256sum': 'd02d624d3ecfe5dbaa2e93b2da5a6443e4f25b9fe2fab43b8b540fccc91e4770',
        'size_bytes': 5687508,
        'generation': 1746525671637063,
        'condition': '(checkout_linux or checkout_mac or checkout_android) and host_os == "win"',
      },
    ]
  },

  # Update prebuilt Rust toolchain.
  'src/third_party/rust-toolchain': {
    'dep_type': 'gcs',
    'bucket': 'chromium-browser-clang',
    'objects': [
      {
        'object_name': 'Linux_x64/rust-toolchain-4a0969e06dbeaaa43914d2d00b2e843d49aa3886-1-llvmorg-21-init-10851-gcd6c4b61.tar.xz',
        'sha256sum': '2d8498b5dcab2954c8212564798252c3edc9c0181ea8b05d3d6ae42b7b6adf7d',
        'size_bytes': 118439104,
        'generation': 1746525613682798,
        'condition': 'host_os == "linux" and non_git_source',
      },
      {
        'object_name': 'Mac/rust-toolchain-4a0969e06dbeaaa43914d2d00b2e843d49aa3886-1-llvmorg-21-init-10851-gcd6c4b61.tar.xz',
        'sha256sum': '93e57858344a978ce2df2d9926c541d8e25cd9def0054328dba4ac13ff633d34',
        'size_bytes': 111691112,
        'generation': 1746525615270255,
        'condition': 'host_os == "mac" and host_cpu == "x64"',
      },
      {
        'object_name': 'Mac_arm64/rust-toolchain-4a0969e06dbeaaa43914d2d00b2e843d49aa3886-1-llvmorg-21-init-10851-gcd6c4b61.tar.xz',
        'sha256sum': '4c1f9d7ff8c1febdb755ad971b0ae8d9b3c5bed5b30b96ea00caa784dfb86427',
        'size_bytes': 101778572,
        'generation': 1746525616855514,
        'condition': 'host_os == "mac" and host_cpu == "arm64"',
      },
      {
        'object_name': 'Win/rust-toolchain-4a0969e06dbeaaa43914d2d00b2e843d49aa3886-1-llvmorg-21-init-10851-gcd6c4b61.tar.xz',
        'sha256sum': '7d2484e229f4015d3711f1294d11048764ec22ebaf8266701abf350cd51c897b',
        'size_bytes': 193318196,
        'generation': 1746525618539172,
        'condition': 'host_os == "win"',
      },
    ],
  },

  'src/third_party/clang-format/script':
    'https://chromium.googlesource.com/external/github.com/llvm/llvm-project/clang/tools/clang-format.git@37f6e68a107df43b7d7e044fd36a13cbae3413f2',
  'src/third_party/compiler-rt/src':
    'https://chromium.googlesource.com/external/github.com/llvm/llvm-project/compiler-rt.git@16c0e42913a45357f0e557dcc0b7055129c78af6',
  'src/third_party/libc++/src':
    'https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx.git@a01c02c9d4acbdae3b7e8a2f3ee58579a9c29f96',
  'src/third_party/libc++abi/src':
    'https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxxabi.git@7ff13dd8c66d5047a3ee1e1ba26ddeb70d3d8740',
  'src/third_party/llvm-libc/src':
    'https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libc.git@e3e030ec6ee1674bf2195d0cfd0a4bf5fee16537',
  'src/third_party/libunwind/src':
    'https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libunwind.git@81e2cb40a70de2b6978e6d8658891ded9a77f7e3',

  'src/third_party/test_fonts/test_fonts': {
      'dep_type': 'gcs',
      'condition': 'non_git_source',
      'bucket': 'chromium-fonts',
      'objects': [
          {
              'object_name': 'f26f29c9d3bfae588207bbc9762de8d142e58935c62a86f67332819b15203b35',
              'sha256sum': 'f26f29c9d3bfae588207bbc9762de8d142e58935c62a86f67332819b15203b35',
              'size_bytes': 32750602,
              'generation': 1717109450425063,
          },
      ],
  },

  'src/third_party/ninja': {
    'packages': [
      {
        'package': Var('ninja_package') + '${{platform}}',
        'version': Var('ninja_version'),
      }
    ],
    'condition': 'non_git_source',
    'dep_type': 'cipd',
  },

  'src/third_party/siso/cipd': {
    'packages': [
      {
        'package': 'infra/build/siso/${{platform}}',
        'version': Var('siso_version'),
      }
    ],
    'condition': 'non_git_source',
    'dep_type': 'cipd',
  },

  'src/third_party/android_system_sdk/cipd': {
      'packages': [
          {
              'package': 'chromium/third_party/android_system_sdk/public',
              'version': 'Pfb3HDUW_uRir_VVTCYkGhf6bnPPF55NUJO2WXOxIe0C',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/tools/resultdb': {
    'packages': [
      {
        'package': 'infra/tools/result_adapter/${{platform}}',
        'version': Var('resultdb_version'),
      },
    ],
    'dep_type': 'cipd',
  },

  'src/third_party/android_build_tools/aapt2/cipd': {
      'packages': [
          {
              'package': 'chromium/third_party/android_build_tools/aapt2',
              'version': '_lNsOL_GGlXLOIMGtrbMOqNd7TQHabaP1q8SlvUpFbMC',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },

  'src/third_party/android_build_tools/bundletool/cipd': {
      'packages': [
          {
               'package': 'chromium/third_party/android_build_tools/bundletool',
               'version': 'zV93G9_1s5h6x7c2qdcibr0uuQ_5Q2QgcxhkUs9-tOsC',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },

  'src/third_party/android_build_tools/dagger_compiler/cipd': {
      'packages': [
          {
               'package': 'chromium/third_party/android_build_tools/dagger_compiler',
               'version': 'AC0DoTEXQf40KFt7hyCNSEJPrT9Rprw9zsZxNKdw7BQC',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_build_tools/error_prone/cipd': {
      'packages': [
          {
               'package': 'chromium/third_party/android_build_tools/error_prone',
               'version': 'FLtgIzcLepAc87BaSXIELZIJPT4yxnr8K3MmFaEHrK8C',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },

  'src/third_party/android_build_tools/error_prone_javac/cipd': {
      'packages': [
          {
               'package': 'chromium/third_party/android_build_tools/error_prone_javac',
               'version': '7EcHxlEXEaLRWEyHIAxf0ouPjkmN1Od6jkutuo0sfBIC',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },

  'src/third_party/android_build_tools/lint/cipd': {
      'packages': [
          {
               'package': 'chromium/third_party/android_build_tools/lint',
               'version': '2fKjMen4v_UU1CDYJqhwJCr6uh-L5w25n9lAKx6SlP0C',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  # TODO(webrtc:42223878): This is only needed for //base.
  'src/third_party/android_build_tools/nullaway/cipd': {
      'packages': [
          {
               'package': 'chromium/third_party/android_build_tools/nullaway',
               'version': 'mFkUqdlIqmMQ3rAaTHFmOBYjNFgFUQOmMlPG6PGgmHYC',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/aosp_dalvik/cipd': {
      'packages': [
          {
              'package': 'chromium/third_party/aosp_dalvik/linux-amd64',
              'version': 'version:2@13.0.0_r24.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/boringssl/src':
    'https://boringssl.googlesource.com/boringssl.git@864a235afcf4d2575b1eab8de96fbf0d84f6cda9',
  'src/third_party/breakpad/breakpad':
    'https://chromium.googlesource.com/breakpad/breakpad.git@b319c12317824bfd0e92828db20bbfe0ec6e31f8',
  'src/third_party/catapult':
    'https://chromium.googlesource.com/catapult.git@64c31ffa4d735add4a7e7520a52e0e3160216132',
  'src/third_party/ced/src': {
    'url': 'https://chromium.googlesource.com/external/github.com/google/compact_enc_det.git@ba412eaaacd3186085babcd901679a48863c7dd5',
  },
  'src/third_party/colorama/src':
    'https://chromium.googlesource.com/external/colorama.git@3de9f013df4b470069d03d250224062e8cf15c49',
  'src/third_party/cpu_features/src': {
    'url': 'https://chromium.googlesource.com/external/github.com/google/cpu_features.git@936b9ab5515dead115606559502e3864958f7f6e',
    'condition': 'checkout_android',
  },
  'src/third_party/crc32c/src':
    'https://chromium.googlesource.com/external/github.com/google/crc32c.git@d3d60ac6e0f16780bcfcc825385e1d338801a558',
  'src/third_party/depot_tools':
    'https://chromium.googlesource.com/chromium/tools/depot_tools.git@fab0a4296b8830c569a4dc82285bfb58a5c2fca8',
  'src/third_party/ffmpeg':
    'https://chromium.googlesource.com/chromium/third_party/ffmpeg.git@dcdd0fa51b65a0b1688ff6b8f0cc81908f09ded2',
  'src/third_party/flatbuffers/src':
    'https://chromium.googlesource.com/external/github.com/google/flatbuffers.git@8db59321d9f02cdffa30126654059c7d02f70c32',
  'src/third_party/grpc/src': {
    'url': 'https://chromium.googlesource.com/external/github.com/grpc/grpc.git@e21fb6701eba9e55f33b427d82010b8f8ce857d7',
  },
  # Used for embedded builds. CrOS & Linux use the system version.
  'src/third_party/fontconfig/src': {
      'url': 'https://chromium.googlesource.com/external/fontconfig.git@14d466b30a8ab4a9d789977ed94f2c30e7209267',
      'condition': 'checkout_linux',
  },
  'src/third_party/freetype/src':
    'https://chromium.googlesource.com/chromium/src/third_party/freetype2.git@79912716e783b4c9e47c35907ae080e2ec02fbc6',
  'src/third_party/harfbuzz-ng/src':
    'https://chromium.googlesource.com/external/github.com/harfbuzz/harfbuzz.git@9f83bbbe64654b45ba5bb06927ff36c2e7588495',
  'src/third_party/google_benchmark/src': {
    'url': 'https://chromium.googlesource.com/external/github.com/google/benchmark.git@761305ec3b33abf30e08d50eb829e19a802581cc',
  },
  # WebRTC-only dependency (not present in Chromium).
  'src/third_party/gtest-parallel':
    'https://chromium.googlesource.com/external/github.com/google/gtest-parallel@96f4f904922f9bf66689e749c40f314845baaac8',
  'src/third_party/google-truth/src': {
      'url': 'https://chromium.googlesource.com/external/github.com/google/truth.git@33387149b465f82712a817e6744847fe136949b3',
      'condition': 'checkout_android',
  },
  'src/third_party/googletest/src':
    'https://chromium.googlesource.com/external/github.com/google/googletest.git@90a41521142c978131f38c6da07b4eb96a9f1ff6',
  'src/third_party/icu': {
    'url': 'https://chromium.googlesource.com/chromium/deps/icu.git@b929596baebf0ab4ac7ec07f38365db4c50a559d',
  },
  'src/third_party/jdk/current': {
      'packages': [
          {
              'package': 'chromium/third_party/jdk/linux-amd64',
              'version': '2iiuF-nKDH3moTImx2op4WTRetbfhzKoZhH7Xo44zGsC',
          },
      ],
      # Needed on Linux for use on chromium_presubmit (for checkstyle).
      'condition': '(checkout_android or checkout_linux) and non_git_source',
      'dep_type': 'cipd',
  },
  # Deprecated - only use for tools which are broken real JDK.
  # Not used by WebRTC. Added for compatibility with Chromium.
  'src/third_party/jdk11': {
      'packages': [
          {
              'package': 'chromium/third_party/jdk',
              # Do not update this hash - any newer hash will point to JDK17+.
              'version': 'egbcSHbmF1XZQbKxp_PQiGLFWlQK65krTGqQE-Bj4j8C',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },
 'src/third_party/jsoncpp/source':
    'https://chromium.googlesource.com/external/github.com/open-source-parsers/jsoncpp.git@42e892d96e47b1f6e29844cc705e148ec4856448', # from svn 248
  'src/third_party/junit/src': {
    'url': 'https://chromium.googlesource.com/external/junit.git@0eb5ce72848d730da5bd6d42902fdd6a8a42055d',
    'condition': 'checkout_android',
  },
  'src/third_party/kotlin_stdlib/cipd': {
      'packages': [
          {
              'package': 'chromium/third_party/kotlin_stdlib',
              'version': 'aHCAN6w_ZZCJL7MOPlDbEieRsiUs6ftT33fCI4bbwVwC',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },

  'src/third_party/kotlinc/current': {
      'packages': [
          {
              'package': 'chromium/third_party/kotlinc',
              'version': 'DMzAXJeHsmdOETLSEAYTfnPUX4VvhvmX2__q71V_Ia8C',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },
  'src/third_party/libFuzzer/src':
    'https://chromium.googlesource.com/external/github.com/llvm/llvm-project/compiler-rt/lib/fuzzer.git@e31b99917861f891308269c36a32363b120126bb',
  'src/third_party/fuzztest/src':
    'https://chromium.googlesource.com/external/github.com/google/fuzztest.git@9c1ffdc7fb3d8db64e42c7a9662d8b9906625b77',
  'src/third_party/libjpeg_turbo':
    'https://chromium.googlesource.com/chromium/deps/libjpeg_turbo.git@e14cbfaa85529d47f9f55b0f104a579c1061f9ad',
  'src/third_party/libsrtp':
    'https://chromium.googlesource.com/chromium/deps/libsrtp.git@a52756acb1c5e133089c798736dd171567df11f5',
  'src/third_party/dav1d/libdav1d':
    'https://chromium.googlesource.com/external/github.com/videolan/dav1d.git@8d956180934f16244bdb58b39175824775125e55',
  'src/third_party/libaom/source/libaom':
    'https://aomedia.googlesource.com/aom.git@378f15d1cbd5102f89a52881bcdcf2c010f3c5c8',
  'src/third_party/libunwindstack': {
      'url': 'https://chromium.googlesource.com/chromium/src/third_party/libunwindstack.git@0d758dd57f42564acecdd7a1e7ac5c8521c1b01a',
      'condition': 'checkout_android',
  },
  'src/third_party/perfetto':
    Var('chromium_git') + '/external/github.com/google/perfetto.git' + '@' + 'e2d7750acd15d57c6c88e3c8fb8c5a45f424ba96',
  'src/third_party/protobuf-javascript/src':
    Var('chromium_git') + '/external/github.com/protocolbuffers/protobuf-javascript' + '@' + 'eb785a9363664a402b6336dfe96aad27fb33ffa8',
  'src/third_party/libvpx/source/libvpx':
    'https://chromium.googlesource.com/webm/libvpx.git@6bb288633613c613c87d23b32e5f1d23102b1a43',
  'src/third_party/libyuv':
    'https://chromium.googlesource.com/libyuv/libyuv.git@4db2af62dab48895226be6b52737247e898ebe36',
  'src/third_party/lss': {
    'url': 'https://chromium.googlesource.com/linux-syscall-support.git@ed31caa60f20a4f6569883b2d752ef7522de51e0',
    'condition': 'checkout_android or checkout_linux',
  },
  'src/third_party/mockito/src': {
    'url': 'https://chromium.googlesource.com/external/mockito/mockito.git@04a2a289a4222f80ad20717c25144981210d2eac',
    'condition': 'checkout_android',
  },
  'src/third_party/instrumented_libs': {
    'url': Var('chromium_git') + '/chromium/third_party/instrumented_libraries.git' + '@' + '69015643b3f68dbd438c010439c59adc52cac808',
    'condition': 'checkout_instrumented_libraries',
  },

  # Used by boringssl.
  'src/third_party/nasm': {
      'url': 'https://chromium.googlesource.com/chromium/deps/nasm.git@9f916e90e6fc34ec302573f6ce147e43e33d68ca'
  },

  'src/third_party/openh264/src':
    'https://chromium.googlesource.com/external/github.com/cisco/openh264@652bdb7719f30b52b08e506645a7322ff1b2cc6f',

  'src/third_party/re2/src':
    'https://chromium.googlesource.com/external/github.com/google/re2.git@c84a140c93352cdabbfb547c531be34515b12228',

  'src/third_party/r8/cipd': {
      'packages': [
          {
              'package': 'chromium/third_party/r8',
              'version': 'UDiEQW04Q7U0lSlAQAik0xbIV-AdjAnCHl7GcecroU4C',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },
  # This duplication is intentional, so we avoid updating the r8.jar used by
  # dexing unless necessary, since each update invalidates all incremental
  # dexing and unnecessarily slows down all bots.
  'src/third_party/r8/d8/cipd': {
      'packages': [
          {
              'package': 'chromium/third_party/r8',
              'version': 'bA3htCoEd_EArHekDGQSNpmBzQrcby2ioG6SFyl3AtwC',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },
  'src/third_party/requests/src': {
    'url': 'https://chromium.googlesource.com/external/github.com/kennethreitz/requests.git@c7e0fc087ceeadb8b4c84a0953a422c474093d6d',
    'condition': 'checkout_android',
  },
  'src/tools':
    'https://chromium.googlesource.com/chromium/src/tools@779ed26f147aae6da64703617484084a7db117db',

  'src/third_party/espresso': {
      'packages': [
          {
              'package': 'chromium/third_party/espresso',
              'version': '5LoBT0j383h_4dXbnap7gnNQMtMjpbMJD1JaGIYNj-IC',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },

  'src/third_party/hamcrest/cipd': {
      'packages': [
          {
              'package': 'chromium/third_party/hamcrest',
              'version': 'dBioOAmFJjqAr_DY7dipbXdVfAxUQwjOBNibMPtX8lQC',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },

  'src/third_party/android_toolchain/ndk': {
    'packages': [
      {
        'package': 'chromium/third_party/android_toolchain/android_toolchain',
        'version': 'KXOia11cm9lVdUdPlbGLu8sCz6Y4ey_HV2s8_8qeqhgC',
      },
    ],
    'condition': 'checkout_android',
    'dep_type': 'cipd',
  },

  'src/third_party/androidx/cipd': {
    'packages': [
      {
          'package': 'chromium/third_party/androidx',
          'version': '0kpSWsOkTy_kz_7BrmXaMj0RWlcTCIC9WOKWl3eSM4cC',
      },
    ],
    'condition': 'checkout_android and non_git_source',
    'dep_type': 'cipd',
  },

  'src/third_party/android_build_tools/manifest_merger/cipd': {
      'packages': [
          {
               'package': 'chromium/third_party/android_build_tools/manifest_merger',
               'version': 'GnPGgBK8kiq32GuJB3iI20yJbFjgt2dqHsmwoPShJ-4C',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },

  'src/third_party/android_sdk/public': {
      'packages': [
          {
              'package': 'chromium/third_party/android_sdk/public/build-tools/36.0.0',
              'version': 'y3EsZLg4bxPmpW0oYsAHylywNyMnIwPS3kh1VbQLAFAC',
          },
          {
              'package': 'chromium/third_party/android_sdk/public/emulator',
              'version': '9lGp8nTUCRRWGMnI_96HcKfzjnxEJKUcfvfwmA3wXNkC',
          },
          {
              'package': 'chromium/third_party/android_sdk/public/platform-tools',
              'version': 'mjFmRj7k_XR9yj60pYbr9mG38FyEbU5oWdU56bZQ5cwC'
          },
          {
              'package': 'chromium/third_party/android_sdk/public/platforms/android-36',
              'version': '_YHemUrK49JrE7Mctdf5DDNOHu1VKBx_PTcWnZ-cbOAC',
          },
          {
              'package': 'chromium/third_party/android_sdk/public/cmdline-tools',
              'version': 'gekOVsZjseS1w9BXAT3FsoW__ByGDJYS9DgqesiwKYoC',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },

  'src/third_party/icu4j/cipd': {
      'packages': [
          {
              'package': 'chromium/third_party/icu4j',
              'version': '8dV7WRVX0tTaNNqkLEnCA_dMofr2MJXFK400E7gOFygC',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },

  'src/third_party/robolectric/cipd': {
      'packages': [
          {
              'package': 'chromium/third_party/robolectric',
              'version': 'G3VkWqTv1YWDvC6zCrL34iQREzrzdBmSL4GMboAIiAEC',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },

  'src/third_party/sqlite4java/cipd': {
      'packages': [
          {
              'package': 'chromium/third_party/sqlite4java',
              'version': 'LofjKH9dgXIAJhRYCPQlMFywSwxYimrfDeBmaHc-Z5EC',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },

  'src/third_party/turbine/cipd': {
      'packages': [
          {
              'package': 'chromium/third_party/turbine',
              'version': 'scfGptWnO9bwzbg-jr0mcnVO3NG5KQJvlAQd_JSD5QUC',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },

  'src/third_party/zstd/src': {
    'url': Var('chromium_git') + '/external/github.com/facebook/zstd.git' + '@' + 'd654fca78690fa15cceb8058ac47454d914a0e63',
    'condition': 'checkout_android',
  },

  'src/tools/luci-go': {
      'packages': [
        {
          'package': 'infra/tools/luci/cas/${{platform}}',
          'version': 'git_revision:a9a10995e2889126ace91faf4052949e38c69d33',
        },
        {
          'package': 'infra/tools/luci/isolate/${{platform}}',
          'version': 'git_revision:a9a10995e2889126ace91faf4052949e38c69d33',
        },
        {
          'package': 'infra/tools/luci/swarming/${{platform}}',
          'version': 'git_revision:a9a10995e2889126ace91faf4052949e38c69d33',
        }
      ],
      'dep_type': 'cipd',
  },

  'src/third_party/pipewire/linux-amd64': {
    'packages': [
      {
        'package': 'chromium/third_party/pipewire/linux-amd64',
        'version': 'BaVKmAmwpjdS6O0pnjSaMNSKhO1nmk5mRnyPVAJ2-HEC',
      },
      {
        'package': 'chromium/third_party/pipewire-media-session/linux-amd64',
        'version': 'Y6wUeITvAA0QD1vt8_a7eQdzbp0gkI1B02qfZUMJdowC',
      },
    ],

    'condition': 'checkout_linux',
    'dep_type': 'cipd',
  },

  'src/third_party/android_deps/autorolled/cipd': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/autorolled',
              'version': 'TZHdcfZjA4uMdOqeaQ-7MG_Pi3P1vgW-fN5TrDzqfVsC',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  # Everything coming after this is automatically updated by the auto-roller.
  # === ANDROID_DEPS Generated Code Start ===
  # Generated by //third_party/android_deps/fetch_all.py
  'src/third_party/android_deps/cipd/libs/com_android_support_support_annotations': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_android_support_support_annotations',
              'version': 'version:2@28.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_android_tools_common': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_android_tools_common',
              'version': 'version:2@30.2.0-beta01.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_android_tools_layoutlib_layoutlib_api': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_android_tools_layoutlib_layoutlib_api',
              'version': 'version:2@30.2.0-beta01.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_android_tools_sdk_common': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_android_tools_sdk_common',
              'version': 'version:2@30.2.0-beta01.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_apps_common_testing_accessibility_framework_accessibility_test_framework': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_apps_common_testing_accessibility_framework_accessibility_test_framework',
              'version': 'version:2@4.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_datatransport_transport_api': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_datatransport_transport_api',
              'version': 'version:2@4.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_datatransport_transport_backend_cct': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_datatransport_transport_backend_cct',
              'version': 'version:2@4.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_datatransport_transport_runtime': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_datatransport_transport_runtime',
              'version': 'version:2@4.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_auth': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_auth',
              'version': 'version:2@21.3.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_auth_api_phone': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_auth_api_phone',
              'version': 'version:2@18.1.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_auth_base': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_auth_base',
              'version': 'version:2@18.1.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_auth_blockstore': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_auth_blockstore',
              'version': 'version:2@16.4.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_base': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_base',
              'version': 'version:2@18.5.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_basement': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_basement',
              'version': 'version:2@18.5.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_cast': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_cast',
              'version': 'version:2@22.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_cast_framework': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_cast_framework',
              'version': 'version:2@22.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_clearcut': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_clearcut',
              'version': 'version:2@17.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_cloud_messaging': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_cloud_messaging',
              'version': 'version:2@17.2.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_fido': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_fido',
              'version': 'version:2@21.1.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_flags': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_flags',
              'version': 'version:2@18.1.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_gcm': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_gcm',
              'version': 'version:2@17.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_identity_credentials': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_identity_credentials',
              'version': 'version:2@16.0.0-alpha05.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_iid': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_iid',
              'version': 'version:2@17.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_instantapps': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_instantapps',
              'version': 'version:2@18.1.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_location': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_location',
              'version': 'version:2@21.3.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_phenotype': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_phenotype',
              'version': 'version:2@17.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_stats': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_stats',
              'version': 'version:2@17.1.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_tasks': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_tasks',
              'version': 'version:2@18.2.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_vision': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_vision',
              'version': 'version:2@20.1.3.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_gms_play_services_vision_common': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_gms_play_services_vision_common',
              'version': 'version:2@19.1.3.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_libraries_identity_googleid_googleid': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_libraries_identity_googleid_googleid',
              'version': 'version:2@1.1.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_material_material': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_material_material',
              'version': 'version:2@1.13.0-alpha05.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_play_core_common': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_play_core_common',
              'version': 'version:2@2.0.3.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_android_play_feature_delivery': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_play_feature_delivery',
              'version': 'version:2@2.1.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_ar_impress': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_ar_impress',
              'version': 'version:2@0.0.2.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_auto_service_auto_service_annotations': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_auto_service_auto_service_annotations',
              'version': 'version:2@1.0-rc6.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_auto_value_auto_value_annotations': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_auto_value_auto_value_annotations',
              'version': 'version:2@1.11.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_code_findbugs_jsr305': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_code_findbugs_jsr305',
              'version': 'version:2@3.0.2.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_code_gson_gson': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_code_gson_gson',
              'version': 'version:2@2.8.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_dagger_dagger': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_dagger_dagger',
              'version': 'version:2@2.52.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_errorprone_error_prone_annotations': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_errorprone_error_prone_annotations',
              'version': 'version:2@2.30.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_firebase_firebase_annotations': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_firebase_firebase_annotations',
              'version': 'version:2@16.2.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_firebase_firebase_common': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_firebase_firebase_common',
              'version': 'version:2@21.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_firebase_firebase_common_ktx': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_firebase_firebase_common_ktx',
              'version': 'version:2@21.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_firebase_firebase_components': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_firebase_firebase_components',
              'version': 'version:2@18.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_firebase_firebase_datatransport': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_firebase_firebase_datatransport',
              'version': 'version:2@19.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_firebase_firebase_encoders': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_firebase_firebase_encoders',
              'version': 'version:2@17.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_firebase_firebase_encoders_json': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_firebase_firebase_encoders_json',
              'version': 'version:2@18.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_firebase_firebase_encoders_proto': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_firebase_firebase_encoders_proto',
              'version': 'version:2@16.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_firebase_firebase_iid': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_firebase_firebase_iid',
              'version': 'version:2@21.1.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_firebase_firebase_iid_interop': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_firebase_firebase_iid_interop',
              'version': 'version:2@17.1.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_firebase_firebase_installations': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_firebase_firebase_installations',
              'version': 'version:2@17.2.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_firebase_firebase_installations_interop': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_firebase_firebase_installations_interop',
              'version': 'version:2@17.1.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_firebase_firebase_measurement_connector': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_firebase_firebase_measurement_connector',
              'version': 'version:2@20.0.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_firebase_firebase_messaging': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_firebase_firebase_messaging',
              'version': 'version:2@24.1.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_guava_failureaccess': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_guava_failureaccess',
              'version': 'version:2@1.0.2.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_guava_guava': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_guava_guava',
              'version': 'version:2@33.3.1-jre.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_guava_guava_android': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_guava_guava_android',
              'version': 'version:2@33.3.1-android.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_j2objc_j2objc_annotations': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_j2objc_j2objc_annotations',
              'version': 'version:2@3.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_protobuf_protobuf_javalite': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_protobuf_protobuf_javalite',
              'version': 'version:2@4.28.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_google_testparameterinjector_test_parameter_injector': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_testparameterinjector_test_parameter_injector',
              'version': 'version:2@1.18.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_googlecode_java_diff_utils_diffutils': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_googlecode_java_diff_utils_diffutils',
              'version': 'version:2@1.3.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_squareup_javapoet': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_squareup_javapoet',
              'version': 'version:2@1.13.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_squareup_moshi_moshi': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_squareup_moshi_moshi',
              'version': 'version:2@1.15.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_squareup_moshi_moshi_adapters': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_squareup_moshi_moshi_adapters',
              'version': 'version:2@1.15.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_squareup_okio_okio_jvm': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_squareup_okio_okio_jvm',
              'version': 'version:2@3.9.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/com_squareup_wire_wire_runtime_jvm': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_squareup_wire_wire_runtime_jvm',
              'version': 'version:2@5.2.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/jakarta_inject_jakarta_inject_api': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/jakarta_inject_jakarta_inject_api',
              'version': 'version:2@2.0.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/javax_annotation_javax_annotation_api': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/javax_annotation_javax_annotation_api',
              'version': 'version:2@1.3.2.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/javax_inject_javax_inject': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/javax_inject_javax_inject',
              'version': 'version:2@1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/net_bytebuddy_byte_buddy': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/net_bytebuddy_byte_buddy',
              'version': 'version:2@1.14.12.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/net_bytebuddy_byte_buddy_agent': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/net_bytebuddy_byte_buddy_agent',
              'version': 'version:2@1.14.12.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_bouncycastle_bcprov_jdk18on': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_bouncycastle_bcprov_jdk18on',
              'version': 'version:2@1.78.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_ccil_cowan_tagsoup_tagsoup': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_ccil_cowan_tagsoup_tagsoup',
              'version': 'version:2@1.2.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_checkerframework_checker_compat_qual': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_checkerframework_checker_compat_qual',
              'version': 'version:2@2.5.5.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_checkerframework_checker_qual': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_checkerframework_checker_qual',
              'version': 'version:2@3.43.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_checkerframework_checker_util': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_checkerframework_checker_util',
              'version': 'version:2@3.25.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_codehaus_mojo_animal_sniffer_annotations': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_codehaus_mojo_animal_sniffer_annotations',
              'version': 'version:2@1.17.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_conscrypt_conscrypt_openjdk_uber': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_conscrypt_conscrypt_openjdk_uber',
              'version': 'version:2@2.5.2.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_jetbrains_kotlin_kotlin_android_extensions_runtime': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_jetbrains_kotlin_kotlin_android_extensions_runtime',
              'version': 'version:2@1.9.22.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_jetbrains_kotlin_kotlin_parcelize_runtime': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_jetbrains_kotlin_kotlin_parcelize_runtime',
              'version': 'version:2@1.9.22.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_jetbrains_kotlinx_atomicfu_jvm': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_jetbrains_kotlinx_atomicfu_jvm',
              'version': 'version:2@0.23.2.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_jetbrains_kotlinx_kotlinx_coroutines_android': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_jetbrains_kotlinx_kotlinx_coroutines_android',
              'version': 'version:2@1.8.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_jetbrains_kotlinx_kotlinx_coroutines_core_jvm': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_jetbrains_kotlinx_kotlinx_coroutines_core_jvm',
              'version': 'version:2@1.10.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_jetbrains_kotlinx_kotlinx_coroutines_guava': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_jetbrains_kotlinx_kotlinx_coroutines_guava',
              'version': 'version:2@1.8.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_jetbrains_kotlinx_kotlinx_coroutines_play_services': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_jetbrains_kotlinx_kotlinx_coroutines_play_services',
              'version': 'version:2@1.10.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_jetbrains_kotlinx_kotlinx_coroutines_test_jvm': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_jetbrains_kotlinx_kotlinx_coroutines_test_jvm',
              'version': 'version:2@1.7.3.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_jetbrains_kotlinx_kotlinx_serialization_core_jvm': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_jetbrains_kotlinx_kotlinx_serialization_core_jvm',
              'version': 'version:2@1.7.2.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_jsoup_jsoup': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_jsoup_jsoup',
              'version': 'version:2@1.15.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_jspecify_jspecify': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_jspecify_jspecify',
              'version': 'version:2@1.0.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_mockito_mockito_android': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_mockito_mockito_android',
              'version': 'version:2@5.11.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_mockito_mockito_core': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_mockito_mockito_core',
              'version': 'version:2@5.11.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_mockito_mockito_subclass': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_mockito_mockito_subclass',
              'version': 'version:2@5.11.0.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_objenesis_objenesis': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_objenesis_objenesis',
              'version': 'version:2@3.3.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_ow2_asm_asm': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_ow2_asm_asm',
              'version': 'version:2@9.7.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_ow2_asm_asm_analysis': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_ow2_asm_asm_analysis',
              'version': 'version:2@9.7.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_ow2_asm_asm_commons': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_ow2_asm_asm_commons',
              'version': 'version:2@9.7.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_ow2_asm_asm_tree': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_ow2_asm_asm_tree',
              'version': 'version:2@9.7.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_ow2_asm_asm_util': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_ow2_asm_asm_util',
              'version': 'version:2@9.7.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_robolectric_annotations': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_robolectric_annotations',
              'version': 'version:2@4.14.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_robolectric_junit': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_robolectric_junit',
              'version': 'version:2@4.14.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_robolectric_nativeruntime': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_robolectric_nativeruntime',
              'version': 'version:2@4.14.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_robolectric_nativeruntime_dist_compat': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_robolectric_nativeruntime_dist_compat',
              'version': 'version:2@1.0.16.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_robolectric_pluginapi': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_robolectric_pluginapi',
              'version': 'version:2@4.14.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_robolectric_plugins_maven_dependency_resolver': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_robolectric_plugins_maven_dependency_resolver',
              'version': 'version:2@4.14.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_robolectric_resources': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_robolectric_resources',
              'version': 'version:2@4.14.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_robolectric_robolectric': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_robolectric_robolectric',
              'version': 'version:2@4.14.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_robolectric_sandbox': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_robolectric_sandbox',
              'version': 'version:2@4.14.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_robolectric_shadowapi': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_robolectric_shadowapi',
              'version': 'version:2@4.14.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_robolectric_shadows_framework': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_robolectric_shadows_framework',
              'version': 'version:2@4.14.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_robolectric_utils': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_robolectric_utils',
              'version': 'version:2@4.14.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_robolectric_utils_reflector': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_robolectric_utils_reflector',
              'version': 'version:2@4.14.1.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  'src/third_party/android_deps/cipd/libs/org_yaml_snakeyaml': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/org_yaml_snakeyaml',
              'version': 'version:2@2.3.cr1',
          },
      ],
      'condition': 'checkout_android and non_git_source',
      'dep_type': 'cipd',
  },

  # === ANDROID_DEPS Generated Code End ===
}

hooks = [
  {
    # This clobbers when necessary (based on get_landmines.py). It should be
    # an early hook but it will need to be run after syncing Chromium and
    # setting up the links, so the script actually exists.
    'name': 'landmines',
    'pattern': '.',
    'action': [
        'python3',
        'src/build/landmines.py',
        '--landmine-scripts',
        'src/tools_webrtc/get_landmines.py',
        '--src-dir',
        'src',
    ],
  },
  {
    # Ensure that the DEPS'd "depot_tools" has its self-update capability
    # disabled.
    'name': 'disable_depot_tools_selfupdate',
    'pattern': '.',
    'action': [
        'python3',
        'src/third_party/depot_tools/update_depot_tools_toggle.py',
        '--disable',
    ],
  },
  {
    'name': 'sysroot_arm',
    'pattern': '.',
    'condition': 'checkout_linux and checkout_arm',
    'action': ['python3', 'src/build/linux/sysroot_scripts/install-sysroot.py',
               '--arch=arm'],
  },
  {
    'name': 'sysroot_arm64',
    'pattern': '.',
    'condition': 'checkout_linux and checkout_arm64',
    'action': ['python3', 'src/build/linux/sysroot_scripts/install-sysroot.py',
               '--arch=arm64'],
  },
  {
    'name': 'sysroot_x86',
    'pattern': '.',
    'condition': 'checkout_linux and (checkout_x86 or checkout_x64)',
    # TODO(mbonadei): change to --arch=x86.
    'action': ['python3', 'src/build/linux/sysroot_scripts/install-sysroot.py',
               '--arch=i386'],
  },
  {
    'name': 'sysroot_mips',
    'pattern': '.',
    'condition': 'checkout_linux and checkout_mips',
    # TODO(mbonadei): change to --arch=mips.
    'action': ['python3', 'src/build/linux/sysroot_scripts/install-sysroot.py',
               '--arch=mipsel'],
  },
  {
    'name': 'sysroot_x64',
    'pattern': '.',
    'condition': 'checkout_linux and checkout_x64',
    # TODO(mbonadei): change to --arch=x64.
    'action': ['python3', 'src/build/linux/sysroot_scripts/install-sysroot.py',
               '--arch=amd64'],
  },
  {
    # Case-insensitivity for the Win SDK. Must run before win_toolchain below.
    'name': 'ciopfs_linux',
    'pattern': '.',
    'condition': 'checkout_win and host_os == "linux"',
    'action': [ 'python3',
                'src/third_party/depot_tools/download_from_google_storage.py',
                '--no_resume',
                '--no_auth',
                '--bucket', 'chromium-browser-clang/ciopfs',
                '-s', 'src/build/ciopfs.sha1',
    ]
  },
  {
    # Update the Windows toolchain if necessary. Must run before 'clang' below.
    'name': 'win_toolchain',
    'pattern': '.',
    'condition': 'checkout_win',
    'action': ['python3', 'src/build/vs_toolchain.py', 'update', '--force'],
  },
  {
    # Update the Mac toolchain if necessary.
    'name': 'mac_toolchain',
    'pattern': '.',
    'condition': 'checkout_mac',
    'action': ['python3', 'src/build/mac_toolchain.py'],
  },

  {
    'name': 'Download Fuchsia SDK from GCS',
    'pattern': '.',
    'condition': 'checkout_fuchsia',
    'action': [
      'python3',
      'src/build/fuchsia/update_sdk.py',
      '--cipd-prefix={fuchsia_sdk_cipd_prefix}',
      '--version={fuchsia_version}',
    ],
  },
  {
    'name': 'Download Fuchsia system images',
    'pattern': '.',
    'condition': 'checkout_fuchsia and checkout_fuchsia_product_bundles',
    'action': [
      'python3',
      'src/build/fuchsia/update_product_bundles.py',
      '{checkout_fuchsia_boot_images}',
    ],
  },
  {
    # Note: On Win, this should run after win_toolchain, as it may use it.
    'name': 'clang',
    'pattern': '.',
    'action': ['python3', 'src/tools/clang/scripts/update.py'],
  },
  {
    # This is supposed to support the same set of platforms as 'clang' above.
    'name': 'clang_coverage',
    'pattern': '.',
    'condition': 'checkout_clang_coverage_tools',
    'action': ['python3', 'src/tools/clang/scripts/update.py',
               '--package=coverage_tools'],
  },
  {
    # This is also supposed to support the same set of platforms as 'clang'
    # above. LLVM ToT support isn't provided at the moment.
    'name': 'clangd',
    'pattern': '.',
    'condition': 'checkout_clangd',
    'action': ['vpython3', 'src/tools/clang/scripts/update.py',
               '--package=clangd'],
  },
  {
    'name': 'rust',
    'pattern': '.',
    'action': ['python3', 'src/tools/rust/update_rust.py'],
  },
  {
    # Update LASTCHANGE.
    'name': 'lastchange',
    'pattern': '.',
    'action': ['python3', 'src/build/util/lastchange.py',
               '-o', 'src/build/util/LASTCHANGE'],
  },
  # Pull dsymutil binaries using checked-in hashes.
  {
    'name': 'dsymutil_mac_arm64',
    'pattern': '.',
    'condition': 'host_os == "mac" and host_cpu == "arm64"',
    'action': [ 'python3',
                'src/third_party/depot_tools/download_from_google_storage.py',
                '--no_resume',
                '--no_auth',
                '--bucket', 'chromium-browser-clang',
                '-s', 'src/tools/clang/dsymutil/bin/dsymutil.arm64.sha1',
                '-o', 'src/tools/clang/dsymutil/bin/dsymutil',
    ],
  },
  {
    'name': 'dsymutil_mac_x64',
    'pattern': '.',
    'condition': 'host_os == "mac" and host_cpu == "x64"',
    'action': [ 'python3',
                'src/third_party/depot_tools/download_from_google_storage.py',
                '--no_resume',
                '--no_auth',
                '--bucket', 'chromium-browser-clang',
                '-s', 'src/tools/clang/dsymutil/bin/dsymutil.x64.sha1',
                '-o', 'src/tools/clang/dsymutil/bin/dsymutil',
    ],
  },
  # Pull rc binaries using checked-in hashes.
  {
    'name': 'rc_win',
    'pattern': '.',
    'condition': 'checkout_win and host_os == "win"',
    'action': [ 'python3',
                'src/third_party/depot_tools/download_from_google_storage.py',
                '--no_resume',
                '--no_auth',
                '--bucket', 'chromium-browser-clang/rc',
                '-s', 'src/build/toolchain/win/rc/win/rc.exe.sha1',
    ],
  },
  {
    'name': 'rc_mac',
    'pattern': '.',
    'condition': 'checkout_win and host_os == "mac"',
    'action': [ 'python3',
                'src/third_party/depot_tools/download_from_google_storage.py',
                '--no_resume',
                '--no_auth',
                '--bucket', 'chromium-browser-clang/rc',
                '-s', 'src/build/toolchain/win/rc/mac/rc.sha1',
    ],
  },
  {
    'name': 'rc_linux',
    'pattern': '.',
    'condition': 'checkout_win and host_os == "linux"',
    'action': [ 'python3',
                'src/third_party/depot_tools/download_from_google_storage.py',
                '--no_resume',
                '--no_auth',
                '--bucket', 'chromium-browser-clang/rc',
                '-s', 'src/build/toolchain/win/rc/linux64/rc.sha1',
    ],
  },
  {
    # Download test resources, i.e. video and audio files from Google Storage.
    'pattern': '.',
    'action': ['download_from_google_storage',
               '--directory',
               '--recursive',
               '--num_threads=10',
               '--no_auth',
               '--quiet',
               '--bucket', 'chromium-webrtc-resources',
               'src/resources'],
  },
  {
    'name': 'Generate component metadata for tests',
    'pattern': '.',
    'action': [
      'vpython3',
      'src/testing/generate_location_tags.py',
      '--out',
      'src/testing/location_tags.json',
    ],
  },
  # Download and initialize "vpython" VirtualEnv environment packages.
  {
    'name': 'vpython_common',
    'pattern': '.',
    'action': [ 'vpython3',
                '-vpython-spec', 'src/.vpython3',
                '-vpython-tool', 'install',
    ],
  },
  # Download remote exec cfg files
  {
    'name': 'configure_reclient_cfgs',
    'pattern': '.',
    'condition': 'download_remoteexec_cfg',
    'action': ['python3',
               'src/buildtools/reclient_cfgs/configure_reclient_cfgs.py',
               '--rbe_instance',
               Var('rbe_instance'),
               '--reproxy_cfg_template',
               'reproxy.cfg.template',
               '--quiet',
               ],
  },
  # Configure Siso for developer builds.
  {
    'name': 'configure_siso',
    'pattern': '.',
    'action': ['python3',
               'src/build/config/siso/configure_siso.py',
               '--rbe_instance',
               Var('rbe_instance'),
               ],
  },
  {
    # Ensure we remove any file from disk that is no longer needed (e.g. after
    # hooks to native GCS deps migration).
    'name': 'remove_stale_files',
    'pattern': '.',
    'action': [
        'python3',
        'src/tools/remove_stale_files.py',
        'src/third_party/test_fonts/test_fonts.tar.gz', # Remove after 20240901
    ],
  },
]

recursedeps = [
  'src/buildtools',
  'src/third_party/instrumented_libs',
]

# Define rules for which include paths are allowed in our source.
include_rules = [
  # Base is only used to build Android APK tests and may not be referenced by
  # WebRTC production code.
  "-base",
  "-chromium",
  "+external/webrtc/webrtc",  # Android platform build.
  "+libyuv",

  # These should eventually move out of here.
  "+common_types.h",

  "+WebRTC",
  "+api",
  "+modules/include",
  "+rtc_base",
  "+test",
  "+rtc_tools",

  # Abseil allowlist. Keep this in sync with abseil-in-webrtc.md.
  "+absl/algorithm/algorithm.h",
  "+absl/algorithm/container.h",
  "+absl/base/attributes.h",
  "+absl/base/config.h",
  "+absl/base/nullability.h",
  "+absl/base/macros.h",
  "+absl/cleanup/cleanup.h",
  "+absl/container",
  "-absl/container/fixed_array.h",
  "+absl/functional/any_invocable.h",
  "+absl/functional/bind_front.h",
  "+absl/memory/memory.h",
  "+absl/numeric/bits.h",
  "+absl/strings/ascii.h",
  "+absl/strings/escaping.h",
  "+absl/strings/match.h",
  "+absl/strings/str_cat.h",  # note - allowed for single argument version only
  "+absl/strings/str_replace.h",
  "+absl/strings/string_view.h",

  # Abseil flags are allowed in tests and tools.
  "+absl/flags",

  # Perfetto should be used through rtc_base/trace_event.h
  '-third_party/perfetto',
  '-perfetto',
  '-protos/perfetto',
]

specific_include_rules = {
  "webrtc_lib_link_test\.cc": [
    "+media/engine",
    "+modules/audio_device",
    "+modules/audio_processing",
  ]
}
