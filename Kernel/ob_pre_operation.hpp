#pragma once

#include <wdm.h>

namespace ob_pre_operation {
inline void* g_handle = nullptr;

OB_PREOP_CALLBACK_STATUS Dispatcher(void* params,
                                    OB_PRE_OPERATION_INFORMATION* info);
void Unregister();
bool Register();
}  // namespace ob_pre_operation