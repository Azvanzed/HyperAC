#pragma once

#include <cstdint>

namespace integrity
{
#pragma pack(push, 1)
	struct modified_byte_t {
		char name[8];
		uint32_t offset;
		uint8_t patch;
	};
#pragma pop

	enum integrity_status_e : uint8_t {
		integrity_failed,
		integrity_corrupted,
		integrity_valid
	};

	integrity_status_e verifyModuleIntegrity(uint64_t base, modified_byte_t** modified_bytes, uint32_t* modified_count);
}

