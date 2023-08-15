#pragma once

#include <cstdint>
#include <vector>
#include <string>


namespace files {
	bool Read(const std::string& filepath, std::vector<uint8_t>* buffer);
}