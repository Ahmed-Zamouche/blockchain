#include "Block.hpp"
#include "common/security/sha256.h"
#include "common/utils.h"
#include <array>
#include <chrono>
#include <functional>
#include <gtest/gtest.h>
#include <ratio>
#include <type_traits>
#include <vector>

using namespace blockchain;
using namespace blockchain::security;

template <typename T,
          typename std::enable_if_t<std::is_fundamental<T>::value, bool> = true>
void insert(std::vector<uint8_t> &vec, const T value) {
  vec.insert(vec.end(), reinterpret_cast<const uint8_t *>(&value),
             reinterpret_cast<const uint8_t *>(&value) + sizeof(T));
}

template <typename C,
          typename std::enable_if_t<is_container<C>::value, bool> = true>
void insert(std::vector<uint8_t> &vec, const C &c) {
  vec.insert(vec.end(), c.begin(), c.end());
}

void insert(std::vector<uint8_t> &vec, const uint8_t *arr, size_t size) {
  vec.insert(vec.end(), arr, arr + size);
}

TEST(Block, DISABLED_block_duration) {
  Block block{
      0, {'G', 'e', 'n', 'e', 's', 'i', 's', ' ', 'B', 'l', 'o', 'c', 'k'}};
  std::array<uint8_t, SHA256::DIGEST_SIZE> hash = {
      0x00, 0x00, 0x00, 0xdc, 0x66, 0x99, 0x64, 0xa0, 0x96, 0xe8, 0x14,
      0x56, 0x1e, 0x35, 0x75, 0xbc, 0x8c, 0x21, 0xc2, 0xcf, 0x27, 0x9b,
      0xa9, 0x09, 0x09, 0xba, 0xc0, 0x30, 0xe2, 0x8e, 0xd3, 0x20};
  {
    block.set_id(0);
    block.set_time(0);
    block.set_nonce(0);
    using namespace std::placeholders;

    auto nonce0 = block.get_nonce();
    auto callback = std::bind(&Block::mine, &block, _1);
    auto actual_duration = utils::run_duration<std::micro>(callback, 3);
    auto nonce1 = block.get_nonce();

    using namespace std::chrono;
    double expected_duration = 1.0f;

    EXPECT_LT(actual_duration.count() / (nonce1 - nonce0), expected_duration);
#if 0
    std::cout << "actual_duration  : "
              << actual_duration.count() / (nonce1 - nonce0) << "us"
              << std::endl;
    std::cout << "expected_duration: " << expected_duration << "us"
              << std::endl;
#endif
    EXPECT_EQ(std::memcmp(block.get_hash().data(), hash.data(), hash.size()),
              0);
  }
}

TEST(Block, block_mine) {
  Block block{
      0, {'G', 'e', 'n', 'e', 's', 'i', 's', ' ', 'B', 'l', 'o', 'c', 'k'}};
  std::array<uint8_t, SHA256::DIGEST_SIZE> hash = {
      0x00, 0xf3, 0x95, 0x6d, 0xba, 0xe1, 0xe0, 0x42, 0x34, 0x5c, 0xc9,
      0x99, 0x9c, 0x60, 0xbb, 0x07, 0x20, 0xf7, 0xb3, 0xf5, 0xec, 0x62,
      0xc1, 0x7a, 0xc7, 0xdf, 0x81, 0xb1, 0xfe, 0x82, 0x0a, 0xa8};
  {
    block.set_id(0);
    block.set_time(0);
    block.set_nonce(0);
    block.mine(1);
    EXPECT_EQ(std::memcmp(block.get_hash().data(), hash.data(), hash.size()),
              0);
  }
  std::vector<uint8_t> vec;
  {
    uint32_t id{};
    uint64_t nonce{160};
    int64_t time_{};

    std::array<uint8_t, 13> data = {'G', 'e', 'n', 'e', 's', 'i', 's',
                                    ' ', 'B', 'l', 'o', 'c', 'k'};
    insert(vec, id);
    insert(vec, nonce);
    insert(vec, time_);
    insert(vec, ZEROS, sizeof(ZEROS));
    insert(vec, data);
    {
      std::array<uint8_t, SHA256::DIGEST_SIZE> digest{};
      SHA256 ctx = SHA256();
      ctx.init();
      ctx.update(vec.data(), vec.size());
      ctx.final(digest.data());
      EXPECT_EQ(
          std::memcmp(block.get_hash().data(), digest.data(), digest.size()),
          0);
    }
  }
}

TEST(Block, block_is_valid) {
  Block block{
      0, {'G', 'e', 'n', 'e', 's', 'i', 's', ' ', 'B', 'l', 'o', 'c', 'k'}};
  {
    block.set_id(0);
    block.set_time(0);
    block.set_nonce(0);
    block.mine(1);
  }

  EXPECT_TRUE(block.is_valid());
  {
    block.set_id(1);
    EXPECT_FALSE(block.is_valid());
    block.set_id(0);
    EXPECT_TRUE(block.is_valid());
  }

  {
    block.set_time(1);
    EXPECT_FALSE(block.is_valid());
    block.set_time(0);
    EXPECT_TRUE(block.is_valid());
  }

  {
    auto nonce = block.get_nonce();
    block.set_nonce(1);
    EXPECT_FALSE(block.is_valid());
    block.set_nonce(nonce);
    EXPECT_TRUE(block.is_valid());
  }

  {
    auto data = block.get_data();
    block.set_data("Hello World!");
    EXPECT_FALSE(block.is_valid());
    block.set_data(data);
    EXPECT_TRUE(block.is_valid());
  }

  {
    Block prev_block{
        1, {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'}};
    prev_block.mine(1);
    block.set_prev_block(&prev_block);
    EXPECT_FALSE(block.is_valid());
    block.set_prev_block(nullptr);
    EXPECT_TRUE(block.is_valid());
  }

  {
    auto hash = block.get_hash();
    block.set_hash(ZEROS, SHA256::DIGEST_SIZE);
    EXPECT_FALSE(block.is_valid());
    block.set_hash(std::move(hash));
    EXPECT_TRUE(block.is_valid());
  }
}

TEST(Block, block_serialize) {
  Block block{
      0, {'G', 'e', 'n', 'e', 's', 'i', 's', ' ', 'B', 'l', 'o', 'c', 'k'}};
  block.set_id(0x01234567);
  block.set_nonce(0x0123456789abcdef);
  block.set_time(0x0123456789abcdef);
  block.mine(1);

  std::ostringstream oss;
  block.serialize(oss);
  std::string str = oss.str();
  std::vector<uint8_t> actual_data{};
  std::copy(str.begin(), str.end(), std::back_inserter(actual_data));

  std::vector<uint8_t> expected_data = {
      0x00, 0x00, 0x00, 0x01, 0x01, 0x23, 0x45, 0x67, 0x01, 0x23, 0x45, 0x67,
      0x89, 0xab, 0xce, 0x34, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
      0x00, 0xc6, 0x55, 0x71, 0x4b, 0x93, 0x28, 0xc2, 0x5a, 0xf0, 0xf1, 0x68,
      0xc2, 0xbe, 0xcb, 0x57, 0xd6, 0x50, 0x7f, 0x17, 0x3d, 0xaf, 0xe3, 0xbb,
      0xba, 0x9a, 0x5a, 0x14, 0x70, 0xe0, 0x88, 0xe5, 0x47, 0x65, 0x6e, 0x65,
      0x73, 0x69, 0x73, 0x20, 0x42, 0x6c, 0x6f, 0x63, 0x6b};
  EXPECT_EQ(actual_data, expected_data);
}

TEST(Block, block_deserialize) {
  Block expected_block{
      0, {'G', 'e', 'n', 'e', 's', 'i', 's', ' ', 'B', 'l', 'o', 'c', 'k'}};
  expected_block.set_id(0x01234567);
  expected_block.set_nonce(0x0123456789abcdef);
  expected_block.set_time(0x0123456789abcdef);
  expected_block.mine(1);

  Block actual_block{0, {'D', 'u', 'm', 'm', 'y'}};
  std::vector<uint8_t> expected_data = {
      0x00, 0x00, 0x00, 0x01, 0x01, 0x23, 0x45, 0x67, 0x01, 0x23, 0x45, 0x67,
      0x89, 0xab, 0xce, 0x34, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
      0x00, 0xc6, 0x55, 0x71, 0x4b, 0x93, 0x28, 0xc2, 0x5a, 0xf0, 0xf1, 0x68,
      0xc2, 0xbe, 0xcb, 0x57, 0xd6, 0x50, 0x7f, 0x17, 0x3d, 0xaf, 0xe3, 0xbb,
      0xba, 0x9a, 0x5a, 0x14, 0x70, 0xe0, 0x88, 0xe5, 0x47, 0x65, 0x6e, 0x65,
      0x73, 0x69, 0x73, 0x20, 0x42, 0x6c, 0x6f, 0x63, 0x6b};
  imemstream is(reinterpret_cast<const char *>(expected_data.data()),
                expected_data.size());

  actual_block.set_data_size(13);
  actual_block.deserialize(is);
  actual_block.set_prev_block(nullptr);
  actual_block.mine(1);
  EXPECT_EQ(actual_block, expected_block);
}
