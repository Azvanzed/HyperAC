#include <mmu.hpp>
#include <service.hpp>
#include <threads.hpp>
#include <shared.hpp>

bool service::invokeRequestCallbackEx(void* input, const size_t input_size,
                                      void* output, const size_t output_size) {
  KeAttachProcess(g_service);

  usermode_callback_t* ctx = nullptr;
  size_t ctx_size = sizeof(usermode_callback_t);
  ZwAllocateVirtualMemory((HANDLE)-1, (void**)&ctx, 0, &ctx_size,
                          MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

  ctx->input_size = input_size;
  ctx->output_size = output_size;

  ZwAllocateVirtualMemory((HANDLE)-1, &ctx->input, 0, &ctx->input_size,
                          MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  memcpy(ctx->input, input, input_size);

  ZwAllocateVirtualMemory((HANDLE)-1, &ctx->output, 0, &ctx->output_size,
                          MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  memset(ctx->output, 0, output_size);

  const bool result = threads::createUserThread(g_request_callback, ctx);
  memcpy(output, ctx->output, output_size);

  KeDetachProcess();
  return true;
}