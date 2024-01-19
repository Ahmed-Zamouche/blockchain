#pragma once

#include <iostream>

namespace blockchain {

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

class ISerializable {
public:
  virtual void serialize(std::ostream &os) const = 0;

  virtual void deserialize(std::istream &is) = 0;
};

} // namespace blockchain
