#include "Blockchain.hpp"
#include "utils.h"
#include <cstddef>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

using namespace blockchain;
using namespace common::security;

TEST(Blockchain, blockchain) {
  constexpr size_t SIZE = 4;
  Blockchain blockchain = Blockchain();

  for (std::size_t i = 0; i < SIZE; i++) {
    std::ostringstream oss;
    oss << "Block " << i << " Data";
    blockchain.insert(oss.str());
  }

  EXPECT_EQ(blockchain.size(), SIZE + 1);
  EXPECT_TRUE(blockchain.is_valid());
}

TEST(Blockchain, blockchain_serialize) {
  constexpr size_t SIZE = 4;
  Blockchain blockchain = Blockchain();

  for (std::size_t i = 0; i < SIZE; i++) {
    std::ostringstream oss;
    oss << "Block " << i << " Data";
    blockchain.insert(oss.str());
  }
  {
    std::ostringstream oss;
    blockchain.serialize(oss);
    std::string str = oss.str();
    std::vector<uint8_t> actual_data{};
    std::copy(str.begin(), str.end(), std::back_inserter(actual_data));
  }
}

TEST(Blockchain, blockchain_deserialize) {
  constexpr size_t SIZE = 4;
  Blockchain expected_blockchain = Blockchain();

  for (std::size_t i = 0; i < SIZE; i++) {
    std::ostringstream oss;
    oss << "Block " << i << " Data";
    expected_blockchain.insert(oss.str());
  }

  std::vector<uint8_t> expected_data{};
  {
    std::ostringstream oss;
    expected_blockchain.serialize(oss);
    std::string str = oss.str();
    std::copy(str.begin(), str.end(), std::back_inserter(expected_data));
    EXPECT_EQ(expected_data.size(),
              (                             // *blockchain
                  4 +                       //   version
                  4 +                       //   difficulty
                  (                         //   *block
                      4 +                   //     size
                      4 +                   //     version
                      4 +                   //     id
                      8 +                   //     nonce
                      8 +                   //     time
                      Sha256::DIGEST_SIZE + //     prev_hash
                      13                    //     data
                      ) *
                      (SIZE + 1)));
  }

  Blockchain actual_blockchain = Blockchain();
  {
    imemstream is(reinterpret_cast<const char *>(expected_data.data()),
                  expected_data.size());
    actual_blockchain.deserialize(is);
    EXPECT_EQ(actual_blockchain, expected_blockchain);
  }
}
