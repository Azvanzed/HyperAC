#include <Windows.h>
#include <win.hpp>

#include <MemOperations/mem.hpp>
#ifdef _DEBUG
#include <iostream>
#define cout(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif
bool create_console()
{
	bool result = false;
	if (AllocConsole()) {
		freopen(("CONIN$"), ("r"), stdin);
		freopen(("CONOUT$"), ("w"), stdout);
		freopen(("CONOUT$"), ("w"), stderr);
		result = true;
	}
	return result;
}
bool DllMain(uint64_t service_callback) {
		//API(LoadLibraryA)("user32.dll");
		//API(MessageBoxW)(0, 0, 0, 0);
	create_console();
	cout("0x" << std::hex << mem::GetTextSectionRVA());
	return true;
}

