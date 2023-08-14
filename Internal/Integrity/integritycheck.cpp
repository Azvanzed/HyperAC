#include "integritycheck.hpp"
#include <Windows.h>

#include <MemOperations/mem.hpp>

#include <Global/global.hpp>

void integritycheck::VerifyModuleSectionIntegrity(const wchar_t* section_name)
{
	uintptr_t rva = 0;
	uintptr_t size = 0;
	mem::GetTextSectionInfo(rva, size);
	uintptr_t text_section_addr = global::game_base + rva;
	API(CreateFile)(0, 0, 0, 0, 0, 0, 0);
}
