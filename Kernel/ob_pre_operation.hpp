#pragma once

#include <wdm.h>

namespace ob_pre_operation {
inline void* g_handle = nullptr;

void onOpenProcess(const PEPROCESS process, ACCESS_MASK* access);
void onDupProcess(const PEPROCESS process, ACCESS_MASK* access);
void onOpenThread(const PETHREAD thread, ACCESS_MASK* access);
void onDupThread(const PETHREAD thread, ACCESS_MASK* access);

OB_PREOP_CALLBACK_STATUS Dispatcher(void* ctx,
                                  OB_PRE_OPERATION_INFORMATION* info);
void Unregister();
bool Register();
}