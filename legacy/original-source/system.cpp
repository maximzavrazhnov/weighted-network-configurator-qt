#include "system.h"


//System::System(std::string filename)
//{
//	ifstream f(filename);
//	string s;
//
//	while (getline(f, s))
//	{
//		players.emplace_back(Player(s));
//	}
//}

System::System(std::vector<std::pair<unsigned, std::vector<std::pair<unsigned, float>>>> playersInfo)
{
	for (auto [numOfLinks, possibleLinks] : playersInfo)
	{
		players.emplace_back(Player(numOfLinks, possibleLinks));
	}
}


void System::combinate()
{
	std::vector<std::vector<Mark>> marks;
	ofstream f("out.txt");

	configs.clear();
	configs.shrink_to_fit();

	for (auto &player : players)
	{
		marks.emplace_back(std::vector<Mark>(player.possibleLinks.size(), Mark::FREE));
	}


	step(Configuration(players), marks, f, { 0, -1 });
	std::sort(configs.begin(), configs.end(), [](Configuration a, Configuration b) {return a.weight >= b.weight;});
}

void System::step(Configuration currentConfig, std::vector<std::vector<Mark>> &marks, ofstream &f, std::pair<int, int> lastInfo)
{
	if (currentConfig.isReady())
	{
		configs.emplace_back(currentConfig);
		f << "Full " << configs.size() << "\n";
		return;
	}

	bool stopFlag = true;
	auto [lastPlayer, lastLink] = lastInfo;

	for (int playerIndex = lastPlayer; playerIndex < players.size(); playerIndex++)
	{
		if (currentConfig.players.at(playerIndex).isReady())
			continue;

		int firstLinkIndex = 0;
		if (playerIndex == lastPlayer)
			firstLinkIndex = lastLink + 1;

		for (int linkIndex = firstLinkIndex; linkIndex < players.at(playerIndex).possibleLinks.size(); linkIndex++)
		{
			if (players.at(playerIndex).possibleLinks.at(linkIndex).first < playerIndex + 1)
				continue;

			if (marks.at(playerIndex).at(linkIndex) == Mark::FREE)
			{
				auto linkedPlayerIndex = players.at(playerIndex).possibleLinks.at(linkIndex).first - 1;
				auto currentConfigCopy = currentConfig;
				if (currentConfigCopy.setLink(playerIndex, linkedPlayerIndex))
				{
					currentConfigCopy.weight += players[playerIndex].possibleLinks[linkIndex].second;
					stopFlag = false;
					marks[playerIndex][linkIndex] = Mark::USED;
					auto otherIndex = players.at(linkedPlayerIndex).getIndex(playerIndex + 1);
					marks[linkedPlayerIndex][otherIndex] = Mark::USED;;
					step(currentConfigCopy, marks, f, { playerIndex, linkIndex });
					marks[playerIndex][linkIndex] = Mark::FREE;
					marks[linkedPlayerIndex][otherIndex] = Mark::FREE;
				}
			}
		}
	}

	if (stopFlag)
	{
		if (!currentConfig.checkConfig(players, f))
		{
			f << currentConfig.print() << "\n\n";
			return;
		}
		f << "Broken : " << configs.size() << "\n";
		configs.emplace_back(currentConfig);
		return;
	}
}

std::string System::print()
{
	string s = "Finded configurations : \n\n";
	for (auto &config : configs)
		s += config.print();
	return s;
}

void System::cleanCopies()
{
	ofstream f("out2.txt");

	sort();
	for (auto it = configs.begin(); it < configs.end() - 1; it++)
	{
		for (auto it2 = it + 1; it2 < configs.end();)
		{
			if (*it == *it2)
			{
				string s = (*it).print();
				f << s << "\n";
				//it2++;
				it2 = configs.erase(it2);
			}
			else
				it2++;
		}
	}
}

void System::sort()
{
	for (auto &config : configs)
		config.sort();
}

std::vector<Configuration> System::getConfigs()
{
	return configs;
}