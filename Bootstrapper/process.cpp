#include <process.hpp>

bool process::Create(const std::wstring& filepath, const std::wstring& args,
                     PROCESS_INFORMATION* pi) {
  STARTUPINFO si;
  memset(&si, 0, sizeof(STARTUPINFO));

  si.cb = sizeof(STARTUPINFO);

  std::wstring cmd = L"\"" + filepath + L"\" " + args;
  return CreateProcess(nullptr, cmd.data(), nullptr, nullptr, false, 0, nullptr, nullptr, &si, pi);
}