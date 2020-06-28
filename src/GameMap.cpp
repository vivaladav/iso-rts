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

const int CELL_FULL = 5;

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

            mCells[ind].empty = (type != CELL_FULL);

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

        mIsoMap->SetCellType(ind, p + 1);

        GameMapCell & cell = mCells[ind];
        cell.ownerId = player->GetPlayerId();
        cell.level = 1;
        cell.empty = false;

        player->SumCells(1);
        player->SumTotalCellsLevel(cell.level);
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
    const int cost = COST_CELL_UPGRADE[gcell.level - 1];

    if(cost > player->GetMoney())
        return ;

    // all good -> upgrade
    ++(gcell.level);

    player->SumTotalCellsLevel(1);
    player->SumMoney(-cost);
}

} // namespace game
