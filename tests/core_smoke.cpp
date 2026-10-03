#include "system.h"

#include <cassert>
#include <cmath>
#include <iostream>

int main()
{
    System::PlayerInput input = {
        {2, {{2, 1.0F}, {3, 2.0F}, {4, 3.0F}}},
        {2, {{1, 1.0F}, {3, 2.0F}, {4, 3.0F}}},
        {2, {{1, 2.0F}, {2, 2.0F}, {4, 1.0F}}},
        {2, {{1, 3.0F}, {2, 3.0F}, {3, 1.0F}}},
    };

    System system(input);
    system.combinate("core_smoke_out.txt");

    const auto& configs = system.getConfigs();

    assert(!configs.empty());

    for (const auto& config : configs)
    {
        assert(config.players.size() == 4);
        for (const auto& player : config.players)
            assert(player.links.size() <= player.numOfLinks);
    }

    for (std::size_t i = 1; i < configs.size(); ++i)
        assert(configs[i - 1].weight >= configs[i].weight);

    std::cout << "Configurations: " << configs.size() << "\n";
    std::cout << "Best weight: " << configs.front().weight << "\n";
    return 0;
}
