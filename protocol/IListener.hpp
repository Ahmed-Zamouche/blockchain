#pragma once

#include <cstdint>
#include <istream>
#include <ostream>
#include <streambuf>

namespace net {

struct membuf : std::streambuf {
  membuf(const char *base, size_t size) {
    char *p(const_cast<char *>(base));
    this->setg(p, p, p + size);
  }
};

struct imemstream : virtual membuf, std::istream {
  imemstream(const char *base, size_t size)
      : membuf(base, size), std::istream(static_cast<std::streambuf *>(this)) {}
};

struct omemstream : virtual membuf, std::ostream {
  omemstream(const char *base, size_t size)
      : membuf(base, size), std::ostream(static_cast<std::streambuf *>(this)) {}
};

class IListener {
public:
  IListener(const char *addr, uint16_t port) : _addr{addr}, _port{port} {
    (void)_addr;
    (void)_port;
  }

  void run() {}

private:
  const char *_addr{"255.255.255.255"};
  uint16_t _port;
};

} // namespace net
