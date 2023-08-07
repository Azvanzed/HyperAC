#include <intrin.h>

#include <ioctl.hpp>
#include <shared.hpp>
#include <service.hpp>

NTSTATUS ioctl::Dispatcher(DEVICE_OBJECT* drv, IRP* irp) {
  UNREFERENCED_PARAMETER(drv);

  NTSTATUS status = STATUS_UNSUCCESSFUL;
  uint64_t byteCount = 0;

  const IO_STACK_LOCATION* stack = IoGetCurrentIrpStackLocation(irp);
  switch (stack->Parameters.DeviceIoControl.IoControlCode) {
    case IOCTL_HYPERAC_INITIALIZE: {
      const initialize_input_t* input =
          (initialize_input_t*)irp->AssociatedIrp.SystemBuffer;

      strcpy(g_game_name, input->game_name);
      strcpy(g_service_name, input->service_name);
      g_request_callback = input->request_callback;

      byteCount = sizeof(initialize_output); 
      status = STATUS_SUCCESS;
    } break;
    case IOCTL_HYPERAC_HEARTBEAT: {
      byteCount = sizeof(heartbeat_output_t);
      status = STATUS_SUCCESS;
    } break;
  }

  irp->IoStatus.Status = status;
  irp->IoStatus.Information = byteCount;

  IofCompleteRequest(irp, IO_NO_INCREMENT);
  return status;
}

bool ioctl::Unregister() {
  if (!g_drv_obj) {
    return true;
  }

  UNICODE_STRING symbolic_link = RTL_CONSTANT_STRING(SYMBOLIC_LINK);

  const NTSTATUS status = IoDeleteSymbolicLink(&symbolic_link);
  IoDeleteDevice(g_drv_obj);
  return NT_SUCCESS(status);
}

NTSTATUS ioctl::onCreate(DEVICE_OBJECT* device, IRP* irp) {
  UNREFERENCED_PARAMETER(device);

  IoCompleteRequest(irp, IO_NO_INCREMENT);
  return irp->IoStatus.Status;
}

NTSTATUS ioctl::onClose(DEVICE_OBJECT* device, IRP* irp) {
  UNREFERENCED_PARAMETER(device);

  IoCompleteRequest(irp, IO_NO_INCREMENT);
  return irp->IoStatus.Status;
}

bool ioctl::Register(DRIVER_OBJECT* drv) {
  UNICODE_STRING device_name = RTL_CONSTANT_STRING(DEVICE_NAME);
  UNICODE_STRING symbolic_link = RTL_CONSTANT_STRING(SYMBOLIC_LINK);

  if (!NT_SUCCESS(IoCreateDevice(drv, 0, &device_name, FILE_DEVICE_UNKNOWN, 0,
                                 false, &g_drv_obj))) {
    return false;
  } else if (!NT_SUCCESS(IoCreateSymbolicLink(&symbolic_link, &device_name))) {
    IoDeleteDevice(g_drv_obj);
    return false;
  }

  drv->MajorFunction[IRP_MJ_DEVICE_CONTROL] = &Dispatcher;
  drv->MajorFunction[IRP_MJ_CLOSE] = &onCreate;
  drv->MajorFunction[IRP_MJ_CREATE] = &onClose;

  return true;
}