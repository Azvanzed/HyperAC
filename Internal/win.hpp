#pragma once

#include <cstdint>

#include <Windows.h>
#include <winternl.h>

namespace win {
	uint64_t findModule(const wchar_t* name);
	uint64_t findExport(uint64_t base, const char* name);
	uint64_t findExport(const char* name);
}

#define API(name) ((decltype(&name))win::findExport(###name))