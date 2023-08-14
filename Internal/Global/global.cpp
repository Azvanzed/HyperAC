#include "global.hpp"
#include <Windows.h>
#include <win.hpp>
#include <MemOperations/mem.hpp>
void global::UpdateGameStaticAddresses()
{
	global::game_base = (uintptr_t)API(GetModuleHandleW)(0);

}