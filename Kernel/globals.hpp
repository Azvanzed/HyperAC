#pragma once

#include <ntifs.h>

#include <cstdint>

#define print(fmt, ...) \
  DbgPrintEx(0, 0, "[HyperAC:" __FUNCTION__ "] " fmt "\n", __VA_ARGS__);

inline void* g_service_callback = nullptr;
inline HANDLE g_game_pid = nullptr;
inline char g_game_name[15];
inline PEPROCESS g_service = nullptr;

#define TAG 'HPAC'