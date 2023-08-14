#include <manual_mapper.hpp>
#include <ntimage.h>
#include <docs.hpp>
#include <threads.hpp>

uint64_t manual_mapper::inject(int32_t process_id, void* data) {
	PEPROCESS eprocess;
	if (!NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)process_id, &eprocess))) {
		return 0;
	}

	PsAcquireProcessExitSynchronization(eprocess);
	KeAttachProcess(eprocess);

	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)data;
	if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
		KeDetachProcess();
		PsReleaseProcessExitSynchronization(eprocess);
		ObfDereferenceObject(eprocess);
		return 0;
	}

	IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)((uint64_t)data + dos->e_lfanew);

	void* alloc = nullptr;
	size_t size = nt->OptionalHeader.SizeOfImage;

	if (!NT_SUCCESS(ZwAllocateVirtualMemory((HANDLE)-1, &alloc, 0, &size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE))) {
		KeDetachProcess();
		PsReleaseProcessExitSynchronization(eprocess);
		ObfDereferenceObject(eprocess);
		return 0;
	}

	uint64_t base = (uint64_t)alloc;

	IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(nt);
	for (uint16_t i = 0; i < nt->FileHeader.NumberOfSections; ++i, ++section) {
		uint64_t dst = base + section->VirtualAddress;
		uint64_t src = (uint64_t)data + section->PointerToRawData;

		memcpy((void*)dst, (void*)src, section->SizeOfRawData);
	}

	KeDetachProcess();
	PsReleaseProcessExitSynchronization(eprocess);
	ObfDereferenceObject(eprocess);
	return base + nt->OptionalHeader.AddressOfEntryPoint;
}