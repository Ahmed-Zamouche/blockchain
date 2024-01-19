#include "sha256.h"
#include <cstdint>
#include <cstring>

namespace blockchain {
namespace security {

const uint8_t ZEROS[SHA256::DIGEST_SIZE] = {};
const char *const ZEROS_STR =
    "0000000000000000000000000000000000000000000000000000000000000000";

std::string sha256(std::string input) {
  std::uint8_t digest[SHA256::DIGEST_SIZE];
  memset(digest, 0, SHA256::DIGEST_SIZE);

  SHA256 ctx = SHA256();
  ctx.init();
  ctx.update((std::uint8_t *)input.c_str(), input.length());
  ctx.final(digest);

  char buf[2 * SHA256::DIGEST_SIZE + 1];
  buf[2 * SHA256::DIGEST_SIZE] = 0;
  for (std::size_t i = 0; i < SHA256::DIGEST_SIZE; i++)
    sprintf(buf + i * 2, "%02x", digest[i]);
  return std::string(buf);
}

} // namespace security
} // namespace blockchain
