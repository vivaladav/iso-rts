#pragma once

namespace game
{

class GameObject;
class Player;
class Unit;

struct GameMapCell
{
    ~GameMapCell();

    Unit * GetUnit() const;
    bool HasUnit() const;

    Player * owner = nullptr;
    GameObject * obj = nullptr;

    int row = 0;
    int col = 0;
    int level = 0;
    int fortLevel = 0;
    bool walkable = true;
    bool changing = false;
};

} // namespace game
