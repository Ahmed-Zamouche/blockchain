load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_test", "cc_library")

cc_library(
  name = "base",
  includes = ["."],
)

cc_library(
  name = "interfaces",
  hdrs = [
    "ISerializable.hpp",
    "IStorage.hpp",
  ],
  srcs = [],
)

cc_library(
  name = "block",
  hdrs = ["Block.hpp"],
  srcs = ["Block.cpp"],
  deps = [
    ":base", 
    ":interfaces",
    "//common:utils",
    "//common/include/net:net",
    "//common/include/type_traits:type_traits",
    "//common/security:sha256"
  ],
)

cc_test(
  name = "test_block",
  srcs = ["test_block.cpp"],
  deps = [
    ":block",
    "//common:utils",
    "@gtest//:main",
  ],
)

cc_library(
  name = "blockchain",
  hdrs = ["Blockchain.hpp"],
  srcs = ["Blockchain.cpp"],
  deps = [
    ":base", 
    ":block",
  ],
)

cc_test(
  name = "test_blockchain",
  srcs = ["test_blockchain.cpp"],
  deps = [
    ":blockchain",
    "//common:utils",
    "//common/include/type_traits:type_traits",
    "@gtest//:main",
  ],
)

cc_library(
  name = "storage_local",
  hdrs = [
    "StorageLocal.hpp",
  ],
  srcs = [],
)

cc_test(
  name = "test_storage_local",
  srcs = ["test_storage_local.cpp",],
  deps = [
    "//common:utils",
    "//common/security:sha256",
    ":interfaces",
    ":blockchain",
    ":storage_local",
    "@gtest//:main",
  ],

)
