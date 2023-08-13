#include <mmu.hpp>
#include <service.hpp>
#include <shared.hpp>
#include <threads.hpp>
#include <docs.hpp>

bool service::invokeRequestCallbackEx(void* buffer, const size_t buffer_size) {
  KeAttachProcess(g_service);
  PsAcquireProcessExitSynchronization(g_service);
  
  void* alloc = nullptr;
  size_t alloc_size = buffer_size;
  if (!NT_SUCCESS(ZwAllocateVirtualMemory((HANDLE)-1, &alloc, 0, &alloc_size,
                                          MEM_COMMIT | MEM_RESERVE,
                                          PAGE_READWRITE))) {
    PsReleaseProcessExitSynchronization(g_service);
    KeDetachProcess();
    return false;
  }

  size_t bytes;
  MM_COPY_ADDRESS address = {.VirtualAddress = alloc};
  MmCopyMemory(alloc, address, buffer_size, MM_COPY_MEMORY_PHYSICAL, &bytes);

  memcpy(alloc, buffer, buffer_size);
  const bool result = threads::createUserThread(g_callback, alloc);

  PsReleaseProcessExitSynchronization(g_service);
  KeDetachProcess();
  return result;
}