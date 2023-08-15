#pragma once

#include <cstdint>

inline void* g_service_callback = nullptr;
inline void* g_service = nullptr;
 
#ifdef _DEBUG
#include <iostream>
#define LOG(fmt, ...) printf(fmt "\n", __VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif