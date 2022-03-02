#pragma once

namespace game
{

constexpr int MAX_NUM_PLAYERS = 4;

constexpr int MAX_UNITS_LEVEL = 2;
constexpr int COST_UNIT_UPGRADE[] = { 1, 2 };

constexpr unsigned int PLAYER_COLOR[] =
{
    0xdb5757ff,
    0x57db62ff,
    0x57badbff,
    0xdb57dbff
};

enum MapLayers
{
    CELL_OVERLAYS1,
    CELL_OVERLAYS2,
    OBJECTS,
    CELL_OVERLAYS3,

    NUM_LAYERS,
};

enum PlayerFaction : unsigned int
{
    FACTION_1,
    FACTION_2,
    FACTION_3,

    NUM_FACTIONS,

    NO_FACTION
};


enum ResourceType : unsigned int
{
    RES_ENERGY,
    RES_MATERIAL1,
    RES_DIAMONDS,
    RES_BLOBS,

    NUM_RESOURCES,

    RES_INVALID
};

} // namespace game
