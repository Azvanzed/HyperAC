#include <ioctl.hpp>

bool ioctl::sendDriverEx(const uint32_t code, void* input,
                         const size_t input_size, void* output,
                         const size_t output_size) {
  memset(output, 0, output_size);

  constexpr uint32_t access = GENERIC_READ | GENERIC_WRITE;
  HANDLE device = CreateFileA("\\\\.\\HyperAC", access, 0, nullptr,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (!device || device == INVALID_HANDLE_VALUE) {
    return false;
  }

  DWORD bytes = 0;
  const BOOL result = DeviceIoControl(device, code, input, input_size, output,
                                      output_size, &bytes, nullptr);

  CloseHandle(device);
  return result && bytes == output_size;
}