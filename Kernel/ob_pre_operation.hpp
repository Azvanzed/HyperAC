#pragma once

#include <wdm.h>

namespace ob_pre_operation {
inline void* g_handle = nullptr;

void onOpenProcess(OB_PRE_OPERATION_INFORMATION* ctx);
void onDupProcess(OB_PRE_OPERATION_INFORMATION* ctx);
void onOpenThread(OB_PRE_OPERATION_INFORMATION* ctx);
void onDupThread(OB_PRE_OPERATION_INFORMATION* ctx);

OB_PREOP_CALLBACK_STATUS Dispatcher(void* params,
                                    OB_PRE_OPERATION_INFORMATION* ctx);
void Unregister();
bool Register();
}  // namespace ob_pre_operation