#pragma once

#include <cstdint>
#include <packets.hpp>

#define SERVER_HOST "192.168.109.130"
#define SERVER_PORT "8881"

namespace server {
	bool sendServer(request_header_t* request, response_header_t** response = nullptr);
}