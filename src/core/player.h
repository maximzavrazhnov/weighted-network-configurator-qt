#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

enum class Mark
{
    USED,
    FREE
};

class Player
{
public:
    Player() = default;

    Player(unsigned numOfLinks,
           const std::vector<std::pair<unsigned, float>>& possibleLinks)
        : numOfLinks(numOfLinks), possibleLinks(possibleLinks)
    {
    }

    unsigned numOfLinks{0};
    std::vector<std::pair<unsigned, float>> possibleLinks;

    int getIndex(unsigned link) const;
};
