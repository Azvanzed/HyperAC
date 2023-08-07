#pragma once

#include <cstdint>

namespace files {
std::wstring toDosPath(const std::wstring& devicepath);
bool isSigned(const std::wstring& filepath);
}