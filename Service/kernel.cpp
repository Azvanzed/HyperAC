#include <kernel.hpp>
#include <ioctl.hpp>

bool kernel::isAlive() {
	kernel_heartbeat_input_t input;
	input.value = __rdtsc();

	kernel_heartbeat_output_t output;
	if (!ioctl::sendDriver(IOCTL_HYPERAC_DRIVER_HEARTBEAT, &input, &output)) {
		return false;
	}

	return output.hashed == (input.value ^ HEARTBEAT_XOR_KEY);
}

bool kernel::Init(void* callback, const char* game_name) {
	initialize_input_t input;
	input.callback = callback;
	strcpy(input.game_name, game_name);

	initialize_output_t output;
	return ioctl::sendDriver(IOCTL_HYPERAC_INITIALIZE, &input, &output);
}

bool kernel::Inject(uint64_t process_id, void* callback, const std::vector<uint8_t>& binary) {
	size_t input_size = sizeof(manual_map_input_t) + binary.size() - 8;
	manual_map_input_t* input = (manual_map_input_t*)malloc(input_size);
	input->process_id = process_id;
	memcpy(&input->data, binary.data(), binary.size());

	manual_map_output_t output;
	if (!ioctl::sendDriverEx(IOCTL_HYPERAC_MANUAL_MAP, input, input_size, &output, sizeof(manual_map_output_t))) {
		free(input);
		return false;
	}

	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, process_id);
	if (handle == INVALID_HANDLE_VALUE) {
		free(input); 
		return false;
	}

	void* alloc = VirtualAllocEx(handle, nullptr, sizeof(dllmain_ctx_t), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!alloc) {
		CloseHandle(handle);
		free(input);
		return false;
	}

	dllmain_ctx_t ctx;
	ctx.process_id = GetCurrentProcessId();
	ctx.callback = callback;
	if (!WriteProcessMemory(handle, alloc, &ctx, sizeof(dllmain_ctx_t), nullptr)) {
		VirtualFree(alloc, 0, MEM_RELEASE);
		CloseHandle(handle);
		free(input);
		return false;
	}

	HANDLE hthread = CreateRemoteThread(handle, nullptr, 0, (LPTHREAD_START_ROUTINE)output.ep, alloc, 0, nullptr);
	if (!hthread) {
		VirtualFree(alloc, 0, MEM_RELEASE);
		CloseHandle(handle);
		free(input);
		return false;
	}

	WaitForSingleObject(hthread, INFINITE);

	CloseHandle(hthread);
	CloseHandle(handle);
	free(input);
	return true;
}

bool kernel::Unit() {
	initialize_input_t input;
	initialize_output_t output;
	return ioctl::sendDriver(IOCTL_HYPERAC_UNINITIALIZE, &input, &output);
}