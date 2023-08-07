#include <callbacks.hpp>
#include <iostream>
#include <Windows.h>

void callbacks::Dispatcher(user_callback_t* ctx) {
  switch (ctx->type) {
    case user_callback_type_e::image_loaded: {
      printf("image_loaded!\n");
    } break;
    case user_callback_type_e::thread_created: {
      printf("thread_created!\n");
    } break;
    case user_callback_type_e::process_created: {
      printf("process_created!\n");
    } break;
    case user_callback_type_e::handle_request: {
      printf("handle_request!\n");
    } break;
  }
}