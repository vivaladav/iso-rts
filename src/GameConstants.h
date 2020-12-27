#pragma once

namespace game
{

constexpr int MAX_NUM_PLAYERS = 4;

constexpr int START_ENERGY = 100;

constexpr float TIME_ENERGY_USE = 2.f;

constexpr int COINS_PER_CELL = 2;

constexpr int ENERGY_PER_CELL = 1;
constexpr int ENERGY_PER_UNIT = 1;

constexpr int MAX_CELL_LEVEL = 3;

constexpr int COST_CELL_UPGRADE[] = { 5, 6, 7 };
constexpr int COST_CONQUEST_CELL = 2;

constexpr int COST_CONQUEST_RES_GEN = 4;

constexpr int MAX_CELL_UNITS = 4;
constexpr int MAX_UNITS_LEVEL = 2;
constexpr int COST_NEW_UNIT[] = { 1, 2, 3 };
constexpr int COST_UNIT_UPGRADE[] = { 1, 2 };

constexpr float TIME_CONQ_CELL = 2.f;
constexpr float TIME_NEW_UNIT = 2.f;
constexpr float TIME_CONQ_RES_GEN = 2.f;
constexpr float TIME_UPG_CELL = 1.f;
constexpr float TIME_UPG_UNIT = 1.f;

constexpr unsigned int PLAYER_COLOR[] =
{
    0xb81414ff,
    0x46b715ff,
    0x1480b8ff,
    0xb814b8ff
};

constexpr float TIME_AI_MOVE = 0.5f;

enum MapLayers
{
    SELECTION,
    MOVE_TARGETS,
    OBJECTS,

    NUM_LAYERS,
};

} // namespace game
