#pragma once

#include <cstdint>

inline uint64_t g_process_base = 0;
inline void* g_service_callback = nullptr;

#ifdef _DEBUG
#include <iostream>
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif