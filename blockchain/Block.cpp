#include "Block.hpp"
#include "common/net/io.h"
#include "common/type_traits/type_traits.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <istream>
#include <sstream>

namespace blockchain {

using namespace security;
using namespace common;

bool Block::is_valid() const {

  SHA256 sha256{};

  sha256.init();
  sha256.update(reinterpret_cast<const uint8_t *>(&_id), sizeof(_id));
  sha256.update(reinterpret_cast<const uint8_t *>(&_nonce), sizeof(_nonce));
  sha256.update(reinterpret_cast<const uint8_t *>(&_time_since_epoch),
                sizeof(_time_since_epoch));
  sha256.update(reinterpret_cast<const uint8_t *>(
                    _prev_block ? _prev_block->get_hash().data() : ZEROS),
                SHA256::DIGEST_SIZE);
  sha256.update(reinterpret_cast<const uint8_t *>(_data.data()), _data.size());

  uint8_t hash[SHA256::DIGEST_SIZE];
  sha256.final(hash);
  return std::memcmp(_hash.data(), hash, SHA256::DIGEST_SIZE) == 0;
}

void Block::calc() {

  SHA256 sha256{};

  sha256.init();
  sha256.update(reinterpret_cast<const uint8_t *>(&_id), sizeof(_id));
  sha256.update(reinterpret_cast<const uint8_t *>(&_nonce), sizeof(_nonce));
  sha256.update(reinterpret_cast<const uint8_t *>(&_time_since_epoch),
                sizeof(_time_since_epoch));
  sha256.update(reinterpret_cast<const uint8_t *>(
                    _prev_block ? _prev_block->get_hash().data() : ZEROS),
                SHA256::DIGEST_SIZE);
  sha256.update(reinterpret_cast<const uint8_t *>(_data.data()), _data.size());
  sha256.final(_hash.data());
}

void Block::mine(size_t difficulty) {

  while (true) {
    calc();
    if (std::memcmp(_hash.data(), ZEROS, difficulty) == 0) {
      break;
    }
    _nonce++;
  }
}

void Block::serialize(std::ostream &os) const {

  net::io::write(os, "version   ", _version);
  net::io::write(os, "id        ", _id);
  net::io::write(os, "nonce     ", _nonce);
  net::io::write(os, "time      ", _time_since_epoch);
  net::io::write(os, "hash      ", _hash.data(), SHA256::DIGEST_SIZE);
  net::io::write(os, "data      ", _data.data(), _data.size());
}

void Block::deserialize(std::istream &is) {

  std::remove_const<decltype(_version)>::type version{};
  { // version
    net::io::read(is, "version   ", version);
    if (_version != version) {
      throw std::runtime_error{"Block migrate not implemented yet!"};
    }
  }

  net::io::read(is, "id        ", _id);
  net::io::read(is, "nonce     ", _nonce);
  net::io::read(is, "time      ", _time_since_epoch);
  net::io::read(is, "hash      ", _hash.data(), SHA256::DIGEST_SIZE);

  {
    assert(_data_size != static_cast<size_t>(-1));

    _data.clear();
    std::istreambuf_iterator<char> it{is}, end;
    if constexpr (true)
      copy_n(it, end, _data_size, std::back_inserter(_data));
    else
      std::copy_n(it, _data_size, std::back_inserter(_data));
  }

  _data_size = static_cast<size_t>(-1);
}

std::string Block::get_hash_str() const {
  std::ostringstream oss;
  for (auto &&c : _hash) {
    oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
  }
  return oss.str();
}

std::string Block::to_json() const {
  std::ostringstream oss;
  oss << "{";

  oss << "\"hash\": \"";
  for (auto &&c : _hash) {
    oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
  }
  oss << "\"";

  oss << ", \"version\": " << std::dec << _version;
  oss << ", \"id\": " << std::dec << _id;
  oss << ", \"nonce\": " << std::dec << _nonce;
  oss << ", \"time\": " << std::dec << _time_since_epoch;

  oss << ", \"prev_hash\": \"";
  const uint8_t *prev_hash =
      _prev_block ? _prev_block->get_hash().data() : ZEROS;
  for (size_t i = 0; i < SHA256::DIGEST_SIZE; i++) {
    oss << std::hex << std::setw(2) << std::setfill('0')
        << static_cast<int>(prev_hash[i]);
  }
  oss << "\"";

  oss << ", \"data\": \"";
  for (auto &&c : _data) {
    oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
  }
  oss << "\"";

  oss << "}";
  return oss.str();
}

} // namespace blockchain
