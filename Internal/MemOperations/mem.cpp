#include "mem.hpp"
#include <Windows.h>
#include <Global/global.hpp>
#include <crt.hpp>
void mem::GetTextSectionInfo(uintptr_t& rva,uintptr_t& size )
{
	global::UpdateGameStaticAddresses();
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)global::game_base;
	PIMAGE_NT_HEADERS imageNTHeaders = (PIMAGE_NT_HEADERS)((BYTE*)dosHeader + dosHeader->e_lfanew);
	PIMAGE_SECTION_HEADER sectionHeader = (PIMAGE_SECTION_HEADER)(imageNTHeaders + 1);
	for (int i = 0; i < imageNTHeaders->FileHeader.NumberOfSections; i++, sectionHeader++)
	{
		if (crt::memcmp(sectionHeader->Name, ".text", 5) == 0)
		{
			rva = sectionHeader->VirtualAddress;
			size = sectionHeader->SizeOfRawData;
			return;
		}
	}
}