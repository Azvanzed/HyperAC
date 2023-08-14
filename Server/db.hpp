#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <mysql_connection.h>

namespace db {
	struct player_t {
		std::string id;
		std::vector<std::string> hwids;
		int last_play;
		int flags;
	};

	inline sql::Connection* g_con = nullptr;

	std::vector<std::string> tokenize(const std::string& str);
	std::string untokenize(const std::vector<std::string>& tokens);

	bool findPlayers(const std::string& id, std::vector<player_t>* players);
	player_t findPlayer(const std::string& id);
	bool addPlayer(const std::string& id, const std::vector<std::string>& hwids);
	bool addPlayer(const player_t& player);
}