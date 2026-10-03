#pragma once

#include <algorithm>
#include <vector>

class PlayerConfig
{
public:
    explicit PlayerConfig(unsigned num) : numOfLinks(num) {}

    unsigned numOfLinks{0};
    std::vector<unsigned> links;

    bool operator==(const PlayerConfig& other) const
    {
        return numOfLinks == other.numOfLinks && links == other.links;
    }

    bool operator!=(const PlayerConfig& other) const
    {
        return !(*this == other);
    }

    bool isReady() const
    {
        return links.size() == numOfLinks;
    }

    void sort()
    {
        std::sort(links.begin(), links.end());
    }

    bool contains(unsigned num) const
    {
        return std::find(links.begin(), links.end(), num) != links.end();
    }
};
