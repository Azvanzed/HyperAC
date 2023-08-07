#include <ntifs.h>

#include <docs.hpp>
#include <load_image_notify.hpp>
#include <threads.hpp>
#include <service.hpp>

void load_image_notify::Dispatcher(UNICODE_STRING* image_name, HANDLE pid,
                                   IMAGE_INFO* info) {
  UNREFERENCED_PARAMETER(image_name);
  UNREFERENCED_PARAMETER(info);

  if (pid == g_game_pid) {

  }
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