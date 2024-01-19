#pragma once

#include "byteorder.h"
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace common {
namespace net {
namespace io {

using ios_failure = std::ios_base::failure;
class ios_eof : public ios_failure {
public:
  explicit ios_eof(const std::string &message,
                   const std::error_code &ec = std::io_errc::stream)
      : ios_failure{message, ec} {}
  explicit ios_eof(const char *message,
                   const std::error_code &ec = std::io_errc::stream)
      : ios_failure(message, ec) {}
  ios_eof(const ios_eof &other) noexcept : ios_failure(other) {}
  ios_eof &operator=(const ios_eof &other) noexcept {
    ios_failure::operator=(other);
    return *this;
  }
  virtual const char *what() const noexcept { return ios_failure::what(); }
};

inline void write(std::ostream &os, const char *name, const uint8_t *data,
                  size_t size) {
  if (!os.write(reinterpret_cast<const char *>(data), size)) {
    std::ostringstream oss;
    oss << "Failed to write " << std::quoted(name, '\'');
    throw ios_failure{oss.str()};
  }
}

inline void write(std::ostream &os, const char *name, const char *data,
                  size_t size) {
  write(os, name, reinterpret_cast<const uint8_t *>(data), size);
}

template <typename T>
inline void write(std::ostream &os, const char *name, T value) {
  value = net::hton(value);
  write(os, name, reinterpret_cast<const uint8_t *>(&value), sizeof(T));
}

inline void read(std::istream &is, const char *name, uint8_t *data,
                 size_t size) {
  if (!is.read(reinterpret_cast<char *>(data), size)) {
    if (is.gcount() == 0) { // eof no error
      throw ios_eof{"eof"};
    }

    std::ostringstream oss;
    oss << "Failed to read " << std::quoted(name, '\'');
    throw ios_failure{oss.str()};
  }
}

inline void read(std::istream &is, const char *name, char *data, size_t size) {

  read(is, name, reinterpret_cast<uint8_t *>(data), size);
}

template <typename T>
inline void read(std::istream &is, const char *name, T &value) {
  read(is, name, reinterpret_cast<uint8_t *>(&value), sizeof(T));
  value = net::ntoh(value);
}
} // namespace io
} // namespace net
} // namespace common
