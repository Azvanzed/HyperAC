#pragma once

#include <ntdef.h>

namespace create_process_notify {
void Dispatcher(HANDLE parent_id, HANDLE pid, BOOLEAN create);
bool Register();
bool Unregister();
}