load("@rules_cc//cc:defs.bzl", "cc_library")


load("@hedron_compile_commands//:refresh_compile_commands.bzl", "refresh_compile_commands")
#CLANGD_FLAGS=--compile-commands-dir=$(pwd)
refresh_compile_commands(
    name = "refresh_compile_commands",
    targets = {
      "//blockchain/...": "--config=linux-x86_64",
    },
)
