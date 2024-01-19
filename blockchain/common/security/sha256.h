#pragma once

#include <cstdint>
#include <string>

#ifdef _WIN32
#include "sha256_win.hpp"
#elif __linux__
#include "sha256_linux.hpp"
#else
#endif
namespace blockchain {
namespace security {

std::string sha256(std::string input);

extern const uint8_t ZEROS[SHA256::DIGEST_SIZE];
extern const char *const ZEROS_STR;

} // namespace security
} // namespace blockchain
