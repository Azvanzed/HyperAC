#include <create_process_notify.hpp>
#include <ntifs.h>
#include <modules.hpp>
#include <service.hpp>

void create_process_notify::Dispatcher(HANDLE parent_id, HANDLE process_id,
                                       BOOLEAN create) {
  if (create) {
    on_process_creation_t callback;
    callback.type = user_callback_type_e::process_created;
    callback.parent_id = (uint64_t)parent_id;
    callback.process_id = (uint64_t)process_id;

    service::invokeRequestCallback(callback);
  }
}

bool create_process_notify::Register() {
  if (!NT_SUCCESS(PsSetCreateProcessNotifyRoutine(&Dispatcher, false))) {
    return false;
  }

  g_is_registered = true;
  return true;
}

bool create_process_notify::Unregister() {
  if (!g_is_registered) {
    return true;
  }

  g_is_registered = false;
  return NT_SUCCESS(PsSetCreateProcessNotifyRoutine(&Dispatcher, true));
}