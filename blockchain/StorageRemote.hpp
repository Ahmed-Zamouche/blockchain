#pragma once

#include "ISerializable.hpp"
#include "IStorage.hpp"

namespace blockchain {
namespace storage {

class StorageRemote : public IStorageRemote {
public:
  StorageRemote() {}
  void sync(const ISerializable &obj) override {}
};

} // namespace storage
} // namespace blockchain
