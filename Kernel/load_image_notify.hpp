#pragma once

#include <ntddk.h>
#include <ntdef.h>
#include <cstdint>

namespace load_image_notify {
void Dispatcher(UNICODE_STRING* image_name, HANDLE pid, IMAGE_INFO* info);
bool Register();
bool Unregister();
}