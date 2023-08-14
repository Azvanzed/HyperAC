#include <cstdint>
#include <docs.hpp>
#include <win.hpp>

void DllMain(uint64_t service_callback) {
	API(MessageBox)(0, 0, 0, 0);
}

