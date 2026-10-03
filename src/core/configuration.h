#pragma once

#include "player.h"
#include "player_config.h"

#include <fstream>
#include <string>
#include <vector>

class Configuration
{
public:
    Configuration() = default;
    explicit Configuration(const std::vector<Player>& infoPlayers);

    bool operator==(const Configuration& other) const;

    bool isReady() const;
    bool checkLink(std::size_t numOfPlayer, std::size_t linkedPlayerIndex) const;
    bool setLink(std::size_t numOfPlayer, std::size_t linkedPlayerIndex);
    std::string print() const;
    void sort();

    // Returns true when no still-feasible missing link can be found.
    bool checkConfig(const std::vector<Player>& oldPlayers, std::ofstream& log) const;

    std::vector<PlayerConfig> players;
    float weight{0.0F};
};
