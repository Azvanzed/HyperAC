#include <create_thread_notify.hpp>

void create_thread_notify::Dispatcher(HANDLE pid, HANDLE tid,
                                      BOOLEAN create) {
  UNREFERENCED_PARAMETER(pid);
  UNREFERENCED_PARAMETER(tid);
  UNREFERENCED_PARAMETER(create);
}

bool create_thread_notify::Register() {
  return NT_SUCCESS(PsSetCreateThreadNotifyRoutine(&Dispatcher));
}

bool create_thread_notify::Unregister() {
  return NT_SUCCESS(PsRemoveCreateThreadNotifyRoutine(&Dispatcher));
}