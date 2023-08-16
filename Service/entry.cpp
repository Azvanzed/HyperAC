
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
#include <kernel.hpp>


LONG NTAPI onRaisedException(EXCEPTION_POINTERS* info) {
	printf("closing...\n");
	
	kernel::Unit();
	Sleep(5000);
	scm::Destroy(L"HyperAC");
	exit(0);
}

int main() {
	AddVectoredExceptionHandler(1, &onRaisedException);
	
	std::vector<uint8_t> buffer;
	if (!server::streamShellcode(shellcode_type_e::main, &buffer)) {
		return MessageBox(nullptr, L"Shellcode stream failed", nullptr, 0);
	}
	printf("shellcode streamed: %.2fkb\n", buffer.size() / 1024.00f);

	if (!kernel::Init(&callbacks::Dispatcher, "Crab Game.com")) {
		return MessageBox(nullptr, L"Kernel init failed", nullptr, 0);
	}

	while (!game::g_process_id) {
		printf("waiting for game...\n");
		Sleep(700);
	}

	Sleep(2000);

	if (!kernel::Inject(game::g_process_id, &callbacks::Dispatcher, buffer)) {
		return MessageBox(nullptr, L"Kernel inject shellcode", nullptr, 0); 
	}

	while (true) {
		Sleep(4000); // it will be a minute when we make it public

		bool is_kernel_alive = kernel::isAlive();
		bool is_internal_alive = (time(nullptr) - g_last_internal_tick) < 10;
		server::sendHeartbeat(is_internal_alive, is_kernel_alive);

	}

	return 0;
}