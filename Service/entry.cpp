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


LONG NTAPI onRaisedException(EXCEPTION_POINTERS* info) {
  initialize_input_t input;
  initialize_output_t output;
  ioctl::sendDriver(IOCTL_HYPERAC_UNINITIALIZE, input, &output);
  scm::Destroy(L"HyperAC");

  exit(0);
}

int main(int, char** argv) {
  AddVectoredExceptionHandler(1, &onRaisedException);

  initialize_input_t input;
  input.callback = &callbacks::Dispatcher;
  strcpy(input.game_name, "Crab Game.com");

  initialize_output_t output;
  ioctl::sendDriver(IOCTL_HYPERAC_INITIALIZE, input, &output);

  HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, game::g_process_id);

  /* detection thread */
  while (true) {
      Sleep(100);

      /* refresh images */
      for (size_t i = 0; i < game::g_images.size(); ++i) {
          on_image_load_t* image = &game::g_images[i];
          if (!processes::isDllLoaded(handle, image->base)) {
              game::g_images.erase(game::g_images.begin() + i);
          }
      }
  }

  CloseHandle(handle);
  return 0;
}