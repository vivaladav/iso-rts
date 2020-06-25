#pragma once

#include <vector>

namespace game
{

class IsoMap;

struct GameMapCell
{
    int mOwnerId = -1;
    bool mEmpty = true;
};

class GameMap
{
public:
    GameMap(IsoMap * isoMap, unsigned int rows, unsigned int cols);

    bool Load(const char * file);

    void SetHomeCell(int numPlayers);

private:
    std::vector<GameMapCell> mCells;

    IsoMap * mIsoMap = nullptr;

    unsigned int mRows = 0;
    unsigned int mCols = 0;
};

} // namespace game
