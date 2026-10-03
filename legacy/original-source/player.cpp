#include "player.h"

size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
	size_t pos = txt.find(ch);
	size_t initialPos = 0;
	strs.clear();

	// Decompose statement
	while (pos != std::string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}

	// Add the last one
	strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

	return strs.size();
}

//Player::Player(const std::string &s)
//{
//	std::vector<string> nums;
//	split(s, nums, ' ');
//	possibleLinks.clear();
//
//	numOfLinks = atoi(nums.at(0).c_str());
//	for (unsigned i = 1; i < nums.size(); i++)
//	{
//		possibleLinks.emplace_back(atoi(nums.at(i).c_str()));
//	}
//}