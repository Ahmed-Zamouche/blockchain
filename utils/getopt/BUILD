load("@rules_cc//cc:defs.bzl", "cc_test", "cc_library")

cc_library(
  name = "getopt_impl",
  hdrs = ["getopt_impl.h"],
  srcs = ["getopt_impl.c"],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "test_getopt_impl",
  srcs = ["test_getopt_impl.cpp"],
  copts = ["-Iexternal/gtest/googletest/include"],
  deps = [
    ":getopt_impl",
    "@gtest//:main",
  ],
)
