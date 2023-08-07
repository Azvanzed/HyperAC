#include <ntimage.h>

#include <docs.hpp>
#include <mmu.hpp>
#include <modules.hpp>

uint64_t modules::findKernelBase(const char* name) {
  ULONG size = 0;
  ZwQuerySystemInformation(SystemModuleInformation, nullptr, 0, &size);

  const SYSTEM_MODULES* modules = mmu::Allocate<SYSTEM_MODULES*>(pool, size);
  if (!NT_SUCCESS(ZwQuerySystemInformation(SystemModuleInformation,
                                           (void*)modules, size, &size))) {
    mmu::Free(modules);
    return 0;
  }

  for (size_t i = 0; i < modules->Count; ++i) {
    const SYSTEM_MODULE* module = &modules->Modules[i];
    if (strstr((char*)&module->Name[module->NameOffset], name)) {
      const uint64_t base = (uint64_t)module->Base;
      mmu::Free(modules);
      return base;
    }
  }

  mmu::Free(modules);
  return 0;
}

uint64_t modules::findUserBase(const PEPROCESS process, const wchar_t* name) {
  KeAttachProcess(process);

  const PEB* peb = PsGetProcessPeb(process);
  for (LIST_ENTRY* curr = peb->Ldr->InLoadOrderModuleList.Flink;
       curr != &peb->Ldr->InLoadOrderModuleList; curr = curr->Flink) {
    const LDR_DATA_TABLE_ENTRY* entry =
        CONTAINING_RECORD(curr, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
    if (wcsstr(entry->BaseDllName.Buffer, name)) {
      const uint64_t base = (uint64_t)entry->DllBase;
      KeDetachProcess();
      return base;
    }
  }

  KeDetachProcess();
  return 0;
}

uint64_t modules::findUserBase(const PEPROCESS process, const uint64_t address,
                               const uint32_t flags) {
  KeAttachProcess(process);

  const PEB* peb = PsGetProcessPeb(process);
  for (LIST_ENTRY* curr = peb->Ldr->InLoadOrderModuleList.Flink;
       curr != &peb->Ldr->InLoadOrderModuleList; curr = curr->Flink) {
    const LDR_DATA_TABLE_ENTRY* entry =
        CONTAINING_RECORD(curr, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

    const uint64_t base = (uint64_t)entry->DllBase;
    if (isBacked(base, address, flags)) {
      KeDetachProcess();
      return base;
    }
  }

  KeDetachProcess();
  return 0;
}

bool modules::isBacked(const uint64_t base, const uint64_t address,
                       const uint32_t flags) {
  const IMAGE_NT_HEADERS* nt = RtlImageNtHeader(base);

  IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(nt);
  for (uint16_t i = 0; i < nt->FileHeader.NumberOfSections; ++i, ++section) {
    if (!(section->Characteristics & flags)) {
      continue;
    }

    const uint64_t start = base + section->PointerToRawData;
    const uint64_t end = start + section->SizeOfRawData;
    if (address >= start && address <= end) {
      return true;
    }
  }

  return false;
}

bool modules::findUserName(const PEPROCESS process, const uint64_t base, wchar_t* name) {
  KeAttachProcess(process);

  const PEB* peb = PsGetProcessPeb(process);
  for (LIST_ENTRY* curr = peb->Ldr->InLoadOrderModuleList.Flink;
       curr != &peb->Ldr->InLoadOrderModuleList; curr = curr->Flink) {
    const LDR_DATA_TABLE_ENTRY* entry =
        CONTAINING_RECORD(curr, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

    if ((uint64_t)entry->DllBase == base) {
      wcscpy(name, entry->BaseDllName.Buffer);
      KeDetachProcess();
      return true;
    }
  }

  KeDetachProcess();
  return false;
}