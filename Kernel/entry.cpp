#include <ob_pre_operation.hpp>
#include <load_image_notify.hpp>
#include <create_process_notify.hpp>
#include <create_thread_notify.hpp>

void driverUnload(DRIVER_OBJECT* drv) {
  UNREFERENCED_PARAMETER(drv);

  ob_pre_operation::Unregister();
  create_process_notify::Unregister();
  create_thread_notify::Unregister();
  load_image_notify::Unregister();
}

NTSTATUS driverLoad(DRIVER_OBJECT* drv, UNICODE_STRING* reg) {
  UNREFERENCED_PARAMETER(reg);

  drv->DriverUnload = &driverUnload;

  if (!ob_pre_operation::Register()) {
    print("failed to register a ObPreOperation Callback");
  } else if (!create_process_notify::Register()) {
    print("failed to register a CreateProcess Callback");
  } else if (!create_thread_notify::Register()) {
    print("failed to register a CreateThread Callback");
  } else if (!load_image_notify::Register()) {
    print("failed to register a LoadImage Callback");
  }

  return STATUS_SUCCESS;
}