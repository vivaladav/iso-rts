#pragma once

#include "GameMapCell.h"

#include <vector>

namespace game
{

class Game;
class IsoMap;
class Player;
class ScreenGame;

struct Cell2D;
struct GameMapCell;

/// Class that handles most of the logic of what happens on the game map.
class GameMap
{
public:
    GameMap(Game * game, ScreenGame * sg, IsoMap * isoMap);

    bool Load(const char * file);

    void SetHomeCell();
    void AssignCell(const Cell2D & cell, Player * player);

    Player * GetCellOwner(unsigned int r, unsigned int c) const;

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

    // unit destroy
    bool CanDestroyUnit(const Cell2D & cell, Player * player);
    void DestroyUnit(const Cell2D & cell, Player * player);

    // unit upgrade
    bool CanUpgradeUnit(const Cell2D & cell, Player * player);
    void StartUpgradeUnit(const Cell2D & cell, Player * player);
    void UpgradeUnit(const Cell2D & cell);

    bool MoveUnits(const Cell2D & start, const Cell2D & end,
                   int numUnits, Player * player);

    const GameMapCell & GetCell(unsigned int r, unsigned int c) const;
    unsigned int GetNumRows() const;
    unsigned int GetNumCols() const;

    void CheckGameEnd();

private:
    void DestroyFortification(GameMapCell & gcell);
    void StopCellChange(GameMapCell & gcell);

    void MoveUnitsData(GameMapCell & gcell0, GameMapCell & gcell1, int numUnits);
    void UpdateCellsAfterMove(GameMapCell & gcell0, GameMapCell & gcell1, bool emptyDest);

    int DefineCellAttPoints(const GameMapCell & cell, int numUnits) const;
    int DefineCellDefPoints(const GameMapCell & cell, int numUnits) const;

    int DefineCellType(const GameMapCell & cell);
    int DefineUnitType(const GameMapCell & cell);

private:
    std::vector<GameMapCell> mCells;

    Game * mGame = nullptr;
    ScreenGame * mScreenGame = nullptr;

    IsoMap * mIsoMap = nullptr;

    unsigned int mRows = 0;
    unsigned int mCols = 0;
};

// ==================== INLINE METHODS ====================

/**
 * @brief Returns the player ID of the Player that currently controls the cell.
 * @param r Row index, starting from 0
 * @param c Column index, starting from 0
 * @return A player ID, or -1 if the cell is not controlled by any player
 */
inline Player * GameMap::GetCellOwner(unsigned int r, unsigned int c) const
{
    if(r < mRows && c < mCols)
        return mCells[r * mCols + c].owner;
    else
        return nullptr;
}

/**
 * @brief Checks if a cell is in the process of changing (upgrade, fortification in progress).
 * @param r Row index, starting from 0
 * @param c Column index, starting from 0
 * @return TRUE if the cell is changing, FALSE otherwise
 */
inline bool GameMap::IsCellChanging(unsigned int r, unsigned int c) const
{
    if(r < mRows && c < mCols)
        return mCells[r * mCols + c].changing;
    else
        return false;
}

/**
 * @brief Gets a GameMapCell object from the map. No boundaries check is done.
 * @param r Row index, starting from 0
 * @param c Column index, starting from 0
 * @return Const reference to a cell of the map
 */
inline const GameMapCell & GameMap::GetCell(unsigned int r, unsigned int c) const
{
    return mCells[r * mCols + c];
}

/**
 * @brief Gets the number of rows of the map.
 * @return Number of map rows
 */
inline unsigned int GameMap::GetNumRows() const { return mRows; }

/**
 * @brief Gets the number of cells of the map.
 * @return Number of map cells
 */
inline unsigned int GameMap::GetNumCols() const { return mCols; }

} // namespace game
