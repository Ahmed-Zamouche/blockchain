#include "utils.h"

std::random_device g_random_device;

#ifndef NDEBUG
std::mt19937 g_random_number_engine(g_random_device());
#else
std::mt19937 g_random_number_engine(0);
#endif
