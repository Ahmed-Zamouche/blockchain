#include "Blockchain.hpp"
#include "common/net/io.h"
#include <cstdint>
#include <cstring>
#include <memory>
#include <ostream>
#include <sstream>

namespace blockchain {

using namespace security;
using namespace common;

void Blockchain::serialize(std::ostream &os) const {

  static_assert(sizeof(uint32_t) <= sizeof(size_t),
                "sizeof(uint32_t) > sizeof(size_t)");

  net::io::write(os, "version   ", _version);
  net::io::write(os, "difficulty", _difficulty);
  net::io::write(os, "size      ", static_cast<uint32_t>(_bchain.size()));

  for (auto &&block : _bchain) {
    net::io::write(os, "block_size",
                   static_cast<uint32_t>(block->get_data().size()));
    block->serialize(os);
  }
}

void Blockchain::deserialize(std::istream &is) {

  std::remove_const<decltype(_version)>::type version{};
  uint32_t bchain_size{};

  { // version
    net::io::read(is, "version   ", version);
    if (_version != version) {
      throw std::runtime_error{"Blockchain migrate not implemented yet!"};
    }
  }

  net::io::read(is, "difficulty", _difficulty);
  net::io::read(is, "chain_size", bchain_size);

  Block *prev_block{nullptr};
  std::vector<std::unique_ptr<Block>> bchain;
  while (true) {
    uint32_t block_size{};
    try {
      net::io::read(is, "block_size", block_size);
    } catch (const net::io::ios_eof &e) {
      // std::cout << e.what() << std::endl;
      break;
    } catch (const net::io::ios_failure &e) {
      std::cerr << e.what() << std::endl;
      throw e;
    }

    bchain.push_back(std::make_unique<Block>(0, ""));
    auto &block = bchain.back();
    block->set_data_size(block_size);
    block->deserialize(is);
    block->set_prev_block(prev_block);
    if (!block->is_valid()) {
      std::ostringstream oss;
      oss << "Failed to deserialize blockchain. invalid block: " << block;
      throw std::runtime_error{oss.str()};
    }
    prev_block = block.get();
  }
  if (bchain_size != bchain.size()) {
    std::ostringstream oss;
    oss << "Failed to deserialize blockchain. expected " << bchain_size
        << " block(s), actual " << bchain.size() << " block(s)";
    throw std::runtime_error{oss.str()};
  }
  _bchain.swap(bchain);
}

std::string Blockchain::to_json() const {
  std::ostringstream oss;
  oss << "{";
  oss << "\"version\": " << _version;
  oss << "\"difficulty\": " << _difficulty;
  oss << ", \"size\": " << _bchain.size();
  oss << ", \"blocks\": ";
  oss << "[";
  if (_bchain.size()) {
    oss << _bchain[0];
  }
  for (std::size_t i = 1; i < _bchain.size(); i++) {
    oss << ", " << _bchain[i];
  }
  oss << "]";
  oss << "}";
  return oss.str();
}

} // namespace blockchain
