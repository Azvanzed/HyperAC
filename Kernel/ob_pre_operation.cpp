#include <docs.hpp>
#include <ob_pre_operation.hpp>
#include <processes.hpp>
#include <threads.hpp>
#include <service.hpp>

OB_PREOP_CALLBACK_STATUS ob_pre_operation::Dispatcher(
    void* params, OB_PRE_OPERATION_INFORMATION* info) {
  UNREFERENCED_PARAMETER(params);

  on_handle_request_t ctx;
  ctx.type = user_callback_type_e::handle_request;
  ctx.process_id = (uint64_t)PsGetCurrentProcessId();

  if (info->ObjectType == *PsProcessType) {
    ctx.target.process.process_id =
        (uint64_t)processes::getId((PEPROCESS)info->Object);

    if (info->Operation == OB_OPERATION_HANDLE_CREATE) {
      ctx.access = info->Parameters->CreateHandleInformation.DesiredAccess;
      ctx.request = handle_request_type_e::open_process;
    } else if (info->Operation == OB_OPERATION_HANDLE_DUPLICATE) {
      ctx.access = info->Parameters->DuplicateHandleInformation.DesiredAccess;
      ctx.request = handle_request_type_e::dup_process;
    }
  } else if (info->ObjectType == *PsThreadType) {
    const CLIENT_ID cid = threads::getCid((PETHREAD)info->Object);
    if (cid.UniqueProcess == g_service_pid) {
      goto end;
    } 

    ctx.target.thread.thread_id = (uint64_t)cid.UniqueThread;
    ctx.target.thread.process_id = (uint64_t)cid.UniqueProcess;

    if (info->Operation == OB_OPERATION_HANDLE_CREATE) {
      ctx.access = info->Parameters->CreateHandleInformation.DesiredAccess;
      ctx.request = handle_request_type_e::open_thread;
    } else if (info->Operation == OB_OPERATION_HANDLE_DUPLICATE) {
      ctx.access = info->Parameters->DuplicateHandleInformation.DesiredAccess;
      ctx.request = handle_request_type_e::dup_thread;
    }
  }

  service::invokeRequestCallback(ctx);
  
end:
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
  op[0].PreOperation = (POB_PRE_OPERATION_CALLBACK)&Dispatcher;

  op[1].ObjectType = PsThreadType;
  op[1].Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
  op[1].PreOperation = (POB_PRE_OPERATION_CALLBACK)&Dispatcher;

  OB_CALLBACK_REGISTRATION info;
  memset(&info, 0, sizeof(OB_CALLBACK_REGISTRATION));

  info.Version = OB_FLT_REGISTRATION_VERSION;
  info.OperationRegistrationCount = 2;
  info.OperationRegistration = op;

  return NT_SUCCESS(ObRegisterCallbacks(&info, &g_handle));
}