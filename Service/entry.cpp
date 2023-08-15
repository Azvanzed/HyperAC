
#include <callbacks.hpp>
#include <cstdint>
#include <game.hpp>
#include <ioctl.hpp>
#include <iostream>
#include <string>
#include <scm.hpp>
#include <processes.hpp>
#include <files.hpp>

#include <server.hpp>
#include <packets.hpp>


LONG NTAPI onRaisedException(EXCEPTION_POINTERS* info) {
	initialize_input_t input;
	initialize_output_t output;
	ioctl::sendDriver(IOCTL_HYPERAC_UNINITIALIZE, &input, &output);
	scm::Destroy(L"HyperAC");

	exit(0);
}

int main() {
	//packet_heartbeat_input_t heartbeat = { sizeof(packet_heartbeat_input_t) };
	//heartbeat.type = packet_type_e::heartbeat;
	//heartbeat.status.dll = false;
	//heartbeat.status.sys = true;
	//server::sendServer(&heartbeat);

	printf("streaming shellcode...\n");
	packet_shellcode_input_t shellcode = { sizeof(packet_shellcode_input_t) };
	shellcode.type = packet_type_e::shellcode;
	shellcode.shellcode = shellcode_type_e::main;

	packet_shellcode_output_t* output = nullptr;
	server::sendServer(&shellcode, (response_header_t**)&output);
	printf("shellcode streamed: %.2fkb\n", output->length / 1024.00f);

	std::vector<uint8_t> buffer;
	for (uint16_t i = 0; i < output->length; ++i) {
		buffer.push_back(output->data[i]);
	}


	AddVectoredExceptionHandler(1, &onRaisedException);

	// initialize driver
	{
		initialize_input_t input;
		input.callback = &callbacks::Dispatcher;
		strcpy(input.game_name, "Crab Game.com");

		initialize_output_t output;
		ioctl::sendDriver(IOCTL_HYPERAC_INITIALIZE, &input, &output);
	}

	while (!game::g_process_id) {
		printf("waiting for game...\n");
		Sleep(700);
	}

	Sleep(2000);

	{
		printf("loading HyperAC.dll\n");

		//std::vector<uint8_t> buffer;
		//files::Read("HyperAC\\HyperAC.dll", &buffer);

		printf("HyperAC.dll: %.2fkb\n", buffer.size() / 1024.00f);

		size_t input_size = sizeof(manual_map_input_t) + buffer.size() - 8;
		manual_map_input_t* input = (manual_map_input_t*)malloc(input_size);
		input->process_id = game::g_process_id;
		memcpy(&input->data, buffer.data(), buffer.size());

		manual_map_output_t output;
		ioctl::sendDriverEx(IOCTL_HYPERAC_MANUAL_MAP, input, input_size, &output, sizeof(manual_map_output_t));

		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, game::g_process_id);
		printf("HyperAC.dll ep at: 0x%llx\n", output.ep);

		void* alloc = VirtualAllocEx(handle, nullptr, sizeof(dllmain_ctx_t), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		dllmain_ctx_t ctx = { GetCurrentProcessId(), &callbacks::Dispatcher };
		WriteProcessMemory(handle, alloc, &ctx, sizeof(dllmain_ctx_t), nullptr);

		CreateRemoteThread(handle, nullptr, 0, (LPTHREAD_START_ROUTINE)output.ep, alloc, 0, nullptr);
		CloseHandle(handle);

		free(input);
	}
	while (true);

	return 0;
}