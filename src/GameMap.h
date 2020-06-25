#pragma once

#include <vector>

namespace game
{

class IsoMap;

struct GameMapCell
{
    int ownerId = -1;
    bool empty = true;
};

class GameMap
{
public:
    GameMap(IsoMap * isoMap, unsigned int rows, unsigned int cols);

    bool Load(const char * file);

    void SetHomeCell(int numPlayers);

    int GetCellOwner(unsigned int r, unsigned int c) const;

private:
    std::vector<GameMapCell> mCells;

    IsoMap * mIsoMap = nullptr;

    unsigned int mRows = 0;
    unsigned int mCols = 0;
};

inline int GameMap::GetCellOwner(unsigned int r, unsigned int c) const
{
    if(r < mRows && c < mCols)
        return mCells[r * mCols + c].ownerId;
    else
        return -1;
}

} // namespace game
