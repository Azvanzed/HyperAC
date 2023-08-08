#include <files.hpp>
#include <Windows.h>
#include <filesystem>
#include <WinTrust.h>
#include <softpub.h>

std::wstring files::toDosPath(const std::wstring& devicepath) {
  wchar_t drives[256];
  if (!GetLogicalDriveStringsW(sizeof(drives) / sizeof(wchar_t) - 1, drives)) {
    return L"";
  }

  for (wchar_t* drive = drives; *drive; drive += wcslen(drive) + 1) {
    wchar_t target[256];
    drive[2] = L'\0';
    if (QueryDosDeviceW(drive, target, sizeof(target) / sizeof(wchar_t)) != 0) {
      const size_t length = wcslen(target);
      if (devicepath.compare(0, length, target) == 0) {
        const std::wstring dospath = drive + devicepath.substr(length);
        if (std::filesystem::exists(dospath)) {
          return dospath;
        }
      }
    }
  }

  return L"";
}

bool files::isSigned(const std::wstring& filepath) {
  WINTRUST_FILE_INFO filedata;
  memset(&filedata, 0, sizeof(WINTRUST_FILE_INFO));

  filedata.cbStruct = sizeof(WINTRUST_FILE_INFO);
  filedata.pcwszFilePath = filepath.data();

  WINTRUST_DATA wvt_data;
  memset(&wvt_data, 0, sizeof(WINTRUST_DATA));

  wvt_data.cbStruct = sizeof(WINTRUST_DATA);
  wvt_data.dwUIChoice = WTD_UI_NONE;
  wvt_data.fdwRevocationChecks = WTD_REVOKE_NONE;
  wvt_data.dwUnionChoice = WTD_CHOICE_FILE;
  wvt_data.dwProvFlags = WTD_SAFER_FLAG;
  wvt_data.pFile = &filedata;

  GUID policy = WINTRUST_ACTION_GENERIC_VERIFY_V2;
  return WinVerifyTrust((HWND)-1, &policy, &wvt_data) == ERROR_SUCCESS;
}