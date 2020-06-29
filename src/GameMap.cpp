#include "GameMap.h"

#include "Cell2D.h"
#include "Game.h"
#include "GameConstants.h"
#include "IsoMap.h"
#include "Player.h"

#include <cassert>
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

            mCells[ind].empty = (type != FULL);

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
        cell.empty = false;

        const int cellType = DefineCellType(cell);
        mIsoMap->SetCellType(ind, cellType);

        player->SumCells(1);
        player->SumTotalCellsLevel(1);
    }
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

} // namespace game
