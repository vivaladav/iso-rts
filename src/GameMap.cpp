#include "GameMap.h"

#include "Cell2D.h"
#include "Game.h"
#include "GameConstants.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "Player.h"
#include "Screens/ScreenGame.h"

#include <utilities/UniformDistribution.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>

#include <iostream>

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

    // PLAYER 2 - LEVEL 2
    P2_1UL2,
    P2_2UL2,
    P2_3UL2,
    P2_4UL2,

    // PLAYER 2 - LEVEL 3
    P2_1UL3,
    P2_2UL3,
    P2_3UL3,
    P2_4UL3,

    // PLAYER 3 - LEVEL 1
    P3_1UL1,
    P3_2UL1,
    P3_3UL1,
    P3_4UL1,

    // PLAYER 3 - LEVEL 2
    P3_1UL2,
    P3_2UL2,
    P3_3UL2,
    P3_4UL2,

    // PLAYER 3 - LEVEL 3
    P3_1UL3,
    P3_2UL3,
    P3_3UL3,
    P3_4UL3,

    // PLAYER 4 - LEVEL 1
    P4_1UL1,
    P4_2UL1,
    P4_3UL1,
    P4_4UL1,

    // PLAYER 4 - LEVEL 2
    P4_1UL2,
    P4_2UL2,
    P4_3UL2,
    P4_4UL2,

    // PLAYER 4 - LEVEL 3
    P4_1UL3,
    P4_2UL3,
    P4_3UL3,
    P4_4UL3,

    NUM_UNIT_TYPES,

    UNIT_NULL
};

// ==================== CONSTRUCTORS AND DESTRUCTOR ====================

GameMap::GameMap(Game * game, ScreenGame * sg, IsoMap * isoMap)
    : mCells(isoMap->GetNumRows() * isoMap->GetNumCols())
    , mGame(game)
    , mScreenGame(sg)
    , mIsoMap(isoMap)
    , mRows(isoMap->GetNumRows())
    , mCols(isoMap->GetNumCols())
{
    int index = 0;

    for(int r = 0; r < isoMap->GetNumRows(); ++r)
    {
        for(int c = 0; c < isoMap->GetNumCols(); ++c)
        {
            GameMapCell & cell = mCells[index];

            cell.row = r;
            cell.col = c;
            cell.walkable = isoMap->GetCellType(r, c) == EMPTY;

            ++index;
        }
    }
}

// ==================== PUBLIC METHODS ====================

void GameMap::SetHomeCell()
{
    const int limitRow = mIsoMap->GetNumRows() - 1;
    const int limitCol = mIsoMap->GetNumCols() - 1;

    const int NUM_CORNERS = 4;
    const Cell2D corners[NUM_CORNERS] =
    {
        { 0, 0 },
        { 0, limitCol },
        { limitRow, limitCol  },
        { limitRow, 0}
    };

    const int numPlayers = mGame->GetNumPlayers();

    int pick = rand() % NUM_CORNERS;

    for(int p = 0; p < numPlayers; ++p)
    {
        Player * player = mGame->GetPlayer(p);

        const int c = pick % NUM_CORNERS;
        const int ind = corners[c].row * mCols + corners[c].col;

        GameMapCell & cell = mCells[ind];
        cell.owner = player;

        const int cellType = DefineCellType(cell);
        mIsoMap->SetCellType(ind, cellType);

        player->SetHomeCell(corners[c]);
        player->SumCells(1);
        player->SumTotalCellsLevel(1);

        // inc by 2, 1, 2
        pick += 2 - (p % 2);
    }
}

void GameMap::AssignCell(const Cell2D & cell, Player * player)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    gcell.owner = player;

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
       gcell.owner != player ||
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
       gcell.owner != player ||
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
       gcell.owner != player ||
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
    player->SumTotalUnitsLevel(gcell.unitsLevel + 1);

    // update map layer
    const int unitImg = DefineUnitType(gcell);

    if(unitImg != UNIT_NULL)
        mIsoMap->GetLayer(UNITS)->ReplaceObject(r, c, unitImg, NO_ALIGNMENT);

    // reset cell's changing flag
    gcell.changing = false;
}

bool GameMap::CanDestroyUnit(const Cell2D & cell, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);

    // out of bounds
    if(!(r < mRows && c < mCols))
        return false;

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    // not own cell or no units -> exit
    if(gcell.owner != player || !gcell.units)
        return false;

    return true;
}

void GameMap::DestroyUnit(const Cell2D & cell, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);
    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    // update player
    player->SumUnits(-gcell.units);
    player->SumTotalUnitsLevel(-gcell.units * (gcell.unitsLevel + 1));

    // remove units from cell
    gcell.units = 0;
    gcell.unitsLevel = 0;

    // destroy object
    mIsoMap->GetLayer(UNITS)->ClearObject(r, c);
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
       gcell.owner != player ||
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

    // update player
    gcell.owner->SumTotalUnitsLevel(gcell.units);

    // update map layer
    const int unitImg = DefineUnitType(gcell);

    if(unitImg != UNIT_NULL)
        mIsoMap->GetLayer(UNITS)->ReplaceObject(r, c, unitImg, NO_ALIGNMENT);

    // reset cell's changing flag
    gcell.changing = false;
}

bool GameMap::CanUnitMove(const Cell2D & start, const Cell2D & end, Player * player) const
{
    const unsigned int r0 = static_cast<unsigned int>(start.row);
    const unsigned int c0 = static_cast<unsigned int>(start.col);

    // start out of bounds
    if(!(r0 < mRows && c0 < mCols))
        return false;

    const unsigned int r1 = static_cast<unsigned int>(end.row);
    const unsigned int c1 = static_cast<unsigned int>(end.col);

    // end out of bounds
    if(!(r1 < mRows && c1 < mCols))
        return false;

    const int diffR = abs(end.row - start.row);
    const int diffC = abs(end.col - start.col);

    // end too far - units can only move to next cell
    if(diffR > 1 || diffC > 1)
        return false;

    const int ind0 = r0 * mCols + c0;
    const GameMapCell & gcell0 = mCells[ind0];

    // start has no units
    if(0 == gcell0.units)
        return false;

    // start cell is not own cell
    if(player != gcell0.owner)
        return false;

    const int ind1 = r1 * mCols + c1;
    const GameMapCell & gcell1 = mCells[ind1];

    // end not a walkable cell
    if(!gcell1.walkable)
        return false;

    // fail if destination is full or has different level units
    if(gcell1.units == MAX_CELL_UNITS ||
       (gcell1.units > 0 && gcell0.owner == gcell1.owner && gcell0.unitsLevel != gcell1.unitsLevel))
        return false;

    // all good
    return true;
}

bool GameMap::MoveUnits(const Cell2D & start, const Cell2D & end, int numUnits, Player * player)
{
    if(!CanUnitMove(start, end, player))
        return false;

    const int ind0 = start.row * mCols + start.col;
    GameMapCell & gcell0 = mCells[ind0];

    // cap units to move to ones in start, just in case
    if(gcell0.units < numUnits)
        numUnits = gcell0.units;

    const int ind1 = end.row * mCols + end.col;
    GameMapCell & gcell1 = mCells[ind1];

    Player * playerDest = gcell1.owner;

    const bool emptyDest = 0 == gcell1.units;

    // move to empty cell
    if(nullptr == playerDest)
    {
        // move units between cells
        MoveUnitsData(gcell0, gcell1, numUnits);

        // update unit objects and cells
        UpdateCellsAfterMove(gcell0, gcell1, emptyDest);
    }
    // move to own cell
    else if(player == playerDest)
    {
        // end cell is empty
        if(emptyDest)
        {
            // move units between cells
            MoveUnitsData(gcell0, gcell1, numUnits);

            // update unit objects and cells
            UpdateCellsAfterMove(gcell0, gcell1, emptyDest);
        }
        // end cell has units
        else
        {
            // cap units moved to max allowed per cell
            if(gcell1.units + numUnits > MAX_CELL_UNITS)
                numUnits = MAX_CELL_UNITS - gcell1.units;

            // move units between cells
            MoveUnitsData(gcell0, gcell1, numUnits);

            // update unit objects and cells
            UpdateCellsAfterMove(gcell0, gcell1, emptyDest);
        }
    }
    // move to enemy cell (attack)
    else
    {
        const int att0 = DefineCellAttPoints(gcell0, numUnits);
        const int def0 = DefineCellDefPoints(gcell0, numUnits);

        const int att1 = DefineCellAttPoints(gcell1, gcell1.units);
        const int def1 = DefineCellDefPoints(gcell1, gcell1.units);

        const int attKillPower = att0 / def1;
        const int defKillPower = att1 / def0;

        // define how many attackers are left alive
        const int attLeft = numUnits > defKillPower ? numUnits - defKillPower : 0;

        // define how many defenders are left alive
        const int defLeft = gcell1.units > attKillPower ? gcell1.units - attKillPower : 0;

        // update attacking player units stats
        const int attKilled = numUnits - attLeft;
        gcell0.units -= attKilled;
        player->SumUnits(-attKilled);
        player->SumTotalUnitsLevel(-attKilled * (gcell0.unitsLevel + 1));

        // update defending player units stats
        const int defKilled = gcell1.units - defLeft;
        gcell1.units -= defKilled;
        playerDest->SumUnits(-defKilled);
        playerDest->SumTotalUnitsLevel(-defKilled * (gcell1.unitsLevel + 1));

        // cell conquered if:
        // - there's at least 1 attacker left
        // - no defenders left
        // - attack kill power is > 0 or it means an empty cell has not been destroyed
        if(attLeft > 0 && 0 == defLeft && attKillPower > 0)
        {
            // fortification of dest cell is destroyed
            if(gcell1.fortLevel > 0)
                DestroyFortification(gcell1);

            // stop any progress
            if(gcell1.changing)
                StopCellChange(gcell1);

            // update number units to move to reflect ones still alive
            numUnits = attLeft;
            // move units between cells
            MoveUnitsData(gcell0, gcell1, numUnits);

            // update unit objects and cells
            UpdateCellsAfterMove(gcell0, gcell1, emptyDest);
        }
        // attack failed, cell defended
        else
        {
            IsoLayer * layerUnits = mIsoMap->GetLayer(UNITS);

            // update unit object in start
            if(0 == gcell0.units)
            {
                gcell0.unitsLevel = 0;
                layerUnits->ClearObject(start.row, start.col);
            }
            else
            {
                const int unitType0 = DefineUnitType(gcell0);
                layerUnits->ChangeObject(start.row, start.col, unitType0);
            }

            // update unit object in end
            if(0 == gcell1.units)
            {
                gcell1.unitsLevel = 0;
                layerUnits->ClearObject(end.row, end.col);
            }
            else
            {
                const int unitType1 = DefineUnitType(gcell1);
                layerUnits->ChangeObject(end.row, end.col, unitType1);
            }
        }
    }

    // check for victory or game over
    CheckGameEnd();

    return true;
}

void GameMap::CheckGameEnd()
{
    const int numPlayers = mGame->GetNumPlayers();
    int defeated = 0;

    // check for game over and defeated opponents
    for(int i = 0; i < numPlayers; ++i)
    {
        const Player * p = mGame->GetPlayer(i);

        if(p->GetNumCells() == 0)
        {
            if(p->IsLocal())
            {
                mScreenGame->GameOver();
                return;
            }
            else
                ++defeated;
        }
    }

    // check for victory - assuming players is always > 1
    if(defeated == (numPlayers - 1))
        mScreenGame->GameWon();
}

// ==================== PRIVATE METHODS ====================

void GameMap::DestroyFortification(GameMapCell & gcell)
{
    gcell.fortLevel = 0;

    mIsoMap->GetLayer(FORTIFICATIONS)->ClearObject(gcell.row, gcell.col);
}

void GameMap::StopCellChange(GameMapCell & gcell)
{
    gcell.changing = false;

    const Cell2D cell(gcell.row, gcell.col);
    mScreenGame->CancelProgressBar(cell);
}

void GameMap::MoveUnitsData(GameMapCell & gcell0, GameMapCell & gcell1, int numUnits)
{
    // move units between cells
    gcell0.units -= numUnits;
    gcell1.units += numUnits;

    // propagate unit level to end
    gcell1.unitsLevel = gcell0.unitsLevel;

    Player * player0 = gcell0.owner;
    Player * player1 = gcell1.owner;

    // update owner of end
    if(player0 != player1)
    {
        gcell1.owner = player0;

        // update start player stats after conquering a new cell
        const int cellLevel = gcell1.level + 1;

        player0->SumCells(1);
        player0->SumTotalCellsLevel(cellLevel);

        // update end player stat after losing a cell
        if(player1)
        {
            player1->SumCells(-1);
            player1->SumTotalCellsLevel(-cellLevel);
        }
    }
}

void GameMap::UpdateCellsAfterMove(GameMapCell & gcell0, GameMapCell & gcell1, bool emptyDest)
{
    IsoLayer * layerUnits = mIsoMap->GetLayer(UNITS);

    // moved all units
    if(0 == gcell0.units)
    {
        // reset unit level in start
        gcell0.unitsLevel = 0;

        // move unit object from start to end
        if(emptyDest)
            layerUnits->MoveObject(gcell0.row, gcell0.col, gcell1.row, gcell1.col, NO_ALIGNMENT);
        else
        {
            // delete unit object in start
            layerUnits->ClearObject(gcell0.row, gcell0.col);

            // update unit object in end
            const int unitType1 = DefineUnitType(gcell1);
            layerUnits->ChangeObject(gcell1.row, gcell1.col, unitType1);
        }
    }
    // moved part of units
    else
    {
        // update unit object in start
        const int unitType0 = DefineUnitType(gcell0);
        layerUnits->ChangeObject(gcell0.row, gcell0.col, unitType0);

        // add new unit object in end
        if(emptyDest)
        {
            const int unitType1 = DefineUnitType(gcell1);
            layerUnits->AddObject(gcell1.row, gcell1.col, unitType1, NO_ALIGNMENT);
        }
        // update existing object in end
        else
        {
            // update unit object in end
            const int unitType1 = DefineUnitType(gcell1);
            layerUnits->ChangeObject(gcell1.row, gcell1.col, unitType1);
        }
    }

    // update cell type of start
    const int cellType0 = DefineCellType(gcell0);
    mIsoMap->SetCellType(gcell0.row, gcell0.col, cellType0);

    // update cell type of end
    const int cellType1 = DefineCellType(gcell1);
    mIsoMap->SetCellType(gcell1.row, gcell1.col, cellType1);
}

int GameMap::DefineCellAttPoints(const GameMapCell & cell, int numUnits) const
{
    // cell attack points range
    const int cellMinPoints = 1;
    const int cellMaxPoints = 2;

    // unit attack points range
    const int unitMinPoints = 2;
    const int unitMaxPoints = 10;

    int score = 0;

    // add cell score
    lib::utilities::UniformDistribution diceCell(cellMinPoints, cellMaxPoints);
    score += diceCell.GetNextValue();

    // add units score
    lib::utilities::UniformDistribution diceUnit(unitMinPoints, unitMaxPoints);

    const float unitMult[MAX_UNITS_LEVEL + 1] = { 1.f, 1.5f, 2.f };

    for(int i = 0; i < numUnits; ++i)
        score += std::roundf(diceUnit.GetNextValue() * unitMult[cell.unitsLevel]);

    return score;
}

int GameMap::DefineCellDefPoints(const GameMapCell & cell, int numUnits) const
{
    // cell defense points range
    const int cellMinPoints = 1;
    const int cellMaxPoints = 5;

    // unit defense points range
    const int unitMinPoints = 2;
    const int unitMaxPoints = 10;

    int score = 0;

    // add cell score
    lib::utilities::UniformDistribution diceCell(cellMinPoints, cellMaxPoints);

    const float cellMult[MAX_CELL_FORT_LEVEL + 1] = { 1.f, 2.f, 3.f, 4.f };

    score += std::roundf(diceCell.GetNextValue() * cellMult[cell.fortLevel]);

    // add units score
    lib::utilities::UniformDistribution diceUnit(unitMinPoints, unitMaxPoints);

    const float unitMult[MAX_UNITS_LEVEL + 1] = { 1.f, 1.5f, 2.f };

    for(int i = 0; i < numUnits; ++i)
        score += std::roundf(diceUnit.GetNextValue() * unitMult[cell.unitsLevel]);

    return score;
}

int GameMap::DefineCellType(const GameMapCell & cell)
{
    // cell is not owned
    if(nullptr == cell.owner)
        return EMPTY;

    int type = EMPTY;

    switch(cell.owner->GetPlayerId())
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
    // cell is not owned or empty
    if(nullptr == cell.owner || 0 == cell.units)
        return UNIT_NULL;

    int type = (cell.units - 1) + (cell.unitsLevel * MAX_CELL_UNITS);

    switch(cell.owner->GetPlayerId())
    {
        case 0:
            type += P1_1UL1;
        break;

        case 1:
            type += P2_1UL1;
        break;

        case 2:
            type += P3_1UL1;
        break;

        case 3:
            type += P4_1UL1;
        break;

        default:
            type = UNIT_NULL;
        break;
    }

    return type;
}

} // namespace game
