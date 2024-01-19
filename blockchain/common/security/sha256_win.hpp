#pragma once

#include <cstddef>
#include <cstdint>

namespace blockchain {
namespace security {

class SHA256 {

public:
  void init();
  void update(const std::uint8_t *message, std::size_t len);
  void final(std::uint8_t *digest);
  static constexpr std::size_t DIGEST_SIZE = (256 / 8);

protected:
  static constexpr size_t SHA224_256_BLOCK_SIZE = (512 / 8);
  static const std::uint32_t sha256_k[64];
  void transform(const std::uint8_t *message, std::size_t block_nb);
  std::size_t m_tot_len;
  std::size_t m_len;
  std::uint8_t m_block[2 * SHA224_256_BLOCK_SIZE];
  std::uint32_t m_h[8];
};

} // namespace security
} // namespace blockchain
