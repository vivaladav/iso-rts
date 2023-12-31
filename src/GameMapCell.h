#pragma once

#include <unordered_map>
#include <unordered_set>

namespace game
{

class GameObject;
class Player;
class ResourceGenerator;
class Unit;

enum PlayerFaction : unsigned int;

enum CellTypes : int
{
    EMPTY = 0,
    SCENE_ROCKS,
    SCENE_ROCKS_SURR,
    NO_FACTION_OBJ,
    BLOBS_SOURCE,
    DIAMONDS_SOURCE,
    BLOBS_SURR,
    DIAMONDS_SURR,
    TREES1,
    TREES1_SURR,
    F1,
    F1_CONNECTED,
    F1_INFLUENCED,
    F2,
    F2_CONNECTED,
    F2_INFLUENCED,
    F3,
    F3_CONNECTED,
    F3_INFLUENCED,
    FOG_OF_WAR,
    NO_FACTION_OBJ_SURR,

    NUM_CELL_TYPES
};

struct GameMapCell
{
    GameMapCell();

    Unit * GetUnit() const;
    bool HasUnit() const;

    ResourceGenerator * GetResourceGenerator() const;

    static bool IsTypePrimary(CellTypes t);

    std::unordered_map<int, bool> influencers;
    PlayerFaction influencer;

    Player * owner = nullptr;
    GameObject * objBottom = nullptr;
    GameObject * objTop = nullptr;

    CellTypes currType = EMPTY;
    CellTypes basicType = EMPTY;

    int row = 0;
    int col = 0;
    bool walkable = true;
    bool walkTarget = false;
    bool changing = false;
    bool linked = false;

private:
    static const std::unordered_set<CellTypes> PRIMARY_TYPES;
};

inline bool GameMapCell::IsTypePrimary(CellTypes t)
{
    return PRIMARY_TYPES.find(t) != PRIMARY_TYPES.end();
}

} // namespace game
