#include <Windows.h>

#include <callbacks.hpp>
#include <files.hpp>
#include <game.hpp>
#include <iostream>
#include <files.hpp>

void callbacks::Dispatcher(user_callback_t* ctx) {

  switch (ctx->type) {
    case user_callback_type_e::image_loaded: {
      on_image_load_t* image = (on_image_load_t*)ctx;

      const std::wstring& dospath = files::toDosPath(image->path);
      wcscpy(image->path, dospath.data());

      if (image->process_id == game::g_process_id) {
        printf("%S: %i\n", dospath.data(), files::isSigned(dospath));

      }
      
      game::g_images.push_back(*image);
    } break;
    case user_callback_type_e::thread_created: {
      game::g_threads.push_back(*(on_thread_creation_t*)ctx);
    } break;
    case user_callback_type_e::process_created: {
      game::g_processes.push_back(*(on_process_creation_t*)ctx);
    } break;
    case user_callback_type_e::handle_request: {
      on_handle_request_t* handle = (on_handle_request_t*)ctx;

      game::g_handles.push_back(*handle);

    } break;
  }

  VirtualFree(ctx, 0, MEM_RELEASE);
}