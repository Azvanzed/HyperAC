#include <Windows.h>
#include <win.hpp>

#include <iostream>

bool DllMain(void* service_callback) {
	g_service_callback = service_callback;

#ifdef _DEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif




	return true;
}

