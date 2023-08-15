#include <intrin.h>

#include <create_process_notify.hpp>
#include <create_thread_notify.hpp>
#include <ioctl.hpp>
#include <load_image_notify.hpp>
#include <ob_pre_operation.hpp>
#include <service.hpp>
#include <shared.hpp>
#include <manual_mapper.hpp>
#include <threads.hpp>

NTSTATUS ioctl::Dispatcher(DEVICE_OBJECT* drv, IRP* irp) {
  UNREFERENCED_PARAMETER(drv);

  NTSTATUS status = STATUS_UNSUCCESSFUL;
  const IO_STACK_LOCATION* stack = IoGetCurrentIrpStackLocation(irp);

  switch (stack->Parameters.DeviceIoControl.IoControlCode) {
    case IOCTL_HYPERAC_INITIALIZE: {
      initialize_input_t input = *(initialize_input_t*)irp->AssociatedIrp.SystemBuffer;
      initialize_output_t* output = (initialize_output_t*)irp->AssociatedIrp.SystemBuffer;

      g_service_callback = input.callback;
      g_service = IoGetCurrentProcess();
      strcpy(g_game_name, input.game_name);

      if (!ob_pre_operation::Register()) {
        print("failed to register a ObPreOperation Callback");
        break;
      } else if (!create_process_notify::Register()) {
        print("failed to register a CreateProcess Callback");
        break;
      } else if (!load_image_notify::Register()) {
          print("failed to register a LoadImage Callback");
          break;
      } else if (!create_thread_notify::Register()) {
        print("failed to register a CreateThread Callback");
        break;
      }

      irp->IoStatus.Information = sizeof(initialize_output_t);
      status = STATUS_SUCCESS;
    } break;
    case IOCTL_HYPERAC_UNINITIALIZE: {
        uninitialize_input_t input = *(uninitialize_input_t*)irp->AssociatedIrp.SystemBuffer;
        uninitialize_output_t* output = (uninitialize_output_t*)irp->AssociatedIrp.SystemBuffer;
        
        ob_pre_operation::Unregister();

      if (!create_process_notify::Unregister()) {
        print("failed to unregister CreateProcess callback");
      } else if (!create_thread_notify::Unregister()) {
        print("failed to unregister CreateThread callback");
      } else if (!load_image_notify::Unregister()) {
        print("failed to unregister LoadImage callback");
      }

      status = STATUS_SUCCESS;
      irp->IoStatus.Information = sizeof(uninitialize_output_t);
    } break;
    case IOCTL_HYPERAC_MANUAL_MAP: {
        manual_map_input_t* input = (manual_map_input_t*)irp->AssociatedIrp.SystemBuffer;
        manual_map_output_t* output = (manual_map_output_t*)irp->AssociatedIrp.SystemBuffer;

        if ((output->ep = manual_mapper::inject(input->process_id, &input->data))) {
            status = STATUS_SUCCESS;
        }

        irp->IoStatus.Information = sizeof(manual_map_output_t);
    } break;
  }


  irp->IoStatus.Status = status;
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