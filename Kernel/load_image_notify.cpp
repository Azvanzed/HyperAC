#include <ntifs.h>

#include <docs.hpp>
#include <load_image_notify.hpp>
#include <threads.hpp>
#include <service.hpp>
#include <shared.hpp>
#include <modules.hpp>

void load_image_notify::Dispatcher(UNICODE_STRING* image_name, HANDLE process_id, IMAGE_INFO* info) {
    if (g_game_pid != process_id) {
        return;
    }

    on_image_load_t callback;
    callback.type = service_callback_type_e::image_loaded;
    callback.parent_id = (uint64_t)PsGetCurrentProcessId();
    callback.process_id = (uint64_t)process_id;
    callback.base = (uint64_t)info->ImageBase;
    wcscpy(callback.path, image_name->Buffer);

    service::invokeRequestCallback(callback);
}

bool load_image_notify::Register() {
  if (!NT_SUCCESS(PsSetLoadImageNotifyRoutine(&Dispatcher))) {
    return false;
  }

  g_is_registered = true;
  return true;
}
bool load_image_notify::Unregister() {
  if (!g_is_registered) {
    return true;
  }

  g_is_registered = false;
  return NT_SUCCESS(PsRemoveLoadImageNotifyRoutine(&Dispatcher));
}