#pragma once

#include <cstdint>
#include <vector>

namespace kernel {
	bool isAlive();
	bool Init(void* callback, const char* game_name);
	bool Inject(uint64_t process_id, void* callback, const std::vector<uint8_t>& binary);
	bool Unit();
}