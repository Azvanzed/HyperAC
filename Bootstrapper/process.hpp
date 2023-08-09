#pragma once

#include <cstdint>
#include <Windows.h>

namespace process {
bool Create(const std::wstring& filepath, const std::wstring& args, PROCESS_INFORMATION* pi);
}