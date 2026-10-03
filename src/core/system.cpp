#include "system.h"

#include <algorithm>
#include <stdexcept>

System::System(const PlayerInput& playersInfo)
{
    players.reserve(playersInfo.size());
    for (const auto& [numOfLinks, possibleLinks] : playersInfo)
        players.emplace_back(numOfLinks, possibleLinks);
}

void System::combinate(const std::string& logFile)
{
    std::vector<std::vector<Mark>> marks;
    std::ofstream log(logFile);

    configs.clear();

    marks.reserve(players.size());
    for (const auto& player : players)
        marks.emplace_back(player.possibleLinks.size(), Mark::FREE);

    if (players.empty())
        return;

    step(Configuration(players), marks, log, {0, -1});

    std::sort(configs.begin(), configs.end(),
              [](const Configuration& a, const Configuration& b)
              {
                  return a.weight > b.weight;
              });

    cleanCopies();
}

void System::step(Configuration currentConfig,
                  std::vector<std::vector<Mark>>& marks,
                  std::ofstream& log,
                  std::pair<std::size_t, int> lastInfo)
{
    if (currentConfig.isReady())
    {
        currentConfig.sort();
        configs.emplace_back(std::move(currentConfig));
        log << "Full " << configs.size() << "\n";
        return;
    }

    bool stopFlag = true;
    const auto [lastPlayer, lastLink] = lastInfo;

    for (std::size_t playerIndex = lastPlayer; playerIndex < players.size(); ++playerIndex)
    {
        if (currentConfig.players.at(playerIndex).isReady())
            continue;

        std::size_t firstLinkIndex = 0;
        if (playerIndex == lastPlayer && lastLink >= 0)
            firstLinkIndex = static_cast<std::size_t>(lastLink + 1);

        for (std::size_t linkIndex = firstLinkIndex;
             linkIndex < players.at(playerIndex).possibleLinks.size();
             ++linkIndex)
        {
            const auto linkedPlayerNumber =
                players.at(playerIndex).possibleLinks.at(linkIndex).first;

            if (linkedPlayerNumber == 0)
                continue;

            // Process each undirected edge from its lower-numbered endpoint.
            if (linkedPlayerNumber < playerIndex + 1)
                continue;

            if (marks.at(playerIndex).at(linkIndex) != Mark::FREE)
                continue;

            const auto linkedPlayerIndex =
                static_cast<std::size_t>(linkedPlayerNumber - 1);

            if (linkedPlayerIndex >= players.size())
                continue;

            auto currentConfigCopy = currentConfig;
            if (!currentConfigCopy.setLink(playerIndex, linkedPlayerIndex))
                continue;

            currentConfigCopy.weight +=
                players[playerIndex].possibleLinks[linkIndex].second;

            const int otherIndex =
                players.at(linkedPlayerIndex).getIndex(
                    static_cast<unsigned>(playerIndex + 1));

            // The GUI is designed for symmetric input. Ignore malformed
            // one-sided links rather than indexing with -1.
            if (otherIndex < 0)
                continue;

            stopFlag = false;
            marks[playerIndex][linkIndex] = Mark::USED;
            marks[linkedPlayerIndex][static_cast<std::size_t>(otherIndex)] = Mark::USED;

            step(currentConfigCopy,
                 marks,
                 log,
                 {playerIndex, static_cast<int>(linkIndex)});

            marks[playerIndex][linkIndex] = Mark::FREE;
            marks[linkedPlayerIndex][static_cast<std::size_t>(otherIndex)] = Mark::FREE;
        }
    }

    if (stopFlag)
    {
        if (!currentConfig.checkConfig(players, log))
        {
            log << currentConfig.print() << "\n";
            return;
        }

        currentConfig.sort();
        log << "Broken : " << configs.size() << "\n";
        configs.emplace_back(std::move(currentConfig));
    }
}

std::string System::print() const
{
    std::string result = "Found configurations:\n\n";
    for (const auto& config : configs)
        result += config.print();
    return result;
}

void System::cleanCopies()
{
    sortConfigs();

    auto sameTopology = [](const Configuration& a, const Configuration& b)
    {
        return a.players == b.players;
    };

    configs.erase(
        std::unique(configs.begin(), configs.end(), sameTopology),
        configs.end());

    std::sort(configs.begin(), configs.end(),
              [](const Configuration& a, const Configuration& b)
              {
                  return a.weight > b.weight;
              });
}

void System::sortConfigs()
{
    for (auto& config : configs)
        config.sort();

    std::sort(configs.begin(), configs.end(),
              [](const Configuration& a, const Configuration& b)
              {
                  if (a.players.size() != b.players.size())
                      return a.players.size() < b.players.size();
                  return a.print() < b.print();
              });
}

const std::vector<Configuration>& System::getConfigs() const noexcept
{
    return configs;
}
