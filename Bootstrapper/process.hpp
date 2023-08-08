#pragma once

#include <cstdint>
#include <Windows.h>

namespace process {
bool Create(const wchar_t* filepath, const wchar_t* args, PROCESS_INFORMATION* pi);
}