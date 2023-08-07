#include <ntifs.h>

#include <docs.hpp>
#include <load_image_notify.hpp>
#include <threads.hpp>
#include <service.hpp>
#include <shared.hpp>
#include <modules.hpp>

void load_image_notify::Dispatcher(UNICODE_STRING* image_name, HANDLE pid,
                                   IMAGE_INFO* info) {
  on_image_load_t ctx;
  ctx.type = user_callback_type_e::image_loaded;
  ctx.process_id = (uint64_t)pid;
  ctx.base = (uint64_t)info->ImageBase;
  wcscpy(ctx.path, image_name->Buffer);

  service::invokeRequestCallback(ctx);
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