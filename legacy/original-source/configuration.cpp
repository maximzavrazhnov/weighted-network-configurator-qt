#include "configuration.h"
#include <fstream>

bool Configuration::isReady()
{
	for (auto &player : players)
	{
		if (!player.isReady())
			return false;
	}

	return true;
}

bool Configuration::setLink(int numOfPlayer, int linkedPlayerIndex)
{
	if (!checkLink(numOfPlayer, linkedPlayerIndex))
		return false;
	players.at(numOfPlayer).links.emplace_back(linkedPlayerIndex);
	players.at(linkedPlayerIndex).links.emplace_back(numOfPlayer);
	return true;
}

std::string Configuration::print()
{
	string s;
	for (int i = 0; i < players.size(); i++)
	{
		s += "Player " + std::to_string(i + 1) + ": ";
		if (players[i].links.size() == 0)
			s += "empty \n";
		else
		{
			for (auto &link : players.at(i).links)
				s += std::to_string(link + 1) + " ";
			s += "\n";
		}
	}
	s += "\n";
	return s;
}

void Configuration::sort()
{
	for (auto &player : players)
		player.sort();
}

bool Configuration::checkConfig(std::vector<Player> &oldPlayers, ofstream &f)
{
	for (int i = 0; i < players.size(); i++)
	{
		if (players.at(i).isReady())
			continue;
		
		for (auto& link : oldPlayers.at(i).possibleLinks)
		{
			if (checkLink(i, link.first - 1))
			{
				f << "Got from " << i << " to " << link.first << "\n";
				return false;
			}
		}
	}

	return true;
}

bool Configuration::checkLink(int numOfPlayer, int linkedPlayerIndex)
{
	if (numOfPlayer >= players.size())
		return false;
	if (linkedPlayerIndex >= players.size())
		return false;
	if (players.at(numOfPlayer).isReady())
		return false;
	if (players.at(linkedPlayerIndex).isReady())
		return false;
	if (players.at(linkedPlayerIndex).find(numOfPlayer))
		return false;
	if (players.at(numOfPlayer).find(linkedPlayerIndex))
		return false;

	return true;
}