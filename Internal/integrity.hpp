#pragma once

#include <cstdint>
#include <vector.hpp>

namespace integrity
{
	struct patch_t {
		uint32_t offset;
		uint8_t value;
	};

	struct integrity_t {
		char filepath[MAX_PATH];
		uint32_t timestamp;
		uint32_t hash;
		vector<patch_t> patches;
	};

	enum integrity_status_e : uint8_t {
		integrity_failed,
		integrity_corrupted,
		integrity_valid
	};

	integrity_status_e verifyModule(uint64_t base, integrity_t* integrity);
}

