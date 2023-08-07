#pragma once

#include <ntdef.h>
#include <wdm.h>

#define DEVICE_NAME L"\\Device\\HyperAC"
#define SYMBOLIC_LINK L"\\DosDevices\\HyperAC"


namespace ioctl {
inline DEVICE_OBJECT* g_drv_obj = nullptr;

NTSTATUS Dispatcher(DEVICE_OBJECT* drv, IRP* irp);
NTSTATUS onCreate(DEVICE_OBJECT* device, IRP* irp);
NTSTATUS onClose(DEVICE_OBJECT* device, IRP* irp);

bool Register(DRIVER_OBJECT* drv);
bool Unregister();
}  // namespace ioctl
