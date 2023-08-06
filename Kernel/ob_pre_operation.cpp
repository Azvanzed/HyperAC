#include <ob_pre_operation.hpp>

void ob_pre_operation::onOpenProcess(const PEPROCESS process,
                                     ACCESS_MASK* access) {
  UNREFERENCED_PARAMETER(process);
  UNREFERENCED_PARAMETER(access);
}

void ob_pre_operation::onDupProcess(const PEPROCESS process,
                                    ACCESS_MASK* access) {
  UNREFERENCED_PARAMETER(process);
  UNREFERENCED_PARAMETER(access);
}

void ob_pre_operation::onOpenThread(const PETHREAD thread,
                                    ACCESS_MASK* access) {
  UNREFERENCED_PARAMETER(thread);
  UNREFERENCED_PARAMETER(access);
}

void ob_pre_operation::onDupThread(const PETHREAD thread, ACCESS_MASK* access) {
  UNREFERENCED_PARAMETER(thread);
  UNREFERENCED_PARAMETER(access);
}

OB_PREOP_CALLBACK_STATUS ob_pre_operation::onHandle(
    void* ctx, OB_PRE_OPERATION_INFORMATION* info) {
  UNREFERENCED_PARAMETER(ctx);

  if (info->Object == *PsProcessType) {
    if (info->Operation == OB_OPERATION_HANDLE_CREATE) {
      onOpenProcess((PEPROCESS)info->Object,
                    &info->Parameters->CreateHandleInformation.DesiredAccess);
    } else if (info->Operation == OB_OPERATION_HANDLE_DUPLICATE) {
      onDupProcess((PEPROCESS)info->Object,
                   &info->Parameters->DuplicateHandleInformation.DesiredAccess);
    }
  }
  else if (info->ObjectType == *PsThreadType) {
    if (info->Operation == OB_OPERATION_HANDLE_CREATE) {
      onOpenThread((PETHREAD)info->Object,
                  &info->Parameters->CreateHandleInformation.DesiredAccess);
    } else if (info->Operation == OB_OPERATION_HANDLE_DUPLICATE) {
      onDupThread((PETHREAD)info->Object,
                  &info->Parameters->DuplicateHandleInformation.DesiredAccess);
    }
  }

  return OB_PREOP_SUCCESS;
}

void ob_pre_operation::Unregister() {
  if (g_handle) {
    ObUnRegisterCallbacks(g_handle);
    g_handle = nullptr;
  }
}

bool ob_pre_operation::Register() {
  OB_OPERATION_REGISTRATION op[2];
  memset(&op, 0, sizeof(op));

  op[0].ObjectType = PsProcessType;
  op[0].Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
  op[0].PreOperation = (POB_PRE_OPERATION_CALLBACK)&onHandle;

  op[1].ObjectType = PsThreadType;
  op[1].Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
  op[1].PreOperation = (POB_PRE_OPERATION_CALLBACK)&onHandle;

  OB_CALLBACK_REGISTRATION info;
  memset(&info, 0, sizeof(OB_CALLBACK_REGISTRATION));

  info.Version = OB_FLT_REGISTRATION_VERSION;
  info.OperationRegistrationCount = 2;
  info.OperationRegistration = op;

  return NT_SUCCESS(ObRegisterCallbacks(&info, &g_handle));
}