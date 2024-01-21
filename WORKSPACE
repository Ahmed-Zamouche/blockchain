workspace(name = "blockchain")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
  name = "gtest",
  urls = ["https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz"],
  strip_prefix = "googletest-release-1.12.1",
  sha256 = "81964fe578e9bd7c94dfdb09c8e4d6e6759e19967e397dbea48d1c10e45d0df2",
  build_file = "@//third-party/gtest:BUILD",
)

http_archive(
  name = "spdlog",
  urls = ["https://github.com/gabime/spdlog/archive/refs/tags/v1.13.0.tar.gz"],
  strip_prefix = "spdlog-1.13.0",
  sha256 = "534f2ee1a4dcbeb22249856edfb2be76a1cf4f708a20b0ac2ed090ee24cfdbc9",
  build_file = "@//third-party/spdlog:BUILD",
)


# http_archive(
#     name = "tink_cc",
#     urls = ["https://github.com/tink-crypto/tink-cc/archive/refs/tags/v2.1.1.zip"],
#     strip_prefix = "tink-2.1.1",
#     sha256 = "3080600b6c38421ebaca5bfc460aa965afc88c877695c080019a8905f0f1c1b8",
# )
# load("@tink_cc//:tink_cc_deps.bzl", "tink_cc_deps")
# tink_cc_deps()
# load("@tink_cc//:tink_cc_deps_init.bzl", "tink_cc_deps_init")
#
# tink_cc_deps_init()

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
http_archive(
    name = "hedron_compile_commands",

    # Replace the commit hash in both places (below) with the latest, rather than using the stale one here.
    # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/ed994039a951b736091776d677f324b3903ef939.tar.gz",
    strip_prefix = "bazel-compile-commands-extractor-ed994039a951b736091776d677f324b3903ef939",
    # When you first run this tool, it'll recommend a sha256 hash to put here with a message like: "DEBUG: Rule 'hedron_compile_commands' indicated that a canonical reproducible form can be obtained by modifying arguments sha256 = ..."
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()
