#pragma once

#include <cstdint>
#include <string>

namespace scm {
bool Create(const std::wstring& name, const std::wstring& description, const std::wstring& path, const uint32_t type);
bool Start(const std::wstring& name);
bool Destroy(const std::wstring& name);
}