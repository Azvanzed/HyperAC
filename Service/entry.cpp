#include <Windows.h>
#include <winternl.h>

#include <callbacks.hpp>
#include <cstdint>
#include <game.hpp>
#include <ioctl.hpp>
#include <iostream>
#include <string>
#include <scm.hpp>

LONG NTAPI onRaisedException(EXCEPTION_POINTERS* info) {
  initialize_input_t input;
  initialize_output_t output;
  ioctl::sendDriver(IOCTL_HYPERAC_UNINITIALIZE, input, &output);
  scm::Destroy(L"HyperAC");

  exit(0);
}

int main(int, char** argv) {
  AddVectoredExceptionHandler(1, &onRaisedException);

  game::g_process_id = atoi(argv[1]);

  initialize_input_t input;
  input.callback = &callbacks::Dispatcher;
  input.process_id = game::g_process_id;

  initialize_output_t output;
  ioctl::sendDriver(IOCTL_HYPERAC_INITIALIZE, input, &output);

  HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, game::g_process_id);

  /* detection thread */
  while (true) {
    auto images = game::g_images;
    auto threads = game::g_threads;

    for (const auto& thread : threads) {
        bool is_backed = false;
        for (const auto& image : images) {
            if (thread.start > image.base && thread.start < image.base + image.size) {
                MEMORY_BASIC_INFORMATION mbi;
                VirtualQueryEx(handle, (void*)thread.start, &mbi, sizeof(MEMORY_BASIC_INFORMATION));

                if (mbi.State & MEM_RELEASE || mbi.State & MEM_FREE/* || !(mbi.Protect & PAGE_EXECUTE) ||
                    !(mbi.Protect & PAGE_EXECUTE_READ) || !(mbi.Protect & PAGE_EXECUTE_READWRITE)*/) {
                    continue;
                }

                is_backed = true;
                break;
            }
        }

        if (!is_backed) {
            printf("illegal thread created! 0x%llx\n", thread.start);
        }

        Sleep(100);
    }
  }

  CloseHandle(handle);
  return 0;
}