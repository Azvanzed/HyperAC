#include <create_process_notify.hpp>

void create_process_notify::Dispatcher(HANDLE parent_id, HANDLE pid,
                                       BOOLEAN create) {
  UNREFERENCED_PARAMETER(parent_id);
  UNREFERENCED_PARAMETER(pid);
  UNREFERENCED_PARAMETER(create);
}

bool create_process_notify::Register() {
  return NT_SUCCESS(PsSetCreateProcessNotifyRoutine(&Dispatcher, false));
}

bool create_process_notify::Unregister() {
  return NT_SUCCESS(PsSetCreateProcessNotifyRoutine(&Dispatcher, true));
}