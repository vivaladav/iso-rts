#pragma once

namespace game
{

struct GameMapCell
{
    int ownerId = -1;
    int level = 0;
    int defLevel = 0;
    int units = 0;
    bool empty = true;
};

} // namespace game
