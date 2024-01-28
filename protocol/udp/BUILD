load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary")


cc_binary(
  name = "sender",
  srcs = ["sender.cpp"],
  deps = [
    "//common:utils",
    "@spdlog//:spdlog",
  ],
)

cc_binary(
  name = "listener",
  srcs = ["listener.cpp"],
  deps = [
    "//common:utils",
    "@spdlog//:spdlog"
  ],
)

