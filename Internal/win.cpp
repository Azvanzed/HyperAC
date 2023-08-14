#include <docs.hpp>
#include <win.hpp>
#include <intrin.h>

#include <crt.hpp>

uint64_t win::findModule(const wchar_t* name) {
    PEB* peb = (PEB*)__readgsqword(0x60);
    LIST_ENTRY* head = &peb->Ldr->InMemoryOrderModuleList;

    for (LIST_ENTRY* curr = head->Flink; curr->Flink != head; curr = curr->Flink) {
        FULL_LDR_DATA_TABLE_ENTRY* entry =
            CONTAINS_RECORD(curr, FULL_LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
        if (!crt::wcsicmp(entry->BaseDllName.Buffer, name)) {
            return (uint64_t)entry->DllBase;
        }
    }

    return 0;
}

uint64_t win::findExport(uint64_t base, const char* name) {
    IMAGE_NT_HEADERS* nt = NT_HEADERS(base);

    uint32_t va = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    if (!va) {
        return 0;
    }

    IMAGE_EXPORT_DIRECTORY* eat = (IMAGE_EXPORT_DIRECTORY*)(base + va);
    uint32_t* rvas = (uint32_t*)(base + eat->AddressOfFunctions);
    uint32_t* names = (uint32_t*)(base + eat->AddressOfNames);
    uint16_t* ords = (uint16_t*)(base + eat->AddressOfNameOrdinals);

    for (uint32_t i = 0; i < eat->NumberOfNames; ++i) {
        if (!crt::stricmp((char*)(base + names[i]), name)) {
            return base + rvas[ords[i]];
        }
    }

    return 0;
}

uint64_t win::findExport(const char* name) {
    PEB* peb = (PEB*)__readgsqword(0x60);
    LIST_ENTRY* head = &peb->Ldr->InMemoryOrderModuleList;

    for (LIST_ENTRY* curr = head->Flink; curr->Flink != head; curr = curr->Flink) {
        LDR_DATA_TABLE_ENTRY* entry =
            CONTAINS_RECORD(curr, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
        uint64_t address = findExport((uint64_t)entry->DllBase, name);
        if (address) {
            return address;
        }
    }

    return 0;
}

