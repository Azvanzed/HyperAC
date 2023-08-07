#include <mmu.hpp>
#include <service.hpp>
#include <shared.hpp>
#include <threads.hpp>

bool service::invokeRequestCallbackEx(void* buffer, const size_t buffer_size) {
  KeAttachProcess(g_service);

  void* alloc = nullptr;
  size_t alloc_size = buffer_size + sizeof(user_callback_t);
  if (!NT_SUCCESS(ZwAllocateVirtualMemory((HANDLE)-1, (void**)&alloc, 0,
                                          &alloc_size, MEM_COMMIT | MEM_RESERVE,
                                          PAGE_READWRITE))) {
    KeDetachProcess();
    return false;
  }

  memcpy(alloc, buffer, buffer_size);
  const bool result = threads::createUserThread(g_callback, alloc);

  KeDetachProcess();
  return result;
}