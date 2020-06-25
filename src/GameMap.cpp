#include "GameMap.h"

#include "IsoMap.h"

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

            mCells[ind].mEmpty = (type != CELL_FULL);

            mIsoMap->SetCellType(r, c, type);
        }
    }

    return true;
}

void GameMap::SetHomeCell(int numPlayers)
{
    const int NUM_CORNERS = 4;
    Cell2D corners[NUM_CORNERS] = { {0, 0}, {0, 14}, {14, 0}, {14, 14} };

    const int pick = rand() % NUM_CORNERS;

    for(int p = 0; p < numPlayers; ++p)
    {
        const int c = (pick + p) % NUM_CORNERS;

        const int ind = corners[c].row * mCols + corners[c].col;

        mIsoMap->SetCellType(ind, p + 1);

        mCells[ind].mOwnerId = p + 1;
    }
}

} // namespace game
