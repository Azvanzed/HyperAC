#include <ntifs.h>
#include <create_thread_notify.hpp>
#include <docs.hpp>
#include <modules.hpp>
#include <mmu.hpp>
#include <service.hpp>
#include <threads.hpp>

void create_thread_notify::onThreadCreation(thread_creation_t* ctx) {
  const int64_t delay = 1000 * -10000;
  KeDelayExecutionThread(KernelMode, false, (LARGE_INTEGER*)&delay);

  const uint64_t backer = modules::findUserBase(
      ctx->process, threads::getStartAddress(ctx->thread),
      (IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ) & ~IMAGE_SCN_MEM_WRITE);

  int output = service::invokeRequestCallback<int, int>(32);
  print("output: %i", output);
  
  return mmu::Free(ctx);
}

void create_thread_notify::Dispatcher(HANDLE pid, HANDLE tid,
                                      BOOLEAN create) {
  UNREFERENCED_PARAMETER(tid);
  UNREFERENCED_PARAMETER(create);

  if (create && pid == g_game_pid) {
    // avoid thread being created
    thread_creation_t* ctx = mmu::Allocate<thread_creation_t>(pool);
    PsLookupThreadByThreadId(tid, &ctx->thread);
    PsLookupProcessByProcessId(pid, &ctx->process);

    HANDLE hthread;
    if (!NT_SUCCESS(PsCreateSystemThread(&hthread, THREAD_ALL_ACCESS, nullptr, nullptr, nullptr,
            (PKSTART_ROUTINE)&onThreadCreation, (void*)ctx))) {
      mmu::Free(ctx);
      goto cleanup;
    }

    ZwClose(hthread);

cleanup:
    ObfDereferenceObject(ctx->process);
    ObfDereferenceObject(ctx->thread);
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