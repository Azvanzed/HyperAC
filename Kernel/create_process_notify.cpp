#include <create_process_notify.hpp>
#include <ntifs.h>
#include <modules.hpp>

void create_process_notify::Dispatcher(HANDLE parent_id, HANDLE pid,
                                       BOOLEAN create) {
  if (create) {
    PEPROCESS process = nullptr, parent = nullptr;
    PsLookupProcessByProcessId(pid, &process);
    PsLookupProcessByProcessId(parent_id, &parent);

    if (strcmp((char*)((uint64_t)process + 0x5a8), g_game_name) ||
        strcmp((char*)((uint64_t)parent + 0x5a8), g_service_name)) {
      goto cleanup;
    }

    g_service = parent;
    g_game = process;

    g_game_pid = pid;
    g_service_pid = parent_id;

  cleanup:
    ObfDereferenceObject(process);
    ObfDereferenceObject(parent);
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