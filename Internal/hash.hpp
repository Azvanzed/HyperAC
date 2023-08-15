#pragma once

#include <cstdint>

namespace hash {
	uint32_t calcHash(uint64_t start, uint64_t size, uint32_t hash = 0xffffffff);
}