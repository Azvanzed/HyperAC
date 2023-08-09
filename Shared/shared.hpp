#pragma once

#include <cstdint>

#define IOCTL_HYPERAC_INITIALIZE \
  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x700, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_HYPERAC_UNINITIALIZE \
  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x710, METHOD_BUFFERED, FILE_ANY_ACCESS)

struct initialize_input_t {
  void* callback;
  uint64_t process_id;
};

struct initialize_output_t {

};

struct uninitialize_input_t {
};

struct uninitialize_output_t {};

enum class user_callback_type_e : uint8_t {
  none,
  image_loaded,
  thread_created,
  process_created,
  handle_request,
};

struct user_callback_t {
  user_callback_type_e type;
};

enum class handle_request_type_e : uint8_t {
	none,
	open_process,
	dup_process,
	open_thread,
	dup_thread,
};

struct on_handle_request_t : user_callback_t {
  handle_request_type_e request;
  uint64_t process_id;
  union {
    struct {
      uint64_t process_id;
    } process;
    struct {
      uint64_t process_id;
      uint64_t thread_id;
    } thread;
  }target;
  uint32_t access;
};

struct on_thread_creation_t : user_callback_t {
  bool create;
  uint64_t start;
  uint64_t process_id;

  struct {
    uint64_t thread_id;
    uint64_t process_id;
  }target;
};

struct on_process_creation_t : user_callback_t {
  bool created;
  uint64_t parent_id;
  uint64_t process_id;
};

struct on_image_load_t : user_callback_t {
  uint64_t base;
  uint64_t process_id;
  
  struct {
    uint64_t process_id;
  }target;
  wchar_t path[512];
};