#pragma once

namespace game
{

struct GameMapCell
{
    int ownerId = -1;
    int level = 0;
    int fortLevel = 0;
    int units = 0;
    int unitsLevel = 0;
    bool empty = true;
};

} // namespace game
