#pragma once

#include <cstdint>

namespace service {
	bool invokeRequestCallbackEx(void* buffer, size_t size);

	template <typename T>
	bool invokeRequestCallback(const T& buffer) {
		return invokeRequestCallbackEx((void*)&buffer, sizeof(T));
	}

	bool sendHeartbeat();
}  // namespace service