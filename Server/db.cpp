#include <db.hpp>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <cstdint>
#include <string.h>

std::vector<std::string> db::tokenize(const std::string& str) {
	std::vector<std::string> tokens;
	size_t start = 0;

	for (size_t i = 0; i < str.size(); ++i) {
		if (str[i] == '|' || str[i] == ';') {
			tokens.push_back(str.substr(start, i - start));
			start = i + 1;
		}
	}

	return tokens;
}

std::string db::untokenize(const std::vector<std::string>& tokens) {
	std::string result;
	for (const std::string& token : tokens) {
		if (!result.empty()) {
			result += "|";
		}
		result += token;
	}

	return result + ";";
}

bool db::findPlayers(const std::string& id, std::vector<player_t>* players) {
	auto statement = g_con->createStatement();
	auto result = statement->executeQuery("SELECT * FROM `players` WHERE `id`='" + id + "';");
	
	while (result->next()) {
		player_t player;
		memset(&player, 0, sizeof(player_t));

		player.id = result->getString("id");
		player.hwids = db::tokenize(result->getString("hwids"));
		player.flags = result->getInt("flags");
		player.last_play = result->getInt("last_play");

		players->push_back(player);
	}

	delete statement;
	delete result;
	return !players->empty();
}

db::player_t db::findPlayer(const std::string& id) {
	std::vector<player_t> players;
	if (!findPlayers(id, &players)) {
		return {};
	}

	return players[0];
}

bool db::addPlayer(const std::string& id, const std::vector<std::string>& hwids) {
	std::vector<player_t> players;
	if (findPlayers(id, &players)) {
		return false;
	}

	auto statement = g_con->createStatement();
	bool status = statement->execute(
		"INSERT INTO `players` (`id`, `hwids`, `last_play`, `flags`) VALUES ("
		"'" + id + "', "
		"'" + db::untokenize(hwids) + "', "
		"0, "
		"0"
		");"
	);

	delete statement;
	return status;
}

bool db::addPlayer(const player_t& player) {
	return addPlayer(player.id, player.hwids);
}