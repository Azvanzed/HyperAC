#pragma once

#include <cstdint>

#define IOCTL_HYPERAC_HEARTBEAT \
  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HYPERAC_INITIALIZE \
  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x700, METHOD_BUFFERED, FILE_ANY_ACCESS)


struct initialize_input_t {
  char game_name[15];
  char service_name[15];
  
  void* request_callback;
};

struct initialize_output {};

struct heartbeat_input_t {};

struct heartbeat_output_t {};

enum report_type_e : uint8_t {
	none,
	suspicious_user_thread,
};

struct suspicious_user_thread_t {
  uint64_t start_address;
  uint64_t backer;
  wchar_t name[64];
};

struct report_t {
  struct report_t* next;
  report_type_e type;
  uint16_t size;
  void* data;
};

struct usermode_callback_t {
  void* input;
  size_t input_size;
  void* output;
  size_t output_size;
};