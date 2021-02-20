#pragma once

namespace game
{

constexpr int MAX_NUM_PLAYERS = 4;

constexpr int MAX_UNITS_LEVEL = 2;
constexpr int ENERGY_NEW_UNIT = 1;
constexpr int MATERIAL_NEW_UNIT = 4;
constexpr int COST_UNIT_UPGRADE[] = { 1, 2 };

constexpr unsigned int PLAYER_COLOR[] =
{
    0xb81414ff,
    0x46b715ff,
    0x1480b8ff,
    0xb814b8ff
};

enum MapLayers
{
    CELL_OVERLAYS,
    MOVE_TARGETS,
    OBJECTS,

    NUM_LAYERS,
};

} // namespace game
