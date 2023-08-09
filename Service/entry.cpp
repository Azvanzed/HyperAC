#include <Windows.h>
#include <winternl.h>

#include <callbacks.hpp>
#include <cstdint>
#include <game.hpp>
#include <ioctl.hpp>
#include <iostream>
#include <string>



int main(int, char** argv) {
  game::g_process_id = atoi(argv[1]);

  initialize_input_t input;
  input.callback = &callbacks::Dispatcher;
  input.process_id = game::g_process_id;

  initialize_output_t output;
  ioctl::sendDriver(IOCTL_HYPERAC_INITIALIZE, input, &output);

  Sleep(-1);
  return 0;
}