#include <mmu.hpp>

void mmu::Init() {
  g_alloc_count = 0;
  g_chunk_count = 1;

  const size_t pool_size = sizeof(alloc_t) * g_chunk_count * chunk_size;
  g_allocs = (alloc_t*)ExAllocatePoolWithTag(NonPagedPoolNx, pool_size, TAG);
}

void mmu::Reallocate() {
  g_chunk_count += 1;

  alloc_t* new_allocs = (alloc_t*)ExAllocatePoolWithTag(
      NonPagedPoolNx, sizeof(alloc_t) * g_chunk_count * chunk_size, TAG);
  memcpy(new_allocs, g_allocs, sizeof(alloc_t) * g_alloc_count);

  ExFreePoolWithTag(g_allocs, 0);
  g_allocs = new_allocs;
}

void mmu::Release() {
  for (size_t i = 0; i < g_alloc_count; ++i) {
    freeEx(&g_allocs[i]);
  }

  g_alloc_count = 0;
  g_chunk_count = 1;
}

mmu::alloc_t* mmu::Find(void* base) {
  for (size_t i = 0; i < g_alloc_count; ++i) {
    alloc_t* alloc = &g_allocs[i];
    if (alloc->base == base) {
      return alloc;
    }
  }

  return nullptr;
}

void mmu::freeEx(alloc_t* alloc) {
  size_t index = g_alloc_count;
  for (size_t i = 0; i < g_alloc_count; ++i) {
    if (&g_allocs[i] == alloc) {
      index = i;
      break;
    }
  }

  if (index == g_alloc_count) {
    return;
  }

  switch (alloc->type) {
    case contiguous:
      MmFreeContiguousMemory(alloc->base);
      break;
    case pool:
      ExFreePoolWithTag(alloc->base, TAG);
      break;
  }

  for (size_t i = index; i < g_alloc_count - 1; ++i) {
    g_allocs[i] = g_allocs[i + 1];
  }

  --g_alloc_count;
}

mmu::alloc_t* mmu::allocateEx(const alloc_type type, const size_t size) {
  if (g_alloc_count >= g_chunk_count * chunk_size) {
    Reallocate();
  }

  alloc_t* alloc = &g_allocs[g_alloc_count++];
  alloc->type = type;
  alloc->size = size;

  switch (type) {
    case pool:
      alloc->base = ExAllocatePoolWithTag(NonPagedPoolNx, size, TAG);
      break;
    case contiguous:
      constexpr PHYSICAL_ADDRESS highest = {.QuadPart = (int64_t)MAXULONG64};
      alloc->base = MmAllocateContiguousMemory(size, highest);
      break;
  }

  memset(alloc->base, 0, alloc->size);
  return alloc;
}