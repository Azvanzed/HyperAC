#include <Windows.h>

#include <callbacks.hpp>
#include <files.hpp>
#include <game.hpp>
#include <iostream>
#include <files.hpp>

#include <scm.hpp>
#include <ioctl.hpp>

void callbacks::Dispatcher(user_callback_t* ctx) {
  switch (ctx->type) {
    case user_callback_type_e::image_loaded: {
      on_image_load_t* image = (on_image_load_t*)ctx;

      const std::wstring& dospath = files::toDosPath(image->path);
      wcscpy(image->path, dospath.data());

      printf("%S: %i\n", dospath.data(), files::isSigned(dospath));
      
      game::g_images.push_back(*image);
    } break;
    case user_callback_type_e::thread_created: {
      on_thread_creation_t* thread = (on_thread_creation_t*)ctx;
      printf("start: 0x%llx\n", thread->start);
      game::g_threads.push_back(*thread);
    } break;
    case user_callback_type_e::game_process: {
        on_game_process_t* game = (on_game_process_t*)ctx;

        if (!game->created) {
            VEH_TRIGGER();
        }

        game::g_process_id = game->process_id;
    } break;
    case user_callback_type_e::handle_request: {
      on_handle_request_t* handle = (on_handle_request_t*)ctx;

      game::g_handles.push_back(*handle);
    } break;
  }

  VirtualFree(ctx, 0, MEM_RELEASE);
}