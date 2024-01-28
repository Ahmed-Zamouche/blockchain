load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary")


cc_binary(
  name = "server",
  srcs = ["server.cpp"],
  deps = [
    "//common:utils",
    "@spdlog//:spdlog",
  ],
)

cc_binary(
  name = "client",
  srcs = ["client.cpp"],
  deps = [
    "//common:utils",
    "@spdlog//:spdlog"
  ],
)

