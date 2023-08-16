#include <create_process_notify.hpp>
#include <ntifs.h>
#include <modules.hpp>
#include <service.hpp>


void create_process_notify::Dispatcher(HANDLE parent_id, HANDLE process_id, BOOLEAN create) {
    if (!g_game_pid) {
        PEPROCESS eprocess;
        if (!NT_SUCCESS(PsLookupProcessByProcessId(process_id, &eprocess))) {
            return;
        } else if (!strcmp((char*)((uint64_t)eprocess + 0x5a8), g_game_name)) {
            g_game_pid = process_id;
        }

        ObfDereferenceObject(eprocess);
    }

    if (g_game_pid != process_id) {
        return;
    }

    on_game_process_t callback;
    callback.type = service_callback_type_e::game_process;
    callback.created = create;
    callback.parent_pid = (uint64_t)parent_id;
    callback.process_id = (uint64_t)process_id;

    service::invokeRequestCallback(callback);
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