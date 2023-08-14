#pragma once

#include <cstdint>

namespace manual_mapper {
	bool inject(int32_t process_id, void* data, size_t size, uint64_t arg);
}