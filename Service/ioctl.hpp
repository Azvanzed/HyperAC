#pragma once

#include <Windows.h>
#include <cstdint>

namespace ioctl {
bool sendDriverEx(uint32_t code, void* input, size_t input_size, void* output, size_t output_size);

template <typename A, typename B>
bool sendDriver(const uint32_t code, const A* input, B* output) {
  return sendDriverEx(code, (void*)input, sizeof(A), (void*)output, sizeof(B));
}
}  // namespace ioctl