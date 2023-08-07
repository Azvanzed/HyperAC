#include <Windows.h>
#include <winternl.h>

#include <cstdint>
#include <iostream>
#include <string>

#include <ioctl.hpp>
#include <callbacks.hpp>

int main() {
  initialize_input_t input;
  input.callback = &callbacks::Dispatcher;

  initialize_output output;
  printf("sent: %i\n",
         ioctl::sendDriver(IOCTL_HYPERAC_INITIALIZE, input, &output));

  STARTUPINFO startup;
  memset(&startup, 0, sizeof(STARTUPINFO));
  
  startup.cb = sizeof(STARTUPINFO);
  
  PROCESS_INFORMATION info;
  CreateProcess(L"C:\\Windows\\System32\\notepad.exe", nullptr, nullptr, nullptr, false,
                0, nullptr, nullptr, &startup, &info);

  CloseHandle(info.hProcess);
  CloseHandle(info.hThread);

  while (true) {
    Sleep(100);
  }

  return 0;
}