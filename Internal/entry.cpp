#include <iostream>
#include <integrity.hpp>


DWORD __stdcall Test(PVOID) {
	while (true) {

		uint32_t modified_count = 0;
		integrity::modified_byte_t* modified_bytes = nullptr;

		integrity::verifyModuleIntegrity((uint64_t)GetModuleHandleW(nullptr), &modified_bytes, &modified_count);

		system("cls");
		if (modified_bytes) {
			for (uint32_t i = 0; i < modified_count; ++i) {
				printf("[%i|%s] 0x%x. %02x\n", i, modified_bytes[i].name, modified_bytes[i].offset, modified_bytes[i].patch);
			}
			
			mmu::Free(modified_bytes);
		}

		Sleep(100);
	}
}


#ifdef _DEBUG
bool DllMain(void*, uint32_t reason) {
	if (reason != 1) {
		return true;
	}

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#else
bool DllMain(void* service_callback) {
	g_service_callback = service_callback;
#endif

	CreateThread(nullptr, 0, &Test, 0, 0, 0);
	
	return true;
}

