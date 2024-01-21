#include "utils.h"
#include <gtest/gtest.h>

TEST(Utils, imemstream) {
  uint8_t buffer[256]{};
  for (size_t i = 0; i < sizeof(buffer); i++) {
    buffer[i] = (uint8_t)i;
  }

  imemstream imems{(char *)buffer, sizeof(buffer)};

  size_t i{};
  uint8_t ch;
  while (imems.read((char *)&ch, 1)) {
    EXPECT_EQ(ch, i++);
  }
  EXPECT_EQ(i, sizeof(buffer));
}

TEST(Utils, omemstream) {
  uint8_t buffer[256]{};

  omemstream omems{(char *)buffer, sizeof(buffer)};

  size_t i{};
  uint8_t ch{0};
  while (omems.write((char *)&ch, 1)) {
    ch++;
    i++;
  }
  EXPECT_EQ(i, sizeof(buffer));

  for (size_t i = 0; i < sizeof(buffer); i++) {
    EXPECT_EQ(buffer[i], (uint8_t)i);
  }
}
