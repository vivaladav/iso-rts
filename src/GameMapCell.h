#pragma once

#include <unordered_map>

namespace game
{

class GameObject;
class Player;
class ResourceGenerator;
class Unit;

struct GameMapCell
{
    GameMapCell();

    Unit * GetUnit() const;
    bool HasUnit() const;

    ResourceGenerator * GetResourceGenerator() const;
    bool HasResourceGenerator() const;

    std::unordered_map<int, bool> influencers;
    int influencer = -1;

    Player * owner = nullptr;
    GameObject * obj = nullptr;

    int row = 0;
    int col = 0;
    bool walkable = true;
    bool changing = false;
    bool linked = false;
};

} // namespace game
