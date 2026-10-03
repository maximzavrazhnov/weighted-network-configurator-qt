#pragma once

#include "configuration.h"

#include <fstream>
#include <string>
#include <utility>
#include <vector>

class System
{
public:
    using PlayerInput =
        std::vector<std::pair<unsigned, std::vector<std::pair<unsigned, float>>>>;

    explicit System(const PlayerInput& playersInfo);

    void combinate(const std::string& logFile = "out.txt");
    std::string print() const;
    const std::vector<Configuration>& getConfigs() const noexcept;

private:
    std::vector<Player> players;
    std::vector<Configuration> configs;

    void step(Configuration currentConfig,
              std::vector<std::vector<Mark>>& marks,
              std::ofstream& log,
              std::pair<std::size_t, int> lastInfo);

    void cleanCopies();
    void sortConfigs();
};
