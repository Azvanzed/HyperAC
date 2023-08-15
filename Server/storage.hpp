#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct detection_t {
	uint32_t id;
	uint32_t type;
};

namespace storage
{
	void StorePlayerDetection(const std::string& player_id, const detection_t& detection, const std::vector<uint8_t>& evidence);
}

