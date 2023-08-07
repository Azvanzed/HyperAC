#pragma once

#include <ntddk.h>

#include <cstdint>

enum alloc_type : uint8_t { contiguous, pool };

namespace mmu {
struct alloc_t {
  alloc_type type;
  void* base;
  size_t size;
};

constexpr uint32_t chunk_size = 100;
inline size_t g_chunk_count;
inline alloc_t* g_allocs;
inline size_t g_alloc_count;

void Init();
void Reallocate();
void Release();

alloc_t* Find(void* base);
void freeEx(alloc_t* alloc);

template <typename T = void*>
void Free(T pool) {
  return freeEx(Find((void*)pool));
}

alloc_t* allocateEx(const alloc_type type, const size_t size);

template <typename T = void*>
T Allocate(const alloc_type type, const size_t size) {
  return (T)allocateEx(type, size)->base;
}

template <typename T = void*>
T* Allocate(const alloc_type type) {
  return (T*)allocateEx(type, sizeof(T))->base;
}
}  // namespace mmu