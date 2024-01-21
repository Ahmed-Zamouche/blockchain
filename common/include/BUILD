load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "top",
  includes = ["."],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "utils",
  hdrs = ["utils.h"],
  deps = [
    ":top",
    "//common/include/type_traits:type_traits"
  ],
  visibility = ["//visibility:public"],
)




