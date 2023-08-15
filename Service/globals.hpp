#pragma once

#include <shared.hpp>
#include <string>
#include <vector>


#define VEH_TRIGGER() *(void**)nullptr = 0;
inline uint32_t g_last_internal_tick = 0;