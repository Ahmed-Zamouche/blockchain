load("@rules_cc//cc:defs.bzl", "cc_test", "cc_library")

cc_library(
  name = "sha256_win",
  hdrs = [],
  srcs = ["sha256_win.cpp"],
  deps = ["//common/include/security:sha256"],
  target_compatible_with = ["@platforms//os:windows"],
  visibility = ["//visibility:private"],
)

cc_library(
  name = "sha256_linux",
  hdrs = [],
  srcs = [],
  deps = ["//common/include/security:sha256"],
  linkopts = ["-lssl", "-lcrypto"],
  target_compatible_with = ["@platforms//os:linux"],
  visibility = ["//visibility:private"],
)

cc_library(
  name = "sha256",
  srcs = ["sha256.cpp"],
  deps = select({
    "@platforms//os:linux" : [":sha256_linux"],
    "@platforms//os:windows" : [":sha256_win"],
    "//conditions:default": ["@platforms//:incompatible"],
  }),

  visibility = ["//visibility:public"],
)

cc_test(
  name = "test_sha256",
  srcs = ["test_sha256.cpp"],
  deps = [
    ":sha256",
    "@gtest//:main",
  ],
)
