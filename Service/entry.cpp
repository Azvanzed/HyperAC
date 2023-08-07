#include <Windows.h>
#include <winternl.h>

#include <callbacks.hpp>
#include <cstdint>
#include <game.hpp>
#include <ioctl.hpp>
#include <iostream>
#include <string>
#include <TlHelp32.h>


int main() {
  initialize_input_t input;
  input.callback = &callbacks::Dispatcher;

  initialize_output output;
  ioctl::sendDriver(IOCTL_HYPERAC_INITIALIZE, input, &output);

  STARTUPINFO startup;
  memset(&startup, 0, sizeof(STARTUPINFO));

  startup.cb = sizeof(STARTUPINFO);

  PROCESS_INFORMATION info;
  CreateProcess(L"C:\\Windows\\System32\\notepad.exe", nullptr, nullptr,
                nullptr, false, 0, nullptr, nullptr, &startup, &info);

  game::g_process_id = info.dwProcessId;

  CloseHandle(info.hProcess);
  CloseHandle(info.hThread);

  while (true) {
    Sleep(100);
  }

  return 0;
}