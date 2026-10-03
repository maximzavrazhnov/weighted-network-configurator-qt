#include "configuration.h"

Configuration::Configuration(const std::vector<Player>& infoPlayers)
{
    players.reserve(infoPlayers.size());
    for (const auto& player : infoPlayers)
        players.emplace_back(player.numOfLinks);
}

bool Configuration::operator==(const Configuration& other) const
{
    return players == other.players && weight == other.weight;
}

bool Configuration::isReady() const
{
    for (const auto& player : players)
    {
        if (!player.isReady())
            return false;
    }
    return true;
}

bool Configuration::setLink(std::size_t numOfPlayer, std::size_t linkedPlayerIndex)
{
    if (!checkLink(numOfPlayer, linkedPlayerIndex))
        return false;

    players.at(numOfPlayer).links.emplace_back(static_cast<unsigned>(linkedPlayerIndex));
    players.at(linkedPlayerIndex).links.emplace_back(static_cast<unsigned>(numOfPlayer));
    return true;
}

std::string Configuration::print() const
{
    std::string result;
    for (std::size_t i = 0; i < players.size(); ++i)
    {
        result += "Player " + std::to_string(i + 1) + ": ";
        if (players[i].links.empty())
        {
            result += "empty\n";
        }
        else
        {
            for (const auto link : players[i].links)
                result += std::to_string(link + 1) + " ";
            result += "\n";
        }
    }
    result += "\n";
    return result;
}

void Configuration::sort()
{
    for (auto& player : players)
        player.sort();
}

bool Configuration::checkConfig(const std::vector<Player>& oldPlayers, std::ofstream& log) const
{
    for (std::size_t i = 0; i < players.size(); ++i)
    {
        if (players.at(i).isReady())
            continue;

        for (const auto& link : oldPlayers.at(i).possibleLinks)
        {
            const auto linkedIndex = static_cast<std::size_t>(link.first - 1);
            if (checkLink(i, linkedIndex))
            {
                log << "Got from " << i << " to " << link.first << "\n";
                return false;
            }
        }
    }

    return true;
}

bool Configuration::checkLink(std::size_t numOfPlayer, std::size_t linkedPlayerIndex) const
{
    if (numOfPlayer >= players.size() || linkedPlayerIndex >= players.size())
        return false;
    if (numOfPlayer == linkedPlayerIndex)
        return false;
    if (players.at(numOfPlayer).isReady() || players.at(linkedPlayerIndex).isReady())
        return false;
    if (players.at(linkedPlayerIndex).contains(static_cast<unsigned>(numOfPlayer)))
        return false;
    if (players.at(numOfPlayer).contains(static_cast<unsigned>(linkedPlayerIndex)))
        return false;

    return true;
}
