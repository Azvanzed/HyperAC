#include <Windows.h>
#include <winternl.h>

#include <cstdint>
#include <iostream>
#include <process.hpp>
#include <scm.hpp>
#include <string>

int WinMain(HMODULE, HMODULE, LPSTR, int) {
  wchar_t driver_path[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, driver_path);
  wcscat(driver_path, L"/HyperAC/HyperAC.sys");

  scm::Destroy(L"HyperAC");
  
  if (!scm::Create(L"HyperAC", L"Hyper Anti-cheat service", driver_path,
                   SERVICE_KERNEL_DRIVER)) {
    return MessageBox(nullptr, L"failed to create service", L"HyperAC",
                      MB_OK | MB_ICONWARNING);
  } else if (!scm::Start(L"HyperAC")) {
    scm::Destroy(L"HyperAC");
    return MessageBox(nullptr, L"failed to start service", L"HyperAC",
                      MB_OK | MB_ICONWARNING);
  }

  PROCESS_INFORMATION info;
  memset(&info, 0, sizeof(info));

  if (!process::Create(L"Crab Game.com", L"", &info)) {
    scm::Destroy(L"HyperAC");
    return MessageBox(nullptr, L"failed to start game", L"HyperAC",
                      MB_OK | MB_ICONWARNING);
  }

  CloseHandle(info.hProcess);
  CloseHandle(info.hThread);

  if (!process::Create(L"HyperAC/HyperAC.exe",
                       std::to_wstring(info.dwProcessId), &info)) {
    scm::Destroy(L"HyperAC");
    return MessageBox(nullptr, L"failed to start hyper", L"HyperAC",
                      MB_OK | MB_ICONWARNING);
  }

  CloseHandle(info.hProcess);
  CloseHandle(info.hThread);
  return 0;
}