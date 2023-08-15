#include <iostream>
#include <vector.hpp>
#include <integrity.hpp>

ULONG __stdcall detectionsThread(void*) {
	while (true) {
		uint64_t base = (uint64_t)GetModuleHandle(nullptr);

		integrity::integrity_t integrity;
		if (integrity::verifyModule(base, &integrity) == integrity::integrity_corrupted) {
			for (size_t i = 0; i < integrity.patches.getSize(); ++i) {
				printf("[%i] 0x%x => 0x%x\n", i, integrity.patches.getStorage()[i].offset, integrity.patches.getStorage()[i].value);
			}
		}

		printf("%i\n", integrity.patches.getSize());
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

	CreateThread(nullptr, 0, &detectionsThread, nullptr, 0, nullptr);
	return true;
}

