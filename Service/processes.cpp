#include <processes.hpp>
#include <TlHelp32.h>
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

int32_t processes::findProcessId(const std::wstring& name) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(processEntry);

    Process32First(snapshot, &processEntry);
    do {
        if (!name.compare(processEntry.szExeFile)) {
            CloseHandle(snapshot);
            return processEntry.th32ProcessID;
        }
    } while (Process32Next(snapshot, &processEntry));

    CloseHandle(snapshot);   
    return 0;
}