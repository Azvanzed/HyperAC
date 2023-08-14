#include <mmu.hpp>

void mmu::Free(void* base) {
	API(VirtualFree)(base, 0, MEM_RELEASE);
}

void* mmu::allocateEx(size_t size, uint32_t protect) {
	return API(VirtualAlloc)(nullptr, size, MEM_RESERVE | MEM_COMMIT, protect);
}