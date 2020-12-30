#pragma once

namespace game
{

class GameObject;
class Player;
class ResourceGenerator;
class Unit;

struct GameMapCell
{
    Unit * GetUnit() const;
    bool HasUnit() const;

    ResourceGenerator * GetResourceGenerator() const;
    bool HasResourceGenerator() const;

    Player * owner = nullptr;
    GameObject * obj = nullptr;

    int row = 0;
    int col = 0;
    bool walkable = true;
    bool changing = false;
    bool linked = false;
};

} // namespace game
