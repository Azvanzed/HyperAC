#include <Windows.h>
#include <winternl.h>

#include <cstdint>
#include <iostream>
#include <string>

#include <ioctl.hpp>
#include <shared.hpp>

bool requestCallback(usermode_callback_t* ctx) {
  printf("usermode called! [0x%llx] [%i]\n", ctx, *(int*)ctx->input);
  *(int*)ctx->output = 53; 
  return true;
}

int main() {
  initialize_input_t input;
  strcpy(input.game_name, "notepad.exe");
  strcpy(input.service_name, "HyperAC.exe");
  input.request_callback = &requestCallback;

  initialize_output output;
  printf("sent: %i\n",
         ioctl::sendDriver(IOCTL_HYPERAC_INITIALIZE, input, &output));

  STARTUPINFO startup;
  memset(&startup, 0, sizeof(STARTUPINFO));
  
  startup.cb = sizeof(STARTUPINFO);
  
  PROCESS_INFORMATION info;
  CreateProcess(L"C:\\Windows\\notepad.exe", nullptr, nullptr, nullptr, false,
                0, nullptr, nullptr, &startup, &info);

  CloseHandle(info.hProcess);
  CloseHandle(info.hThread);

  while (true) {
    printf("alive\n");
    Sleep(1000);
  }

  return 0;
}