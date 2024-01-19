#pragma once

#include "ISerializable.hpp"

namespace blockchain {
namespace storage {

class IStorage {};

class IStorageLocal : public IStorage {
public:
  virtual void save(const ISerializable &) const = 0;
  virtual void load(ISerializable &) const = 0;
};

class IStorageRemote : public IStorage {
public:
  virtual void sync(const ISerializable &) = 0;
};

} // namespace storage
} // namespace blockchain
