#pragma once
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <openssl/evp.h>
#include <openssl/types.h>

namespace blockchain {
namespace security {

class SHA256 {
public:
  inline SHA256() : _mdctx{EVP_MD_CTX_create()} {}
  inline ~SHA256() { EVP_MD_CTX_destroy(_mdctx); }
  inline void init() { EVP_DigestInit_ex(_mdctx, EVP_sha256(), NULL); }
  inline void update(const std::uint8_t *message, std::size_t len) {
    EVP_DigestUpdate(_mdctx, message, len);
  }
  inline void final(std::uint8_t *digest) {
    unsigned int digest_len;
    EVP_DigestFinal_ex(_mdctx, digest, &digest_len);
    assert(digest_len == DIGEST_SIZE);
  }
  static constexpr std::size_t DIGEST_SIZE = (256 / 8);

protected:
  EVP_MD_CTX *_mdctx;
};

} // namespace security
} // namespace blockchain
