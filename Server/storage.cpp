#include "storage.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

//players/player_id/Detections/timestamp_id_type/evidence.bin

void storage::StorePlayerDetection(const std::string& player_id, const detection_t& detection, const std::vector<uint8_t>& evidence) {
	std::filesystem::path basePath = "players";
	basePath /= player_id;
	basePath /= "Detections";
	basePath /= std::to_string(detection.timestamp) + "_" + std::to_string(detection.id) + "_" + std::to_string(detection.type);
	std::error_code ec;
	if (!create_directories(basePath, ec)) {
		std::cerr << "Failed to create directories: " << ec.message() << std::endl;
		return;
	}
	std::filesystem::path evidencePath = basePath / "evidence.bin";
	std::ofstream file(evidencePath, std::ios::binary);
	if (!file) {
		std::cerr << "Failed to open file for writing: " << evidencePath << std::endl;
		return;
	}
	file.write((const char*)evidence.data(), evidence.size());
}
