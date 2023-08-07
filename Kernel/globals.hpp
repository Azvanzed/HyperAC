#pragma once

#include <ntifs.h>

#include <cstdint>

#define print(fmt, ...) \
  DbgPrintEx(0, 0, "[HyperAC:" __FUNCTION__ "] " fmt "\n", __VA_ARGS__);

inline void* g_callback;
inline PEPROCESS g_service;
inline HANDLE g_service_pid;

#define TAG 'HPAC'