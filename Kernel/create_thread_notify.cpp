#include <ntifs.h>
#include <create_thread_notify.hpp>
#include <docs.hpp>
#include <modules.hpp>
#include <mmu.hpp>
#include <service.hpp>
#include <threads.hpp>
#include <shared.hpp>

void create_thread_notify::Dispatcher(HANDLE process_id, HANDLE thread_id, BOOLEAN create) {
  if (g_game_pid == process_id) {
    PETHREAD thread;
    if (!NT_SUCCESS(PsLookupThreadByThreadId(thread_id, &thread))) {
      return;
    }

    on_thread_creation_t callback;
    callback.type = service_callback_type_e::thread_created;
    callback.create = create;
    callback.start = threads::getStartAddress(thread);
    callback.parent_pid = (uint64_t)PsGetCurrentProcessId();
    callback.thread_id = (uint64_t)thread_id;
    callback.process_id = (uint64_t)process_id;

    service::invokeRequestCallback(callback);
    ObfDereferenceObject(thread);
  }
}

bool create_thread_notify::Register() {
  if (!NT_SUCCESS(PsSetCreateThreadNotifyRoutine(&Dispatcher))) {
    return false;
  }

  g_is_registered = true;
  return true;
}

bool create_thread_notify::Unregister() {
  if (!g_is_registered) {
    return true;
  }

  g_is_registered = false;
  return NT_SUCCESS(PsRemoveCreateThreadNotifyRoutine(&Dispatcher));
}