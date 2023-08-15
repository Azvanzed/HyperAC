#include <mmu.hpp>
#include <service.hpp>
#include <shared.hpp>
#include <threads.hpp>
#include <docs.hpp>

bool service::invokeRequestCallbackEx(void* buffer, size_t size) {
  PsAcquireProcessExitSynchronization(g_service);
  KeAttachProcess(g_service);
  
  void* alloc = nullptr;
  size_t alloc_size = size;
  if (!NT_SUCCESS(ZwAllocateVirtualMemory((HANDLE)-1, &alloc, 0, &alloc_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE))) {
    KeDetachProcess();
    PsReleaseProcessExitSynchronization(g_service);
    return false;
  }

  memcpy(alloc, buffer, size);
  bool result = threads::createUserThread(g_service_callback, alloc);

  KeDetachProcess();
  PsReleaseProcessExitSynchronization(g_service);
  return result;
}