#include <threads.hpp>
#include <docs.hpp>

CLIENT_ID threads::getCid(const PETHREAD thread) {
  return *(CLIENT_ID*)((uint64_t)thread + 0x478);
}

bool threads::createUserThread(void* routine, void* param) {
  HANDLE hthread;
  if (!NT_SUCCESS(RtlCreateUserThread((HANDLE)-1, nullptr, false, 0, 0, 0,
                                      routine, param, &hthread, nullptr))) {
    return false;
  }

  ZwClose(hthread);
  return true;
}

uint64_t threads::getStartAddress(const PETHREAD thread) {
  return *(uint64_t*)((uint64_t)thread + 0x4d0);
}