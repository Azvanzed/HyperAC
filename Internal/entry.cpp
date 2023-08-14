#include <Windows.h>
#include <win.hpp>

#include <iostream>

bool DllMain(uint64_t service_callback) {
#ifdef _DEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif




	return true;
}

