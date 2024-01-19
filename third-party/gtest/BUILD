load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "main",
    srcs = glob(
        ["googletest/src/*.cc"],
        exclude = ["googletest/src/gtest-all.cc"]
    ),
    hdrs = glob([
        "googletest/include/**/*.h",
        "googletest/src/*.h"
    ]),
    includes = ["googletest/include"],
    copts = [
        "-Iexternal/gtest/googletest/include",
        "-Iexternal/gtest/googletest",
    ],
    linkopts = select({
      "@platforms//os:linux":  ["-pthread"],
      "@platforms//os:windows":  [],
      "//conditions:default": [],
    }),
    visibility = ["//visibility:public"],
)
