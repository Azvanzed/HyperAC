#pragma once

#include <ntifs.h>

#include <cstdint>

#define print(fmt, ...) \
  DbgPrintEx(0, 0, "[HyperAC:" __FUNCTION__ "] " fmt "\n", __VA_ARGS__);

inline char g_game_name[15];
inline char g_service_name[15];

inline PEPROCESS g_game;
inline PEPROCESS g_service;

inline HANDLE g_game_pid;
inline HANDLE g_service_pid;

inline void* g_request_callback;

inline uint64_t g_ntoskrnl;

#define TAG 'HPAC'