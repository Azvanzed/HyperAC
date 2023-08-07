#pragma once

#include <cstdint>
#include <wdm.h>

namespace processes {
HANDLE getId(const PEPROCESS process);
}