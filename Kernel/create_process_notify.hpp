#pragma once

#include <ntdef.h>

namespace create_process_notify {
inline bool g_is_registered = false;

void Dispatcher(HANDLE parent_id, HANDLE process_id, BOOLEAN create);
bool Register();
bool Unregister();
}