#pragma once

namespace game
{

constexpr int MAX_NUM_PLAYERS = 4;

constexpr int START_MONEY = 10;

constexpr float TIME_COINS_GEN = 5.f;

constexpr int COINS_PER_CELL = 1;

constexpr int MAX_CELL_LEVEL = 3;

constexpr int COST_CELL_UPGRADE[] = { 30, 40, 50 };

constexpr int MAX_CELL_FORT_LEVEL = 3;

constexpr int COST_CELL_FORT[] = { 20, 30, 40 };

constexpr int MAX_CELL_UNITS = 1;
constexpr int MAX_UNITS_LEVEL = 2;
constexpr int COST_NEW_UNIT[] = { 10, 20, 40 };
constexpr int COST_UNIT_UPGRADE[] = { 10, 20 };

enum MapLayers
{
    FORTIFICATIONS = 0,
    SELECTION,
    UNITS,

    NUM_LAYERS,
};

} // namespace game
