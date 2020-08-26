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
    GameMap(Game * game, IsoMap * isoMap, unsigned int rows, unsigned int cols);

    bool Load(const char * file);

    void SetHomeCell();
    void AssignCell(const Cell2D & cell, Player * player);

    int GetCellOwner(unsigned int r, unsigned int c) const;

    bool IsCellChanging(unsigned int r, unsigned int c) const;

    // cell upgrade
    bool CanUpgradeCell(const Cell2D & cell, Player * player);
    void StartUpgradeCell(const Cell2D & cell, Player * player);
    void UpgradeCell(const Cell2D & cell, Player * player);

    // cell fortify
    bool CanFortifyCell(const Cell2D & cell, Player * player);
    void StartFortifyCell(const Cell2D & cell, Player * player);
    void FortifyCell(const Cell2D & cell);

    // unit create
    bool CanCreateUnit(const Cell2D & cell, Player * player);
    void StartCreateUnit(const Cell2D & cell, Player * player);
    void CreateUnit(const Cell2D & cell, Player * player);

    // unit upgrade
    bool CanUpgradeUnit(const Cell2D & cell, Player * player);
    void StartUpgradeUnit(const Cell2D & cell, Player * player);
    void UpgradeUnit(const Cell2D & cell);

    void MoveUnits(const Cell2D * start, const Cell2D * end, int numUnits, Player * player);

    const GameMapCell & GetCell(unsigned int r, unsigned int c) const;

private:
    int DefineCellType(const GameMapCell & cell);
    int DefineUnitType(const GameMapCell & cell);

private:
    std::vector<GameMapCell> mCells;

    Game * mGame = nullptr;

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

inline bool GameMap::IsCellChanging(unsigned int r, unsigned int c) const
{
    if(r < mRows && c < mCols)
        return mCells[r * mCols + c].changing;
    else
        return false;
}

inline const GameMapCell & GameMap::GetCell(unsigned int r, unsigned int c) const
{
    return mCells[r * mCols + c];
}

} // namespace game
