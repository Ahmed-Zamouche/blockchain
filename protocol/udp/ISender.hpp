#pragma once

#include <cstdint>
namespace net {

class ISender {
public:
  ISender(const char *addr, uint16_t port) : _addr{addr}, _port{port} {
    (void)_addr;
    (void)_port;
  }
  void run() {}

private:
  const char *_addr{"255.255.255.255"};
  uint16_t _port;
};

}; // namespace net
