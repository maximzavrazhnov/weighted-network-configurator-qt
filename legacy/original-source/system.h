#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "configuration.h"

class System
{
public:
	//System(std::string filename);
	System(std::vector<std::pair<unsigned, std::vector<std::pair<unsigned, float>>>> playersInfo);

	void combinate();
	std::string print();
	std::vector<Configuration> getConfigs();

private:
	std::vector<Player> players;
	std::vector<Configuration> configs = {};

	void step(Configuration currentConfig, std::vector<std::vector<Mark>> &marks, ofstream& f, std::pair<int, int> lastInfo);
	
	void cleanCopies();
	void sort();
	
};

