#pragma once

#include <ntddk.h>

#include <cstdint>

#define print(fmt, ...) \
  DbgPrintEx(0, 0, "[HyperAC:" __FUNCTION__ "] " fmt "\n", __VA_ARGS__);
