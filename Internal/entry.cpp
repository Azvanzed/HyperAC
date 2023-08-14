#include <cstdint>
#include <Windows.h>
#include <win.hpp>

void DllMain(uint64_t service_callback) {
	API(MessageBoxW)(0, 0, 0, 0);
}

