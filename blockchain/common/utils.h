#pragma once

#include "type_traits/is_container.h"
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <istream>
#include <ostream>
#include <ratio>
#include <streambuf>
#include <utility>

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

inline std::ostream &print_bytes(std::ostream &os, const uint8_t *data,
                                 size_t size) {
  if (size) {
    os << "0x" << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<unsigned int>(data[0]);
  }
  for (size_t i = 1; i < size; i++) {
    os << ", "
       << "0x" << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<unsigned int>(data[i]);
  }
  return os;
}

template <typename C,
          typename std::enable_if<blockchain::is_container<C>::value &&
                                  !std::is_same<C, std::string>::value>::type
              * = nullptr>
std::ostream &operator<<(std::ostream &os, const C &c) {
  return print_bytes(os, c.data(), c.size());
}

#include <chrono>
static inline uint64_t time_since_epoch() {
#ifdef NDEBUG
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
#else
  static uint64_t t{};
  return t++;
#endif
}

#include <iostream>
#include <random>

extern std::random_device g_random_device;
extern std::mt19937 g_random_number_engine;

namespace blockchain {
namespace utils {
template <typename IntType> inline static int get_random_number() {

  static std::uniform_int_distribution<std::mt19937::result_type> dist(
      std::numeric_limits<IntType>::min(), std::numeric_limits<IntType>::max());
  return dist(g_random_number_engine);
}

template <typename Period = std::milli, typename Callable, typename... Args>
std::chrono::duration<double, Period> run_duration(Callable &callable,
                                                   Args &&...args) {
  using std::chrono::high_resolution_clock;

  auto t0 = high_resolution_clock::now();
  callable(std::forward<Args>(args)...);
  auto t1 = high_resolution_clock::now();

  return (t1 - t0);
}
} // namespace utils
} // namespace blockchain
