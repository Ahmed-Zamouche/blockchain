#pragma once

#include "Block.hpp"
#include "ISerializable.hpp"
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace blockchain {

class Blockchain : public ISerializable {
  typedef std::unique_ptr<Block> value_type;
  typedef typename std::vector<value_type> block_type;

public:
  typedef typename block_type::iterator iterator;
  typedef typename block_type::const_iterator const_iterator;

  Blockchain() { insert("Genesis Block"); }

  void insert(const std::string &data) {
    _bchain.push_back(std::make_unique<Block>(
        static_cast<uint32_t>(_bchain.size()), data,
        _bchain.size() ? _bchain.back().get() : nullptr));
    _bchain.back()->mine(_difficulty);
  }
  const Block &front() const { return *_bchain.front(); }

  const Block &back() const { return *_bchain.back(); }
  size_t size() const { return _bchain.size(); }
  bool is_valid() const {
    for (auto &&block : _bchain) {
      if (!block->is_valid())
        return false;
    }
    return true;
  }

  virtual void serialize(std::ostream &os) const override;
  virtual void deserialize(std::istream &is) override;

  iterator begin() { return _bchain.begin(); }
  const_iterator begin() const { return _bchain.begin(); }
  const_iterator cbegin() const { return _bchain.cbegin(); }
  iterator end() { return _bchain.end(); }
  const_iterator end() const { return _bchain.end(); }
  const_iterator cend() const { return _bchain.cend(); }

  std::string to_json() const;
  std::string to_string() const { return to_json(); }

private:
  friend bool operator==(const Blockchain &lhs, const Blockchain &rhs);
  friend std::ostream &operator<<(std::ostream &os,
                                  const Blockchain &blockchain);

  const uint32_t _version{1};
  uint32_t _difficulty{2};
  std::vector<std::unique_ptr<Block>> _bchain;
};

inline bool operator==(const Blockchain &lhs, const Blockchain &rhs) {
  return lhs._version == rhs._version && lhs._difficulty == rhs._difficulty &&
         lhs._bchain == rhs._bchain;
}

inline bool operator!=(const Blockchain &lhs, const Blockchain &rhs) {
  return !(lhs == rhs);
}

inline std::ostream &operator<<(std::ostream &os,
                                const Blockchain &blockchain) {
  return os << blockchain.to_string();
}

class BlockchainManager {
public:
private:
};

} // namespace blockchain
