#pragma once
#include "player.h"
#include "player_config.h"

class Configuration
{
public:
	Configuration() {}
	Configuration(std::vector<Player> &infoPlayers)
	{
		for (auto &player : infoPlayers)
			players.emplace_back(PlayerConfig(player.numOfLinks));
		weight = 0;
	}
	Configuration(Configuration &other)
	{
		players = other.players;
		weight = other.weight;
	}
	Configuration(Configuration &&other)
	{
		players = std::move(other.players);
		weight = other.weight;
	}

	Configuration& operator= (Configuration &other)
	{
		players = other.players;
		weight = other.weight;
		return *this;
	}

	Configuration& operator= (Configuration &&other)
	{
		players = std::move(other.players);
		weight = other.weight;
		return *this;
	}

	bool operator== (Configuration &other)
	{
		if (players.size() != other.players.size())
			return false;
		for (int i = 0; i < players.size(); i++)
			if (players.at(i) != other.players.at(i))
				return false;
		if (weight != other.weight)
			return false;
		return true;
	}

	bool isReady();
	bool checkLink(int numOfPlayer, int linkedPlayerIndex);
	bool setLink(int numOfPlayer, int linkedPlayerIndex);
	std::string print();
	void sort();
	bool checkConfig(std::vector<Player> &oldPlayers, ofstream &f);

	std::vector<PlayerConfig> players;
	float weight;
};

