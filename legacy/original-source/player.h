#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);

enum class Mark
{
	USED, FREE
};

class Player
{
public:
	Player() {};
	Player(int num, const std::vector<std::pair<unsigned, float>> &info)
	{
		numOfLinks = num;
		possibleLinks = info;
	}

	//Player(const std::string &s);
	Player(Player &other)
	{
		numOfLinks = other.numOfLinks;
		possibleLinks = other.possibleLinks;
	}
	Player(Player &&other)
	{
		numOfLinks = other.numOfLinks;
		possibleLinks = std::move(other.possibleLinks);
	}

	unsigned numOfLinks;
	std::vector<std::pair<unsigned, float>> possibleLinks;

	int getIndex(int link)
	{
		for (int i = 0; i < possibleLinks.size(); i++)
			if (possibleLinks[i].first == link)
				return i;
		return -1;
	}
};

