#pragma once

#include <cstdint>
#include <wdm.h>

namespace threads {
bool createUserThread(void* routine, void* parameter, const bool wait = true);
uint64_t getStartAddress(const PETHREAD thread);
}