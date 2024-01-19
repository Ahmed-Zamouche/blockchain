#pragma once

#include "ISerializable.hpp"
#include "blockchain/common/utils.h"
#include "common/security/sha256.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <ctime>
#include <memory>

namespace blockchain {

class Block : public ISerializable {
public:
  Block(uint32_t index, const std::string &data, Block *prev_block)
      : _id{index}, _nonce{static_cast<uint64_t>(-1)},
        _time_since_epoch{time_since_epoch()}, _prev_block{prev_block},
        _data{data} {}

  Block(uint32_t index, const std::string &data)
      : Block(index, data, nullptr) {}

  Block(uint32_t index, std::string &&data, Block *prev_block)
      : _id{index}, _nonce{static_cast<uint64_t>(-1)},
        _time_since_epoch{time_since_epoch()}, _prev_block{prev_block},
        _data{std::move(data)} {}
  Block(uint32_t index, std::string &&data)
      : Block(index, std::forward<std::string>(data), nullptr) {}

  Block(const Block &o) {
    if (this != &o) {
      _id = o._id;
      _nonce = o._nonce;
      _time_since_epoch = o._time_since_epoch;
      _prev_block = o._prev_block;
      _data = o._data;
      _hash = o._hash;
    }
  }
  Block &operator=(const Block &o) {
    if (this != &o) {
      _id = o._id;
      _nonce = o._nonce;
      _time_since_epoch = o._time_since_epoch;
      _prev_block = o._prev_block;
      _data = o._data;
      _hash = o._hash;
    }
    return *this;
  }
  Block(Block &&o) {
    if (this != &o) {
      _id = o._id;
      _nonce = o._nonce;
      _time_since_epoch = o._time_since_epoch;
      _prev_block = o._prev_block;
      _data = o._data;
      _hash = o._hash;
    }
  }
  Block &operator=(Block &&o) {
    if (this != &o) {
      _id = o._id;
      _nonce = o._nonce;
      _time_since_epoch = o._time_since_epoch;
      _prev_block = o._prev_block;
      _data = o._data;
      _hash = o._hash;
    }
    return *this;
  }
  virtual ~Block() = default;

  void calc();
  void mine(size_t difficulty);
  bool is_valid() const;

  // getter
  uint32_t get_id() const { return _id; }
  uint64_t get_nonce() const { return _nonce; }
  uint64_t get_time() const { return _time_since_epoch; }
  uint32_t get_version() const { return _version; }
  const Block *get_prev_block() const { return _prev_block; }
  std::string get_prev_hash_str() const {
    return _prev_block ? _prev_block->get_hash_str() : security::ZEROS_STR;
  }
  const uint8_t *get_prev_hash() const {
    return _prev_block ? _prev_block->get_hash().data() : security::ZEROS;
  }
  const std::string &get_data() const { return _data; }
  std::string &get_data() { return _data; }
  const std::array<uint8_t, security::SHA256::DIGEST_SIZE> &get_hash() const {
    return _hash;
  }
  std::array<uint8_t, security::SHA256::DIGEST_SIZE> &get_hash() {
    return _hash;
  }
  std::string get_hash_str() const;

  // setter
  void set_id(uint32_t id) { _id = id; }
  void set_nonce(uint64_t nonce) { _nonce = nonce; }
  void set_time(uint64_t time) { _time_since_epoch = time; }
  void set_prev_block(Block *prev_block) { _prev_block = prev_block; }
  void set_data(const std::string &data) { _data = data; }
  void set_data(std::string &&data) { _data = std::move(data); }
  void set_data_size(size_t size) { _data_size = size; }
  void set_hash(const uint8_t *data, size_t size) {
    assert(size == security::SHA256::DIGEST_SIZE);
    std::copy_n(data, size, _hash.begin());
  }

  void
  set_hash(const std::array<uint8_t, security::SHA256::DIGEST_SIZE> &hash) {
    _hash = hash;
  }
  void set_hash(std::array<uint8_t, security::SHA256::DIGEST_SIZE> &&hash) {
    _hash = std::move(hash);
  }

  virtual void serialize(std::ostream &os) const override;
  virtual void deserialize(std::istream &is) override;

  std::string to_json() const;
  std::string to_string() const { return to_json(); }

private:
  friend bool operator==(const Block &lhs, const Block &rhs);
  friend std::ostream &operator<<(std::ostream &os, const Block &block);

  const uint32_t _version{1};
  uint32_t _id;
  uint64_t _nonce;
  uint64_t _time_since_epoch;
  Block *_prev_block;
  std::string _data;
  size_t _data_size{static_cast<size_t>(-1)}; // used for deserialize
  std::array<uint8_t, security::SHA256::DIGEST_SIZE> _hash;
  security::SHA256 _sha256{};
};

inline bool operator==(const Block &lhs, const Block &rhs) {
  // clang-format off
  bool equal =
      lhs._version == rhs._version &&
      lhs._id == rhs._id &&
      lhs._nonce == rhs._nonce &&
      lhs._time_since_epoch == rhs._time_since_epoch &&
      lhs._data.size() == rhs._data.size() &&
      memcmp(lhs._data.data(), rhs._data.data(), lhs._data.size()) == 0 &&
      lhs._hash == rhs._hash;
  // clang-format on
  if (!equal) {
    std::cout << "lhs: " << lhs << std::endl;
    std::cout << "rhs: " << rhs << std::endl;
  }
  return equal;
}

inline bool operator!=(const Block &lhs, const Block &rhs) {
  return !(lhs == rhs);
}

inline bool operator==(const std::unique_ptr<Block> &lhs,
                       const std::unique_ptr<Block> &rhs) {
  if (lhs.get() == rhs.get())
    return true;
  else if (lhs.get() == nullptr || rhs.get() == nullptr)
    return false;
  else
    return *lhs == *rhs;
}

inline bool operator!=(const std::unique_ptr<Block> &lhs,
                       const std::unique_ptr<Block> &rhs) {
  return !(lhs == rhs);
}

inline std::ostream &operator<<(std::ostream &os, const Block &block) {
  return os << block.to_json();
}
} // namespace blockchain
