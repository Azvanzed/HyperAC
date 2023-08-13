#include <Windows.h>
#include <winternl.h>

#include <cstdint>
#include <iostream>
#include <process.hpp>
#include <scm.hpp>
#include <string>
#include <filesystem>

int WinMain(HMODULE, HMODULE, LPSTR, int) {
    std::wstring current_path = std::filesystem::current_path().wstring();
    std::wstring driver_path = current_path + L"\\HyperAC\\HyperAC.sys";
    std::wstring game_path = current_path + L"\\Crab Game.com";
    std::wstring service_path = current_path + L"\\HyperAC\\HyperAC.exe";

    scm::Destroy(L"HyperAC");
    if (!scm::Create(L"HyperAC", L"Hyper Anti-cheat service", driver_path, SERVICE_KERNEL_DRIVER)) {
        return MessageBox(nullptr, L"failed to create service", L"HyperAC", MB_OK | MB_ICONWARNING);
    } else if (!scm::Start(L"HyperAC")) {
        scm::Destroy(L"HyperAC");
        return MessageBox(nullptr, L"failed to start service", L"HyperAC", MB_OK | MB_ICONWARNING);
    }

    PROCESS_INFORMATION info;
    memset(&info, 0, sizeof(PROCESS_INFORMATION));

    if (!process::Create(service_path, L"", &info)) {
        scm::Destroy(L"HyperAC");
        return MessageBox(nullptr, L"failed to start hyperac", L"HyperAC", MB_OK | MB_ICONWARNING);
    }

    CloseHandle(info.hProcess);
    CloseHandle(info.hThread);

    Sleep(2000);

    if (!process::Create(game_path, L"", &info)) {
        scm::Destroy(L"HyperAC");
        return MessageBox(nullptr, L"failed to start game", L"HyperAC", MB_OK | MB_ICONWARNING);
    }

    CloseHandle(info.hProcess);
    CloseHandle(info.hThread);
    return 0;
}