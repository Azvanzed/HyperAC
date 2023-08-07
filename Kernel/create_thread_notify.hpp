#pragma once

#include <ntdef.h>

namespace create_thread_notify {
inline bool g_is_registered = false;

struct thread_creation_t {
  PEPROCESS process;
  PETHREAD thread;
};

void onThreadCreation(thread_creation_t* ctx);
void Dispatcher(HANDLE pid, HANDLE tid, BOOLEAN create);
bool Register();
bool Unregister();
}  // namespace create_thread_notify