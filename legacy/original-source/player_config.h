#pragma once
#include <vector>
#include <algorithm>

class PlayerConfig
{
public:
	PlayerConfig() = delete;
	PlayerConfig(int num) : numOfLinks(num){}
	unsigned numOfLinks;
	std::vector<unsigned> links;


	PlayerConfig& operator = (PlayerConfig &other)
	{
		numOfLinks = other.numOfLinks;
		links = other.links;
		return *this;
	}

	bool operator == (PlayerConfig &other)
	{
		return numOfLinks == other.numOfLinks && links == other.links;
	}

	bool operator != (PlayerConfig &other)
	{
		return !operator==(other);
	}

	bool isReady()
	{
		return (links.size() == numOfLinks);
	}

	void sort()
	{
		std::sort(links.begin(), links.end());
	}

	bool find(unsigned num)
	{
		for (auto& link : links)
		{
			if (num == link)
				return true;
		}
		return false;
	}
};

