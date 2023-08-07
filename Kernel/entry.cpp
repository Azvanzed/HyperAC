#include <intrin.h>

#include <create_process_notify.hpp>
#include <create_thread_notify.hpp>
#include <ioctl.hpp>
#include <load_image_notify.hpp>
#include <ob_pre_operation.hpp>
#include <mmu.hpp>
#include <modules.hpp>

void driverUnload(DRIVER_OBJECT* drv) {
  UNREFERENCED_PARAMETER(drv);

  if (!ioctl::Unregister()) {
    print("failed to unregister Ioctl Callback");
  } else if (!create_process_notify::Unregister()) {
    print("failed to unregister CreateProcess Callback");
  } else if (!create_thread_notify::Unregister()) {
    print("failed to unregister CreateThread Callback");
  } else if (!load_image_notify::Unregister()) {
    print("failed to unregister LoadImage Callback");
  }

  ob_pre_operation::Unregister();
  mmu::Release();
}

NTSTATUS driverLoad(DRIVER_OBJECT* drv, UNICODE_STRING* reg) {
  UNREFERENCED_PARAMETER(reg);

  mmu::Init();

  drv->DriverUnload = &driverUnload;

  if (!ioctl::Register(drv)) {
    print("failed to register a Ioctl Handler");
    return STATUS_UNSUCCESSFUL;
  }

  return STATUS_SUCCESS;
}