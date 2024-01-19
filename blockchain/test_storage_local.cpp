#include "Blockchain.hpp"
#include "StorageLocal.hpp"
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

using namespace blockchain;
using namespace blockchain::storage;
using namespace blockchain::security;

TEST(StorageLocal, storage_save) {
  constexpr size_t SIZE = 4;
  Blockchain blockchain = Blockchain();

  for (std::size_t i = 0; i < SIZE; i++) {
    std::ostringstream oss;
    oss << "Block " << i << " Data";
    blockchain.insert(oss.str());
  }

  std::vector<uint8_t> expected_data{};
  {
    std::ostringstream oss;
    blockchain.serialize(oss);
    std::string str = oss.str();
    std::copy(str.begin(), str.end(), std::back_inserter(expected_data));
  }
  {
    StorageLocal storage{blockchain.front().get_prev_hash_str()};
    storage.save(blockchain);
  }
  {

    std::ifstream ifs{std::filesystem::path{"data"} /
                          blockchain.front().get_prev_hash_str(),
                      std::ifstream::in | std::ifstream::binary};
    ifs.seekg(0, std::ifstream::end);
    size_t size = ifs.tellg();
    ifs.seekg(0, std::ifstream::beg);

    EXPECT_EQ(size, expected_data.size());
    std::vector<uint8_t> actual_data(size);
    ifs.read(reinterpret_cast<char *>(actual_data.data()), size);

    EXPECT_EQ(actual_data, expected_data);
  }
}

TEST(StorageLocal, storage_load) {
  constexpr size_t SIZE = 4;
  Blockchain expected_blockchain = Blockchain();

  for (std::size_t i = 0; i < SIZE; i++) {
    std::ostringstream oss;
    oss << "Block " << i << " Data";
    expected_blockchain.insert(oss.str());
  }

  const std::filesystem::path basename{
      expected_blockchain.front().get_prev_hash_str()};
  StorageLocal storage{basename};

  storage.save(expected_blockchain);

  Blockchain actual_blockchain = Blockchain();
  storage.load(actual_blockchain);

  EXPECT_EQ(expected_blockchain.size(), actual_blockchain.size());
  EXPECT_EQ(expected_blockchain, actual_blockchain);
}
