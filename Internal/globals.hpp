#pragma once

#include <cstdint>

inline uint64_t g_process_base = 0;
inline void* g_service_callback = nullptr;

#ifdef _DEBUG
#include <iostream>
#define LOG(fmt, ...) printf(fmt "\n", __VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif