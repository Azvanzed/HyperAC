#include <storage.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

// players/player_id/Detections/timestamp_uid_type/evidence.bin
void storage::StorePlayerDetection(const std::string& player_id, const detection_t& detection, const std::vector<uint8_t>& evidence) {
	std::filesystem::path basePath = "players";
	basePath /= player_id;
	basePath /= "Detections";
	basePath /= std::to_string(time(0)) + "_" + std::to_string(detection.id) + "_" + std::to_string(detection.type);
	
	std::error_code ec;
	if (!create_directories(basePath, ec)) {
		std::cerr << "Failed to create directories: " << ec.message() << std::endl;
		return;
	}

	std::filesystem::path path = basePath / "evidence.bin";
	std::ofstream file(path, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Failed to open file for writing: " << path << std::endl;
		return;
	}

	file.write((char*)evidence.data(), evidence.size());
}
