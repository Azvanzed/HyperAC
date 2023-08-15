#include <integrity.hpp>
#include <hash.hpp>
#include <intrin.h>

integrity::integrity_status_e integrity::verifyModule(uint64_t base, integrity_t* integrity) {
    if (!API(GetModuleFileNameA)((HMODULE)base, integrity->filepath, MAX_PATH)) {
        return integrity_failed;
    }

    HANDLE handle = API(CreateFileA)(integrity->filepath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (handle == INVALID_HANDLE_VALUE) {
        return integrity_failed;
    }

    ULONG bytes_read;
    uint32_t size = API(GetFileSize)(handle, nullptr);
    uint8_t* buffer = mmu::Allocate<uint8_t*>(size);
    if (!API(ReadFile)(handle, buffer, size, &bytes_read, nullptr) || bytes_read != size) {
        mmu::Free(buffer);
        API(CloseHandle)(handle);
        return integrity_failed;
    }

    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)buffer;
    IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(buffer + dos->e_lfanew);

    integrity->timestamp = nt->FileHeader.TimeDateStamp;
    integrity->hash = 0xffffffff;

    IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(nt);
    for (uint16_t i = 0; i < nt->FileHeader.NumberOfSections; ++i, ++section) {
        if ((section->Characteristics & IMAGE_SCN_MEM_WRITE) || !(section->Characteristics & IMAGE_SCN_MEM_EXECUTE)) {
            continue;
        }

        integrity->hash = hash::calcHash((uint64_t)(base + section->VirtualAddress), section->SizeOfRawData, integrity->hash);
        for (uint32_t x = 0; x < section->SizeOfRawData; ++x) {
            uint8_t va_byte = *(uint8_t*)(base + section->VirtualAddress + x);
            uint8_t raw_byte = *(uint8_t*)(buffer + section->PointerToRawData + x);
            if (va_byte == raw_byte) {
                continue;
            }

            integrity->patches.pushBack({ section->VirtualAddress + x, va_byte });
        }
    }

    mmu::Free(buffer);
    API(CloseHandle)(handle);
    return integrity->patches.isEmpty() ? integrity_valid : integrity_corrupted;
}
