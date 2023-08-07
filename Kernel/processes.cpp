#include <processes.hpp>

HANDLE processes::getId(const PEPROCESS process) {
  return *(HANDLE*)((uint64_t)process + 0x440);
}