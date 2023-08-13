#pragma once

#include <ntifs.h>

#include <cstdint>

#define print(fmt, ...) \
  DbgPrintEx(0, 0, "[HyperAC:" __FUNCTION__ "] " fmt "\n", __VA_ARGS__);

inline void* g_callback;
inline HANDLE g_game_pid;

inline PEPROCESS g_service = nullptr;

#define TAG 'HPAC'