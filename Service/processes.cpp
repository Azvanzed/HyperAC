#include <processes.hpp>
#include <Psapi.h>

bool processes::isDllLoaded(HANDLE handle, uint64_t base) {
    ULONG bytes_needed;
    HMODULE modules[1024];
    if (!EnumProcessModules(handle, modules, sizeof(modules), &bytes_needed)) {
        return false;
    }

    for (uint32_t i = 0; i < bytes_needed / sizeof(HMODULE); ++i) {
        if ((uint64_t)modules[i] == base) {
            return true;
        }
    }

    return false;
}