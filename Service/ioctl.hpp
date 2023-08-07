#pragma once

#include <Windows.h>
#include <cstdint>

namespace ioctl {
bool sendDriverEx(const uint32_t code, void* input, const size_t input_size,
                  void* output, const size_t output_size);

template <typename A, typename B>
bool sendDriver(const uint32_t code, const A& input, B* output) {
  return sendDriverEx(code, (void*)&input, sizeof(A), (void*)output, sizeof(B));
}
}  // namespace ioctl