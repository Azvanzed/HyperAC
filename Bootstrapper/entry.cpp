#include <Windows.h>
#include <winternl.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <TlHelp32.h>

#include <process.hpp>

int main() {
  PROCESS_INFORMATION game_info;
  process::Create(L"C:\\Program Files (x86)\\Steam\\steamapps\\common\\Crab Game\\Crab Game.exe", L"", &game_info);

  std::cin.get();
  return 0;
}