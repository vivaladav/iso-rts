#pragma once

#include "GameMapCell.h"

#include <vector>

namespace game
{

class Game;
class IsoMap;
class Player;

struct Cell2D;

struct GameMapCell;

class GameMap
{
public:
    GameMap(IsoMap * isoMap, unsigned int rows, unsigned int cols);

    bool Load(const char * file);

    void SetHomeCell(Game * game);

    int GetCellOwner(unsigned int r, unsigned int c) const;

    void FortifyCell(const Cell2D * cell, Player * player);
    void UpgradeCell(const Cell2D * cell, Player * player);
    void NewUnit(const Cell2D * cell, Player * player);

    void MoveUnits(const Cell2D * start, const Cell2D * end, int numUnits, Player * player);

    const GameMapCell & GetCell(unsigned int r, unsigned int c) const;

private:
    int DefineCellType(const GameMapCell & cell);
    int DefineUnitType(const GameMapCell & cell);

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

inline const GameMapCell & GameMap::GetCell(unsigned int r, unsigned int c) const
{
    return mCells[r * mCols + c];
}

} // namespace game
