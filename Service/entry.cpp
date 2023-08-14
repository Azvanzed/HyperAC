#include <Windows.h>
#include <winternl.h>

#include <callbacks.hpp>
#include <cstdint>
#include <game.hpp>
#include <ioctl.hpp>
#include <iostream>
#include <string>
#include <scm.hpp>
#include <processes.hpp>
#include <fstream>

LONG NTAPI onRaisedException(EXCEPTION_POINTERS* info) {
  initialize_input_t input;
  initialize_output_t output;
  ioctl::sendDriver(IOCTL_HYPERAC_UNINITIALIZE, &input, &output);
  scm::Destroy(L"HyperAC");

  exit(0);
}


void readFile(const std::string& filepath, std::vector<uint8_t>* buffer) {
	std::ifstream stream(filepath, std::ios::binary);
	buffer->assign((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	stream.close();
}


int main(int, char** argv) {
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

	  std::vector<uint8_t> buffer;
	  readFile("HyperAC\\HyperAC.dll", &buffer);

	  printf("HyperAC.dll: %.2fkb\n", buffer.size() / 1024.00f);

	  size_t input_size = sizeof(manual_map_input_t) + buffer.size() - 8;
	  manual_map_input_t* input = (manual_map_input_t*)malloc(input_size);
	  input->process_id = game::g_process_id;
	  memcpy(&input->data, buffer.data(), buffer.size());

	  manual_map_output_t output;
	  ioctl::sendDriverEx(IOCTL_HYPERAC_MANUAL_MAP, input, input_size, &output, sizeof(manual_map_output_t));

	  HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, game::g_process_id);
	  printf("HyperAC.dll ep at: 0x%llx\n", output.ep);

	  CreateRemoteThread(handle, nullptr, 0, (LPTHREAD_START_ROUTINE)output.ep, &callbacks::Dispatcher, 0, nullptr);

	  CloseHandle(handle);


	  free(input);
  }
  while (true);

  return 0;
}