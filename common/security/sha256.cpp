
#include "security/sha256.hpp"
#include <cstdint>

namespace common {
namespace security {

const uint8_t ZEROS[Sha256::DIGEST_SIZE] = {};
const char *const ZEROS_STR =
    "0000000000000000000000000000000000000000000000000000000000000000";

std::string sha256(std::string input) {
  std::uint8_t digest[Sha256::DIGEST_SIZE];
  memset(digest, 0, Sha256::DIGEST_SIZE);

  Sha256 sha256{};

  sha256.init();
  sha256.update((std::uint8_t *)input.c_str(), input.length());
  sha256.final(digest);

  char buf[2 * Sha256::DIGEST_SIZE + 1];
  buf[2 * Sha256::DIGEST_SIZE] = 0;
  for (std::size_t i = 0; i < Sha256::DIGEST_SIZE; i++)
    sprintf(buf + i * 2, "%02x", digest[i]);
  return std::string(buf);
}

} // namespace security
} // namespace common
