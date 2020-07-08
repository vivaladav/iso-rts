#include "GameMap.h"

#include "Cell2D.h"
#include "Game.h"
#include "GameConstants.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "Player.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <string>

namespace game
{

enum CellTypes : int
{
    EMPTY = 0,
    FULL,

    // PLAYER 1
    P1L1,
    P1L2,
    P1L3,
    P1L4,

    // PLAYER 2
    P2L1,
    P2L2,
    P2L3,
    P2L4,

    // PLAYER 3
    P3L1,
    P3L2,
    P3L3,
    P3L4,

    // PLAYER 4
    P4L1,
    P4L2,
    P4L3,
    P4L4,

    NUM_CELL_TYPES
};

enum UnitType : int
{
    // PLAYER 1
    P1_1UL1,
    P1_2UL1,
    P1_3UL1,
    P1_4UL1,

    // PLAYER 2
    P2_1UL1,
    P2_2UL1,
    P2_3UL1,
    P2_4UL1,

    // PLAYER 3
    P3_1UL1,
    P3_2UL1,
    P3_3UL1,
    P3_4UL1,

    // PLAYER 4
    P4_1UL1,
    P4_2UL1,
    P4_3UL1,
    P4_4UL1,

    NUM_UNIT_TYPES,

    UNIT_NULL
};

GameMap::GameMap(IsoMap * isoMap, unsigned int rows, unsigned int cols)
    : mCells(rows * cols)
    , mIsoMap(isoMap)
    , mRows(rows)
    , mCols(cols)
{
    assert(isoMap);
}

bool GameMap::Load(const char * file)
{
    std::fstream f(file);

    if(!f.is_open())
        return false;

    std::string line;

    for(unsigned int r = 0; r < mRows; ++r)
    {
        const unsigned int indb = r * mCols;

        std::getline(f, line);

        for(unsigned int c = 0; c < mCols; ++c)
        {
            const unsigned int ind = indb + c;

            const int type = line[c] - '0';

            mCells[ind].walkable = type != FULL;

            mIsoMap->SetCellType(r, c, type);
        }
    }

    return true;
}

void GameMap::SetHomeCell(Game * game)
{
    const int NUM_CORNERS = 4;
    Cell2D corners[NUM_CORNERS] = { {0, 0}, {0, 14}, {14, 0}, {14, 14} };

    const int pick = rand() % NUM_CORNERS;

    const int numPlayers = game->GetNumPlayers();

    for(int p = 0; p < numPlayers; ++p)
    {
        Player * player = game->GetPlayer(p);

        const int c = (pick + p) % NUM_CORNERS;
        const int ind = corners[c].row * mCols + corners[c].col;

        GameMapCell & cell = mCells[ind];
        cell.ownerId = player->GetPlayerId();

        const int cellType = DefineCellType(cell);
        mIsoMap->SetCellType(ind, cellType);

        player->SumCells(1);
        player->SumTotalCellsLevel(1);
    }
}

void GameMap::FortifyCell(const Cell2D * cell, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(cell->row);
    const unsigned int c = static_cast<unsigned int>(cell->col);

    // out of bounds
    if(!(r < mRows && c < mCols))
        return ;

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    // not own cell or max level cell -> exit
    if(gcell.ownerId != player->GetPlayerId() || MAX_CELL_FORT_LEVEL == gcell.fortLevel)
        return ;

    // check if player has enough money
    const int cost = COST_CELL_FORT[gcell.fortLevel];

    if(cost > player->GetMoney())
        return ;

    // all good -> upgrade
    ++(gcell.fortLevel);

    // update player
    player->SumMoney(-cost);

    // update map layer
    mIsoMap->GetLayer(FORTIFICATIONS)->ReplaceObject(r, c, gcell.fortLevel - 1, NO_ALIGNMENT);
}

void GameMap::UpgradeCell(const Cell2D * cell, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(cell->row);
    const unsigned int c = static_cast<unsigned int>(cell->col);

    // out of bounds
    if(!(r < mRows && c < mCols))
        return ;

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    // not own cell or max level cell -> exit
    if(gcell.ownerId != player->GetPlayerId() || MAX_CELL_LEVEL == gcell.level)
        return ;

    // check if player has enough money
    const int cost = COST_CELL_UPGRADE[gcell.level];

    if(cost > player->GetMoney())
        return ;

    // all good -> upgrade
    ++(gcell.level);

    // update player
    player->SumTotalCellsLevel(1);
    player->SumMoney(-cost);

    // update map
    const int cellType = DefineCellType(gcell);
    mIsoMap->SetCellType(ind, cellType);
}

void GameMap::NewUnit(const Cell2D * cell, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(cell->row);
    const unsigned int c = static_cast<unsigned int>(cell->col);

    // out of bounds
    if(!(r < mRows && c < mCols))
        return ;

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    // not own cell or max level cell -> exit
    if(gcell.ownerId != player->GetPlayerId() || MAX_CELL_UNITS == gcell.units)
        return ;

    // check if player has enough money
    const int cost = COST_NEW_UNIT[gcell.unitsLevel];

    if(cost > player->GetMoney())
        return ;

    // all good -> upgrade
    ++(gcell.units);

    // update player
    player->SumUnits(1);
    player->SumMoney(-cost);

    // update map layer
    const int unitImg = DefineUnitType(gcell);

    if(unitImg != UNIT_NULL)
        mIsoMap->GetLayer(UNITS)->ReplaceObject(r, c, unitImg, NO_ALIGNMENT);
}

void GameMap::MoveUnits(const Cell2D * start, const Cell2D * end, int numUnits, Player * player)
{
    const unsigned int r0 = static_cast<unsigned int>(start->row);
    const unsigned int c0 = static_cast<unsigned int>(start->col);

    // out of bounds
    if(!(r0 < mRows && c0 < mCols))
        return ;

    const unsigned int r1 = static_cast<unsigned int>(end->row);
    const unsigned int c1 = static_cast<unsigned int>(end->col);

    // out of bounds
    if(!(r1 < mRows && c1 < mCols))
        return ;

    const int diffR = abs(end->row - start->row);
    const int diffC = abs(end->col - start->col);

    // units can only move to next cell
    if(diffR > 1 || diffC > 1)
        return ;

    const int ind0 = r0 * mCols + c0;
    GameMapCell & gcell0 = mCells[ind0];

    // not enough units to move
    if(0 == gcell0.units || gcell0.units < numUnits)
        return ;

    const int ind1 = r1 * mCols + c1;
    GameMapCell & gcell1 = mCells[ind1];

    // not a walkable cell
    if(!gcell1.walkable)
        return ;

    // free cell
    if(-1 == gcell1.ownerId)
    {
        gcell0.units -= numUnits;
        gcell1.units += numUnits;

        gcell1.ownerId = gcell0.ownerId;

        player->SumCells(1);
        player->SumTotalCellsLevel(1);

        mIsoMap->GetLayer(UNITS)->MoveObject(r0, c0, r1, c1, NO_ALIGNMENT);

        const int cellType = DefineCellType(gcell1);
        mIsoMap->SetCellType(ind1, cellType);
    }
    // own cell
    else if(gcell1.ownerId == player->GetPlayerId())
    {
        gcell0.units -= numUnits;
        gcell1.units += numUnits;

        mIsoMap->GetLayer(UNITS)->MoveObject(r0, c0, r1, c1, NO_ALIGNMENT);
    }
    // enemy cell
    else
    {
        // TODO move to owned cell
    }
}

int GameMap::DefineCellType(const GameMapCell & cell)
{
    int type = EMPTY;

    switch(cell.ownerId)
    {
        case 0:
            type = P1L1 + cell.level;
        break;

        case 1:
            type = P2L1 + cell.level;
        break;

        case 2:
            type = P3L1 + cell.level;
        break;

        case 3:
            type = P4L1 + cell.level;
        break;

        default:
        break;
    }

    return type;
}

int GameMap::DefineUnitType(const GameMapCell & cell)
{
    int type = UNIT_NULL;

    switch (cell.ownerId)
    {
        case 0:
            type = P1_1UL1 + cell.units - 1;
        break;

        case 1:
            type = P2_1UL1 + cell.units - 1;
        break;

        case 2:
            type = P3_1UL1 + cell.units - 1;
        break;

        case 3:
            type = P4_1UL1 + cell.units - 1;
        break;

        default:
        break;
    }

    return type;
}

} // namespace game
