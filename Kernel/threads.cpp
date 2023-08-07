#include <threads.hpp>
#include <docs.hpp>

bool threads::createUserThread(void* routine, void* parameter, const bool wait) {
  HANDLE hthread;
  if (!NT_SUCCESS(RtlCreateUserThread((HANDLE)-1, nullptr, false, 0, 0, 0,
                                      routine, parameter, &hthread, nullptr))) {
    return false;
  }

  if (wait) {
    const int64_t delay = 10000 * -10000;
    ZwWaitForSingleObject(hthread, false, (LARGE_INTEGER*)&delay);
  }

  ZwClose(hthread);
  return true;
}

uint64_t threads::getStartAddress(const PETHREAD thread) {
  return *(uint64_t*)((uint64_t)thread + 0x4d0);
}