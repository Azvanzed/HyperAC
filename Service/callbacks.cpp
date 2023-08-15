#include <Windows.h>

#include <callbacks.hpp>
#include <files.hpp>
#include <game.hpp>
#include <iostream>
#include <files.hpp>

#include <scm.hpp>
#include <ioctl.hpp>

void callbacks::Dispatcher(service_callback_t* ctx) {
    if (ctx->type == service_callback_type_e::integrity_violation) {
        printf("REQUEST: %i\n", ctx->type);
        Sleep(1000);

    }

    switch (ctx->type) {
    case service_callback_type_e::image_loaded: {
        on_image_load_t* image = (on_image_load_t*)ctx;

        const std::wstring& dospath = files::toDosPath(image->path);
        wcscpy(image->path, dospath.data());

        printf("%S: %i\n", dospath.data(), files::isSigned(dospath));
        
        game::g_images.push_back(*image);
    } break;
    case service_callback_type_e::thread_created: {
        on_thread_creation_t* thread = (on_thread_creation_t*)ctx;
        game::g_threads.push_back(*thread);
    } break;
    case service_callback_type_e::game_process: {
        on_game_process_t* game = (on_game_process_t*)ctx;

        if (!game->created) {
            Sleep(5000);
            VEH_TRIGGER();
        }

        game::g_process_id = game->process_id;
    } break;
    case service_callback_type_e::handle_request: {
        on_handle_request_t* handle = (on_handle_request_t*)ctx;

        game::g_handles.push_back(*handle);
    } break;
    case service_callback_type_e::integrity_violation: {
        printf("\n\n\nintegrity violation!\n\n\n");
    } break;
    }

    VirtualFree(ctx, 0, MEM_RELEASE);
}