#include <process.hpp>

bool process::Create(const wchar_t* filepath, const wchar_t* args,
                     PROCESS_INFORMATION* pi) {
  STARTUPINFO si;
  memset(&si, 0, sizeof(STARTUPINFO));

  si.cb = sizeof(STARTUPINFO);

  if (!CreateProcess(filepath, (LPWSTR)args, nullptr, nullptr, false, 0,
                     nullptr, L"", &si, pi)) {
    return false;
  }

  CloseHandle(pi->hProcess);
  CloseHandle(pi->hThread);
  return true;
}