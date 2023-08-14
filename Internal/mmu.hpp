#pragma once

#include <cstdint>

namespace mmu {
	void Free(void* base);

	void* allocateEx(size_t size, uint32_t protect = PAGE_READWRITE);

	template <typename T>
	T* Allocate() {
		return (T*)allocateEx(sizeof(T));
	}	

	template <typename T>
	T Allocate(size_t size) {
		return (T)allocateEx(size);
	}
}