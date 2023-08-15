#include <hash.hpp>
#include <intrin.h>

uint32_t hash::calcHash(uint64_t start, uint64_t size, uint32_t hash) {
	for (uint64_t curr = start; curr < start + size; ++curr) {
		hash = _mm_crc32_u8(hash, *(uint8_t*)curr);
	}

	return hash;
}