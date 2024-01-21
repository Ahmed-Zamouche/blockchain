#include "security/sha256.hpp"
#include <gtest/gtest.h>

using namespace common;

TEST(sha256, checksum) {
  { // EMPTY
    EXPECT_EQ(
        security::sha256(""),
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
  }
  { // NIST.1

    EXPECT_EQ(
        security::sha256("abc"),
        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
  }
  { // NIST.3

    EXPECT_EQ(
        security::sha256(std::string(1000000, 'a')),
        "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0");
  }
}
