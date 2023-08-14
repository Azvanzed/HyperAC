#pragma once

#include <cstdint>
#include <Windows.h>

namespace processes {
	bool isDllLoaded(HANDLE handle, uint64_t base);
	int32_t findProcessId(const std::wstring& name);
}