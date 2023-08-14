#pragma once

#include <cstdint>

namespace win {
	uint64_t findModule(const wchar_t* name);
	uint64_t findExport(uint64_t base, const char* name);
}