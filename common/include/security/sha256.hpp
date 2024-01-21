#pragma once

#if defined(__linux__)
#include <cassert>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/types.h>
#endif

#include <cstdint>
#include <string>

namespace common {
namespace security {

class Sha256 {
public:
#if defined(_WIN32)
  Sha256() {}
  ~Sha256() {}
  void init();
  void update(const std::uint8_t *message, std::size_t len);
  void final(std::uint8_t *digest);
#elif defined(__linux__)
  inline Sha256() : _mdctx{EVP_MD_CTX_create()} {}
  inline ~Sha256() { EVP_MD_CTX_destroy(_mdctx); }
  inline void init() { EVP_DigestInit_ex(_mdctx, EVP_sha256(), NULL); }
  inline void update(const std::uint8_t *message, std::size_t len) {
    EVP_DigestUpdate(_mdctx, message, len);
  }
  inline void final(std::uint8_t *digest) {
    unsigned int digest_len;
    EVP_DigestFinal_ex(_mdctx, digest, &digest_len);
    assert(digest_len == Sha256::DIGEST_SIZE);
  }
#else
#error ""
#endif
  static constexpr std::size_t DIGEST_SIZE = (256 / 8);

private:
#if defined(_WIN32)
  void transform(const std::uint8_t *message, std::size_t block_nb);
  std::size_t m_tot_len;
  std::size_t m_len;
  std::uint32_t m_h[8];
#elif defined(__linux__)
  EVP_MD_CTX *_mdctx;
#else
#error ""
#endif
};

std::string sha256(std::string input);
extern const uint8_t ZEROS[Sha256::DIGEST_SIZE];
extern const char *const ZEROS_STR;

} // namespace security
} // namespace common
