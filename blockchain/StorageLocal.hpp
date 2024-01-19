#pragma once

#include "ISerializable.hpp"
#include "IStorage.hpp"
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace blockchain {
namespace storage {

class StorageLocal : public IStorageLocal {
public:
  StorageLocal(const std::filesystem::path &basename) : _basename{basename} {}
  void save(const ISerializable &obj) const override {

    namespace fs = std::filesystem;

    if (!fs::is_directory(_base_path) || !fs::exists(_base_path)) {
      fs::create_directory(_base_path);
    }
    std::ofstream ofs{_base_path / _basename, std::ofstream::out |
                                                  std::ofstream::binary |
                                                  std::ofstream::trunc};

    if (!ofs.is_open()) {
      std::ostringstream oss;
      std::string filename = (_base_path / _basename).string();
      oss << "Failed to open " << std::quoted(filename) << ": "
          << std::strerror(errno);
      throw std::runtime_error{oss.str()};
    }

    obj.serialize(ofs);
    ofs.close();
  }
  void load(ISerializable &obj) const override {
    std::ifstream ifs{_base_path / _basename,
                      std::ifstream::in | std::ifstream::binary};

    if (!ifs.is_open()) {
      std::ostringstream oss;
      std::string filename = (_base_path / _basename).string();
      oss << "Failed to open " << std::quoted(filename) << ": "
          << std::strerror(errno);
      throw std::runtime_error{oss.str()};
    }

    obj.deserialize(ifs);
    ifs.close();
  }

private:
  std::filesystem::path _base_path{"./data/"};
  std::filesystem::path _basename;
};

} // namespace storage
} // namespace blockchain
