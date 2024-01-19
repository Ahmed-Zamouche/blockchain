#pragma once

#include <cstdint>

namespace common {
namespace net {

constexpr uint8_t swap8(uint8_t v) { return v; }

} // namespace net
} // namespace common

#ifdef __linux__
#include <netinet/in.h>

#define htonll(x) ((((uint64_t)htonl(x)) << 32) + htonl((x) >> 32))
#define ntohll(x) htonll(x)

#else

#include <bit>

namespace common {
namespace net {

constexpr uint16_t swap16(uint16_t v) {
  static_assert(std::endian::native == std::endian::big ||
                    std::endian::native == std::endian::little,
                "Unknown endianness");
  if constexpr (std::endian::native == std::endian::little) {
    return (((uint16_t)swap8(v & 0xff) << 8) | swap8(v >> 8));
  }
  return v;
}

constexpr uint32_t swap32(uint32_t v) {
  if constexpr (std::endian::native == std::endian::little) {
    return (((uint32_t)swap16(v & 0xffff) << 16) | swap16(v >> 16));
  }
  return v;
}

constexpr uint64_t swap64(uint64_t v) {
  if constexpr (std::endian::native == std::endian::little) {
    return (((uint64_t)swap32(v & 0xffffffff) << 32) | swap32(v >> 32));
  }
  return v;
}

} // namespace net
} // namespace common

constexpr uint64_t htonll(uint64_t v) { return common::net::swap64(v); }
constexpr uint64_t ntohll(uint64_t v) { return common::net::swap64(v); }
constexpr uint32_t htonl(uint32_t v) { return common::net::swap32(v); }
constexpr uint32_t ntohl(uint32_t v) { return common::net::swap32(v); }
constexpr uint16_t htons(uint16_t v) { return common::net::swap16(v); }
constexpr uint16_t ntohs(uint16_t v) { return common::net::swap16(v); }

#endif
namespace common {
namespace net {

template <typename T> static inline T hton(T v) {
  // clang-format off
      static_assert(
          std::is_same<decltype(v), uint64_t>::value ||
          std::is_same<decltype(v), int64_t>::value ||
          std::is_same<decltype(v), uint32_t>::value ||
          std::is_same<decltype(v), int32_t>::value ||
          std::is_same<decltype(v), uint16_t>::value ||
          std::is_same<decltype(v), int16_t>::value ||
          std::is_same<decltype(v), uint8_t>::value ||
          std::is_same<decltype(v), int8_t>::value,
          "value must be of type uint64_t, uint32_t, uint16_t or uint8_t");
  // clang-format on
  if constexpr (sizeof(T) == sizeof(uint64_t))
    return htonll(v);
  else if constexpr (sizeof(T) == sizeof(uint32_t))
    return htonl(v);
  else if constexpr (sizeof(T) == sizeof(uint16_t))
    return htons(v);
  else if constexpr (sizeof(T) == sizeof(uint8_t))
    return swap8(v);
}

template <typename T> static inline T ntoh(T v) { return hton(v); }

} // namespace net
} // namespace common
