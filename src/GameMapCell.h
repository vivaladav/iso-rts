#pragma once

namespace game
{

class Player;

struct GameMapCell
{
    Player * owner = nullptr;
    int row = 0;
    int col = 0;
    int level = 0;
    int fortLevel = 0;
    int units = 0;
    int unitsLevel = 0;
    bool walkable = true;
    bool changing = false;

    void Clear();
};

inline void GameMapCell::Clear()
{
    owner = nullptr;
    level = 0;
    fortLevel = 0;
    units = 0;
    unitsLevel = 0;
    walkable = true;
    changing = false;
}

} // namespace game
