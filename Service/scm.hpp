#pragma once

#include <cstdint>

namespace scm {
bool Create(const std::wstring& name, const std::wstring& desc, const std::wstring& filepath, const uint32_t type);
bool Start(const std::wstring& name);
bool Destroy(const std::wstring& name);
}