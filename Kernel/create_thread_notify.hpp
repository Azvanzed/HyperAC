#pragma once

#include <ntdef.h>

namespace create_thread_notify {
void Dispatcher(HANDLE pid, HANDLE tid, BOOLEAN create);
bool Register();
bool Unregister();
}  // namespace create_thread_notify