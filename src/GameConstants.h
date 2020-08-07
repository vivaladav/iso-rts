#pragma once

namespace game
{

constexpr int MAX_NUM_PLAYERS = 4;

constexpr int START_MONEY = 10;

constexpr float TIME_COINS_GEN = 5.f;

constexpr int COINS_PER_CELL = 1;

constexpr int MAX_CELL_LEVEL = 3;

constexpr int COST_CELL_UPGRADE[] = { 1, 2, 3 };

constexpr int MAX_CELL_FORT_LEVEL = 3;

constexpr int COST_CELL_FORT[] = { 2, 3, 4 };

constexpr int MAX_CELL_UNITS = 4;
constexpr int MAX_UNITS_LEVEL = 2;
constexpr int COST_NEW_UNIT[] = { 1, 2, 4 };
constexpr int COST_UNIT_UPGRADE[] = { 1, 2 };

constexpr float TIME_FORT_CELL = 1.f;
constexpr float TIME_UPG_CELL = 1.f;

enum MapLayers
{
    FORTIFICATIONS = 0,
    SELECTION,
    UNITS,

    NUM_LAYERS,
};

} // namespace game
