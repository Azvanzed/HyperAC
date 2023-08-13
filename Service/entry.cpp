#include <Windows.h>
#include <winternl.h>

#include <callbacks.hpp>
#include <cstdint>
#include <game.hpp>
#include <ioctl.hpp>
#include <iostream>
#include <string>
#include <scm.hpp>

LONG NTAPI onRaisedException(EXCEPTION_POINTERS* info) {
  initialize_input_t input;
  initialize_output_t output;
  ioctl::sendDriver(IOCTL_HYPERAC_UNINITIALIZE, input, &output);
  scm::Destroy(L"HyperAC");

  exit(0);
}





int main(int, char** argv) {
  AddVectoredExceptionHandler(1, &onRaisedException);

  game::g_process_id = atoi(argv[1]);

  initialize_input_t input;
  input.callback = &callbacks::Dispatcher;
  input.process_id = game::g_process_id;

  initialize_output_t output;
  ioctl::sendDriver(IOCTL_HYPERAC_INITIALIZE, input, &output);

  /* detection thread */
  while (true) {
    auto images = game::g_processes;
    auto threads = game::g_processes;

  }

  return 0;
}