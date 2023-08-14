#include <integrity.hpp>
#include <intrin.h>

integrity::integrity_status_e integrity::verifyModuleIntegrity(uint64_t base, modified_byte_t** modified_bytes, uint32_t* modified_count) {
    wchar_t filepath[MAX_PATH];
    if (!API(GetModuleFileNameW)((HMODULE)base, filepath, MAX_PATH)) {
        return integrity_failed;
    }

    HANDLE handle = API(CreateFileW)(filepath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (handle == INVALID_HANDLE_VALUE) {
        return integrity_failed;
    }

    uint32_t size = API(GetFileSize)(handle, nullptr);
    if (size == INVALID_FILE_SIZE) {
        API(CloseHandle)(handle);
        return integrity_failed;
    }

    ULONG bytes_read;
    uint8_t* buffer = mmu::Allocate<uint8_t*>(size);
    if (!API(ReadFile)(handle, buffer, size, &bytes_read, nullptr) || bytes_read != size) {
        mmu::Free(buffer);
        API(CloseHandle)(handle);
        return integrity_failed;
    }

    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)buffer;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
        mmu::Free(buffer);
        API(CloseHandle)(handle);
        return integrity_failed;
    }

    IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(buffer + dos->e_lfanew);

    const auto find_patched_bytes = [&](modified_byte_t* bytes) {
        uint32_t count = 0;
        IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(nt);
        for (uint16_t i = 0; i < nt->FileHeader.NumberOfSections; ++i, ++section) {
            if ((section->Characteristics & IMAGE_SCN_MEM_WRITE) || !(section->Characteristics & IMAGE_SCN_MEM_EXECUTE)) {
                continue;
            }

            for (uint32_t x = 0; x < section->SizeOfRawData; ++x) {
                if (*(uint8_t*)(base + section->VirtualAddress + x) != *(uint8_t*)(buffer + section->PointerToRawData + x)) {
                    if (bytes) {
                        modified_byte_t* modification = &bytes[count];
                        modification->offset = x;
                        modification->patch = *(uint8_t*)(base + section->VirtualAddress + x);
                        crt::memcpy(modification->name, (char*)section->Name, sizeof(IMAGE_SECTION_HEADER::Name));
                    }

                    ++count;
                }
            }
        }

        return count;
    };

    uint32_t count = find_patched_bytes(nullptr);
    if (count) {
        *modified_bytes = mmu::Allocate<modified_byte_t*>(count * sizeof(modified_byte_t));
        *modified_count = count;
        find_patched_bytes(*modified_bytes);

        mmu::Free(buffer);
        API(CloseHandle)(handle);
        return integrity_corrupted;
    }

    mmu::Free(buffer);
    API(CloseHandle)(handle);
    return integrity_valid;
}
