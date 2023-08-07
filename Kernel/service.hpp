#pragma once

#include <cstdint>

namespace service {
bool invokeRequestCallbackEx(void* input, const size_t input_size, void* output,
                             const size_t output_size);

template <typename A, typename B>
B invokeRequestCallback(const A& input) {
  B result;
  memset((void*)&result, 0, sizeof(B));

  invokeRequestCallbackEx((void*)&input, sizeof(A), (void*)&result, sizeof(B));
  return result;
}
}  // namespace service