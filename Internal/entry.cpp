#include <iostream>
#include <vector.hpp>
#include <integrity.hpp>
#include <string.hpp>
#include <intrin.h>
#include <service.hpp>

extern "C" void __CxxFrameHandler4() { __ud2(); }
extern "C" void __std_terminate() { __ud2(); }

ULONG __stdcall detectionsThread(void*) {
	while (true) {

		on_internal_heartbeat_t heartbeat_callback;
		heartbeat_callback.type = service_callback_type_e::internal_heartbeat;
		heartbeat_callback.xored_time = (10000 / 60) ^ HEARTBEAT_XOR_KEY;
		service::invokeRequestCallback(heartbeat_callback);


		uint64_t base = (uint64_t)API(GetModuleHandleA)(nullptr);

		integrity::integrity_t integrity;
		if (integrity::verifyModule(base, &integrity) == integrity::integrity_corrupted) {
			on_integrity_violation_t callback;
			callback.type = service_callback_type_e::integrity_violation;
			crt::memcpy(&callback.filepath, &integrity.filepath, MAX_PATH);
			callback.hash = integrity.hash;
			callback.file_hash = integrity.file_hash;
			callback.count = integrity.patches.getSize();

			size_t size = callback.count * sizeof(patch_t);
			callback.patches = (patch_t*)API(VirtualAllocEx)(g_service, nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			API(WriteProcessMemory)(g_service, callback.patches, integrity.patches.getStorage(), size, nullptr);

			service::invokeRequestCallback(callback);
		}

		API(Sleep)(100);
	}

	API(CloseHandle)(g_service);
}

#ifdef _DEBUG
bool DllMain(void*, uint32_t reason) {
	if (reason != 1) {
		return true;
	}

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#else
bool DllMain(dllmain_ctx_t* ctx) {
	API(AllocConsole)();

	g_service = API(OpenProcess)(PROCESS_ALL_ACCESS, false, ctx->process_id);
	g_service_callback = ctx->callback;
#endif

	HANDLE hthread = API(CreateThread)(nullptr, 0, &detectionsThread, nullptr, 0, nullptr);
	API(CloseHandle)(hthread);

	return true;
}

