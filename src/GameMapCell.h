#pragma once

#include <unordered_map>

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
    SCENE_RESOURCE,
    BLOBS_SOURCE,
    DIAMONDS_SOURCE,
    BLOBS_SURR,
    DIAMONDS_SURR,
    TREES1,
    TREES1_SURR,

    // FACTION 1
    F1,
    F1_CONNECTED,
    F1_INFLUENCED,

    // FACTION 2
    F2,
    F2_CONNECTED,
    F2_INFLUENCED,

    // FACTION 3
    F3,
    F3_CONNECTED,
    F3_INFLUENCED,

    // SPECIAL
    FOG_OF_WAR,

    NUM_CELL_TYPES
};

struct GameMapCell
{
    GameMapCell();

    Unit * GetUnit() const;
    bool HasUnit() const;

    ResourceGenerator * GetResourceGenerator() const;
    bool HasResourceGenerator() const;

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
    bool changing = false;
    bool linked = false;
};

} // namespace game
