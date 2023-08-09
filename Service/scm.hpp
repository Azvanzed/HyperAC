#pragma once

#include <cstdint>

namespace scm {
bool Create(const wchar_t* name, const wchar_t* description,
            const wchar_t* filepath, const uint32_t type);
bool Start(const wchar_t* name);
bool Destroy(const wchar_t* name);
}