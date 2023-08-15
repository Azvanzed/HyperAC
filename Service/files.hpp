#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace files {
std::wstring toDosPath(const std::wstring& devicepath);
bool isSigned(const std::wstring& filepath);
bool Read(const std::string& filepath, std::vector<uint8_t>* buffer);
}