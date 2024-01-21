load("@rules_cc//cc:defs.bzl", "cc_test", "cc_library")

cc_library(
  name = "utils",
  srcs = ["utils.cpp"],
  deps = [
    "//common/include:utils",
  ],
  visibility = ["//visibility:public"],
)


cc_test(
  name = "test_utils",
  srcs = ["test_utils.cpp"],
  deps = [
    ":utils",
    "@gtest//:main"
  ],
)
