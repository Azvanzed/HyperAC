#include <docs.hpp>
#include <ob_pre_operation.hpp>
#include <processes.hpp>
#include <threads.hpp>
#include <service.hpp>

void ob_pre_operation::onOpenProcess(OB_PRE_OPERATION_INFORMATION* ctx) {
  const HANDLE process_id = processes::getId((PEPROCESS)ctx->Object);
  if (process_id != g_game_pid) {
    return;
  }

  on_handle_request_t callback;
  callback.type = service_callback_type_e::handle_request;
  callback.request = handle_request_type_e::open_process;
  callback.parent_id = (uint64_t)PsGetCurrentProcessId();
  callback.process_id = (uint64_t)process_id;
  callback.access = ctx->Parameters->CreateHandleInformation.DesiredAccess;

  service::invokeRequestCallback(callback);
}

void ob_pre_operation::onDupProcess(OB_PRE_OPERATION_INFORMATION* ctx) {
  const HANDLE process_id = processes::getId((PEPROCESS)ctx->Object);
  if (process_id != g_game_pid) {
    return;
  }

  on_handle_request_t callback;
  callback.type = service_callback_type_e::handle_request;
  callback.request = handle_request_type_e::dup_process;
  callback.parent_id = (uint64_t)PsGetCurrentProcessId();
  callback.process_id = (uint64_t)process_id;
  callback.access = ctx->Parameters->DuplicateHandleInformation.DesiredAccess;

  service::invokeRequestCallback(callback);
}

void ob_pre_operation::onOpenThread(OB_PRE_OPERATION_INFORMATION* ctx) {
  const CLIENT_ID cid = threads::getCid((PETHREAD)ctx->Object);
  if (cid.UniqueProcess != g_game_pid) {
    return;
  }

  on_handle_request_t callback;
  callback.type = service_callback_type_e::handle_request;
  callback.request = handle_request_type_e::open_thread;
  callback.parent_id = (uint64_t)PsGetCurrentProcessId();
  callback.thread_id = (uint64_t)cid.UniqueThread;
  callback.process_id = (uint64_t)cid.UniqueProcess;
  callback.access = ctx->Parameters->CreateHandleInformation.DesiredAccess;

  service::invokeRequestCallback(callback);
}

void ob_pre_operation::onDupThread(OB_PRE_OPERATION_INFORMATION* ctx) {
  const CLIENT_ID cid = threads::getCid((PETHREAD)ctx->Object);
  if (cid.UniqueProcess != g_game_pid) {
    return;
  }

  on_handle_request_t callback;
  callback.type = service_callback_type_e::handle_request;
  callback.request = handle_request_type_e::dup_thread;
  callback.parent_id = (uint64_t)PsGetCurrentProcessId();
  callback.thread_id = (uint64_t)cid.UniqueThread;
  callback.process_id = (uint64_t)cid.UniqueProcess;
  callback.access = ctx->Parameters->DuplicateHandleInformation.DesiredAccess;

  service::invokeRequestCallback(callback);
}

OB_PREOP_CALLBACK_STATUS ob_pre_operation::Dispatcher(
    void* params, OB_PRE_OPERATION_INFORMATION* ctx) {
  UNREFERENCED_PARAMETER(params);

  if (ctx->ObjectType == *PsProcessType) {
    if (ctx->Operation == OB_OPERATION_HANDLE_CREATE) {
      onOpenProcess(ctx);
    } else if (ctx->Operation == OB_OPERATION_HANDLE_DUPLICATE) {
      onDupProcess(ctx);
    }
  } else if (ctx->ObjectType == *PsThreadType) {
    if (ctx->Operation == OB_OPERATION_HANDLE_CREATE) {
      onOpenThread(ctx);
    } else if (ctx->Operation == OB_OPERATION_HANDLE_DUPLICATE) {
      onDupThread(ctx);
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