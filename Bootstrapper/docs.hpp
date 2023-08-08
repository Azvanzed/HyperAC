#pragma once

#include <Windows.h>
#include <ntstatus.h>
#include <winternl.h>

extern "C" {
IMAGE_NT_HEADERS* NTAPI RtlImageNtHeader(ULONG64 Base);
}
