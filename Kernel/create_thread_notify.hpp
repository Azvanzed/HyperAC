#pragma once

#include <ntdef.h>

namespace create_thread_notify {
inline bool g_is_registered = false;

void Dispatcher(HANDLE process_id, HANDLE thread_id, BOOLEAN create);
bool Register();
bool Unregister();
}  // namespace create_thread_notify