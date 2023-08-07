#pragma once 

#include <ntifs.h>
#include <cstdint>

namespace modules {
uint64_t findKernelBase(const char* name);
uint64_t findUserBase(const PEPROCESS process, const wchar_t* name);
uint64_t findUserBase(const PEPROCESS process, const uint64_t address,
                      const uint32_t flags);
bool isBacked(const uint64_t base, const uint64_t address, const uint32_t flags);
bool findUserName(const PEPROCESS process, const uint64_t base,
                           wchar_t* name);
}