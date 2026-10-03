#include "player.h"

int Player::getIndex(unsigned link) const
{
    for (std::size_t i = 0; i < possibleLinks.size(); ++i)
    {
        if (possibleLinks[i].first == link)
            return static_cast<int>(i);
    }
    return -1;
}
