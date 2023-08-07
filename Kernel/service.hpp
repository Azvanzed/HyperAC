#pragma once

#include <cstdint>

namespace service {
bool invokeRequestCallbackEx(void* buffer, const size_t buffer_size);

template <typename T>
bool invokeRequestCallback(const T& buffer) {
  return invokeRequestCallbackEx((void*)&buffer, sizeof(T));
}
}  // namespace service