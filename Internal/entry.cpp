#include <iostream>
#include <vector.hpp>
#include <integrity.hpp>
#include <string.hpp>
#include <intrin.h>


extern "C" void __CxxFrameHandler4() { __ud2(); }
extern "C" void __std_terminate() { __ud2(); }

ULONG __stdcall detectionsThread(void*) {
	while (true) {
		uint64_t base = (uint64_t)API(GetModuleHandleA)(nullptr);

		integrity::integrity_t integrity;
		if (integrity::verifyModule(base, &integrity) == integrity::integrity_corrupted) {
			API(MessageBoxW)(0, string::toString(integrity.patches.getSize()).getData(), 0, 0);
		}

		API(Sleep)(100);
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

	
	API(CreateThread)(nullptr, 0, &detectionsThread, nullptr, 0, nullptr);
	return true;
}

