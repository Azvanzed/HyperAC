#include <load_image_notify.hpp>

void load_image_notify::Dispatcher(UNICODE_STRING* image_name, HANDLE pid,
                                   IMAGE_INFO* info) {
  UNREFERENCED_PARAMETER(image_name);
  UNREFERENCED_PARAMETER(pid);
  UNREFERENCED_PARAMETER(info);
}

bool load_image_notify::Register() {
  return NT_SUCCESS(PsSetLoadImageNotifyRoutine(&Dispatcher));
}
bool load_image_notify::Unregister() {
  return NT_SUCCESS(PsRemoveLoadImageNotifyRoutine(&Dispatcher));
}