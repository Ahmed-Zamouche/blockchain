#pragma once

#include "is_container.h"
#include <cstddef>

template <typename InputIt, typename OutputIt>
void copy_n(InputIt first, InputIt last, size_t count, OutputIt out) {
  for (size_t i = 0; i < count && first != last; ++i) {
    out = *first++;
  }
}
