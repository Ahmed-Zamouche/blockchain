load("@rules_cc//cc:defs.bzl", "cc_test", "cc_library")

cc_library
cc_library(
  name = "sha256_win",
  hdrs = [ "sha256_win.hpp"],
  srcs = ["sha256_win.cpp"],
  target_compatible_with = ["@platforms//os:windows"],
  visibility = ["//visibility:private"],
)

cc_library(
  name = "sha256_linux",
  hdrs = [ "sha256_linux.hpp"],
  srcs = ["sha256_linux.cpp"],
  linkopts = ["-lssl", "-lcrypto"],
  target_compatible_with = ["@platforms//os:linux"],
  visibility = ["//visibility:private"],
)

cc_library(
  name = "sha256",
  hdrs = [ "sha256.h"],
  srcs = ["sha256.cpp"],
  deps = select({
    "@platforms//os:linux" : ["sha256_linux"],
    "@platforms//os:windows" : ["sha256_win"],
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
