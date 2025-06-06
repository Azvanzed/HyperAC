#pragma once

#include <cstdint>
#include <wdm.h>

namespace threads {
CLIENT_ID getCid(const PETHREAD thread);
bool createUserThread(void* routine, void* param);
uint64_t getStartAddress(const PETHREAD thread);
}