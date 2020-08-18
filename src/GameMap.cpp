#include "GameMap.h"

#include "Cell2D.h"
#include "Game.h"
#include "GameConstants.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "Player.h"
#include "utilities/UniformDistribution.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

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
    // PLAYER 1 - LEVEL 1
    P1_1UL1,
    P1_2UL1,
    P1_3UL1,
    P1_4UL1,

    // PLAYER 1 - LEVEL 2
    P1_1UL2,
    P1_2UL2,
    P1_3UL2,
    P1_4UL2,

    // PLAYER 1 - LEVEL 3
    P1_1UL3,
    P1_2UL3,
    P1_3UL3,
    P1_4UL3,

    // PLAYER 2 - LEVEL 1
    P2_1UL1,
    P2_2UL1,
    P2_3UL1,
    P2_4UL1,

    // PLAYER 3 - LEVEL 1
    P3_1UL1,
    P3_2UL1,
    P3_3UL1,
    P3_4UL1,

    // PLAYER 4 - LEVEL 1
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

void GameMap::AssignCell(const Cell2D & cell, Player * player)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    gcell.ownerId = player->GetPlayerId();

    const int cellType = DefineCellType(gcell);
    mIsoMap->SetCellType(ind, cellType);

    player->SumCells(1);
    player->SumTotalCellsLevel(1);
}

bool GameMap::CanUpgradeCell(const Cell2D & cell, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);

    // out of bounds
    if(!(r < mRows && c < mCols))
        return false;

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    // already changing, not own cell or already max level
    if(gcell.changing ||
       gcell.ownerId != player->GetPlayerId() ||
       MAX_CELL_LEVEL == gcell.level)
        return false;

    // check if player has enough money
    const int cost = COST_CELL_UPGRADE[gcell.level];

    if(cost > player->GetMoney())
        return false;

    return true;
}

void GameMap::StartUpgradeCell(const Cell2D & cell, Player * player)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    // take player's money
    const int cost = COST_CELL_UPGRADE[gcell.level];

    player->SumMoney(-cost);

    // mark cell as changing
    gcell.changing = true;
}

void GameMap::UpgradeCell(const Cell2D & cell, Player * player)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    ++(gcell.level);

    // update player
    player->SumTotalCellsLevel(1);

    // update map
    const int cellType = DefineCellType(gcell);
    mIsoMap->SetCellType(ind, cellType);

    // reset cell's changing flag
    gcell.changing = false;
}

bool GameMap::CanFortifyCell(const Cell2D & cell, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);

    // out of bounds
    if(!(r < mRows && c < mCols))
        return false;

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    // already changing, not own cell or max level cell -> exit
    if(gcell.changing ||
       gcell.ownerId != player->GetPlayerId() ||
       MAX_CELL_FORT_LEVEL == gcell.fortLevel)
        return false;

    // check if player has enough money
    const int cost = COST_CELL_FORT[gcell.fortLevel];

    if(cost > player->GetMoney())
        return false;

    return true;
}

void GameMap::StartFortifyCell(const Cell2D & cell, Player * player)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    // take player's money
    const int cost = COST_CELL_FORT[gcell.fortLevel];

    player->SumMoney(-cost);

    // mark cell as changing
    gcell.changing = true;
}

void GameMap::FortifyCell(const Cell2D & cell)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);
    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    ++(gcell.fortLevel);

    // update map layer
    mIsoMap->GetLayer(FORTIFICATIONS)->ReplaceObject(r, c, gcell.fortLevel - 1, NO_ALIGNMENT);

    // reset cell's changing flag
    gcell.changing = false;
}

bool GameMap::CanCreateUnit(const Cell2D & cell, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);

    // out of bounds
    if(!(r < mRows && c < mCols))
        return false;

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    // already changing, not own cell or max level cell -> exit
    if(gcell.changing ||
       gcell.ownerId != player->GetPlayerId() ||
       MAX_CELL_UNITS == gcell.units)
        return false;

    // check if player has enough money
    const int cost = COST_NEW_UNIT[gcell.unitsLevel];

    if(cost > player->GetMoney())
        return false;

    return true;
}

void GameMap::StartCreateUnit(const Cell2D & cell, Player * player)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    // make player pay
    const int cost = COST_NEW_UNIT[gcell.unitsLevel];
    player->SumMoney(-cost);

    // mark cell as changing
    gcell.changing = true;
}

void GameMap::CreateUnit(const Cell2D & cell, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    ++(gcell.units);

    // update player
    player->SumUnits(1);

    // update map layer
    const int unitImg = DefineUnitType(gcell);

    if(unitImg != UNIT_NULL)
        mIsoMap->GetLayer(UNITS)->ReplaceObject(r, c, unitImg, NO_ALIGNMENT);

    // reset cell's changing flag
    gcell.changing = false;
}

bool GameMap::CanUpgradeUnit(const Cell2D & cell, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);

    // out of bounds
    if(!(r < mRows && c < mCols))
        return false;

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    // cell already changing, not own cell or max level units or no units -> exit
    if(gcell.changing ||
       gcell.ownerId != player->GetPlayerId() ||
       MAX_UNITS_LEVEL == gcell.unitsLevel || !gcell.units)
        return false;

    // check if player has enough money
    const int cost = COST_UNIT_UPGRADE[gcell.unitsLevel] * gcell.units;

    if(cost > player->GetMoney())
        return false;

    return true;
}

void GameMap::StartUpgradeUnit(const Cell2D & cell, Player * player)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    // make player pay
    const int cost = COST_UNIT_UPGRADE[gcell.unitsLevel];
    player->SumMoney(-cost);

    // mark cell as changing
    gcell.changing = true;
}

void GameMap::UpgradeUnit(const Cell2D & cell)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);
    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    // all good -> upgrade
    ++(gcell.unitsLevel);

    // update map layer
    const int unitImg = DefineUnitType(gcell);

    if(unitImg != UNIT_NULL)
        mIsoMap->GetLayer(UNITS)->ReplaceObject(r, c, unitImg, NO_ALIGNMENT);

    // reset cell's changing flag
    gcell.changing = false;
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

    IsoLayer * layerUnits = mIsoMap->GetLayer(UNITS);

    bool moved = false;

    // free (not owned by any player) cell
    if(-1 == gcell1.ownerId)
    {
        gcell0.units -= numUnits;
        gcell1.units += numUnits;

        gcell1.ownerId = gcell0.ownerId;
        gcell1.unitsLevel = gcell0.unitsLevel;

        player->SumCells(1);
        player->SumTotalCellsLevel(1);

        moved = layerUnits->MoveObject(r0, c0, r1, c1, NO_ALIGNMENT);

        // conquest cell
        if(moved)
        {
            const int cellType = DefineCellType(gcell1);
            mIsoMap->SetCellType(ind1, cellType);
        }
    }
    // own cell
    else if(gcell1.ownerId == player->GetPlayerId())
    {
        // target cell has already units
        if(gcell1.units)
        {
            // destination is full or has different level units
            if(gcell1.units == MAX_CELL_UNITS || (gcell0.unitsLevel != gcell1.unitsLevel))
                return ;

            // cap units moved to max allowed per cell
            if(gcell1.units + numUnits > MAX_CELL_UNITS)
                numUnits = MAX_CELL_UNITS - gcell1.units;

            gcell0.units -= numUnits;
            gcell1.units += numUnits;

            // update dest object
            const int unitType1 = DefineUnitType(gcell1);
            layerUnits->ChangeObject(r1, c1, unitType1);

            // update src object if any unit left
            if(gcell0.units)
            {
                const int unitType0 = DefineUnitType(gcell0);
                layerUnits->ChangeObject(r0, c0, unitType0);
            }
            // otherwise, clear src object
            else
            {
                gcell0.unitsLevel = 0;
                layerUnits->ClearObject(r0, c0);
            }

            // done here
            return ;
        }
        else
        {
            gcell0.units -= numUnits;
            gcell1.units += numUnits;

            gcell1.unitsLevel = gcell0.unitsLevel;

            moved = layerUnits->MoveObject(r0, c0, r1, c1, NO_ALIGNMENT);
        }
    }
    // enemy cell
    else
    {
        lib::utilities::UniformDistribution rgen(POINTS_CELL_MIN, POINTS_CELL_MAX);

        // points of cell
        const int cellPoints = rgen.GetNextValue() * (gcell1.fortLevel + POINTS_CELL_INC);

        // points of attacking units
        rgen.SetParameters(POINTS_UNIT_MIN, POINTS_UNIT_MAX);
        std::vector<int> pointsAtt(numUnits);

        for(int i = 0; i < numUnits; ++i)
            pointsAtt[i] = rgen.GetNextValue() * (gcell0.unitsLevel + POINTS_UNIT_INC);

        // cell with units
        if(gcell1.units)
        {

        }
        // cell with no units
        else
        {
            int losses = 0;

            // fight
            for(int points : pointsAtt)
            {
                if(points <= cellPoints)
                {
                    --gcell0.units;
                    ++losses;
                }
            }

            // update player
            player->SumUnits(-losses);

            // some unit left
            if(gcell0.units)
            {
                // remove alive attacking units from cell0
                const int attackingLeft = numUnits - losses;
                gcell0.units -= attackingLeft;

                // update attacking units
                if(gcell0.units)
                {
                    const int unitType0 = DefineUnitType(gcell0);
                    layerUnits->ChangeObject(r0, c0, unitType0);
                }
                else
                    layerUnits->ClearObject(r0, c0);

                // some attacking unit left -> cell conquered
                if(attackingLeft)
                {
                    // update conquered cell
                    gcell1.ownerId = gcell0.ownerId;
                    gcell1.level = 0;
                    gcell1.fortLevel = 0;
                    gcell1.unitsLevel = gcell1.unitsLevel;
                    gcell1.units = attackingLeft;

                    // update source attackingcell
                    const int cellType = DefineCellType(gcell1);
                    mIsoMap->SetCellType(ind1, cellType);

                    // create new units in conquered cell
                    const int unitImg1 = DefineUnitType(gcell1);
                    layerUnits->AddObject(r1, c1, unitImg1, NO_ALIGNMENT);

                    // update player
                    player->SumCells(1);
                    player->SumTotalCellsLevel(1);
                }
            }
            // no attacking units left -> defender won
            else
            {
                // clear fortification, if any
                if(gcell0.fortLevel)
                    mIsoMap->GetLayer(FORTIFICATIONS)->ClearObject(r0, c0);

                // clear attacker cell
                gcell0.ownerId = -1;
                gcell0.level = 0;
                gcell0.fortLevel = 0;
                gcell0.unitsLevel = 0;

                // update cell
                const int cellType = DefineCellType(gcell0);
                mIsoMap->SetCellType(ind0, cellType);

                // delete units in attacker cell
                layerUnits->ClearObject(r0, c0);
            }
        }
    }

    if(moved)
    {
        // create image in src cell if any unit left
        if(gcell0.units)
        {
            const int unitImg0 = DefineUnitType(gcell0);
            layerUnits->AddObject(r0, c0, unitImg0, NO_ALIGNMENT);
        }
        // reset unit level of empty cells
        else
            gcell0.unitsLevel = 0;

        // update image in dest cell
        const int unitType1 = DefineUnitType(gcell1);
        layerUnits->ChangeObject(r1, c1, unitType1);
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
            type = P1_1UL1 + (cell.units - 1) + (cell.unitsLevel * MAX_CELL_UNITS);
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
