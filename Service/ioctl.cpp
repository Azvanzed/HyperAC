#include <ioctl.hpp>

bool ioctl::sendDriverEx(uint32_t code, void* input, size_t input_size, void* output, size_t output_size) {
  memset(output, 0, output_size);

  constexpr uint32_t access = GENERIC_READ | GENERIC_WRITE;
  HANDLE device = CreateFileA("\\\\.\\HyperAC", access, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (!device || device == INVALID_HANDLE_VALUE) {
    return false;
  }

  DWORD bytes = 0;
  BOOL result = DeviceIoControl(device, code, input, input_size, output, output_size, &bytes, nullptr);

  CloseHandle(device);
  return result && bytes == output_size;
}