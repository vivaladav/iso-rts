#pragma once

#include "GameMapCell.h"

#include <vector>

namespace game
{

class Game;
class GameObject;
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
    ~GameMap();

    void SetSize(unsigned int rows, unsigned int cols);

    void SyncWalkableCells();

    void ApplyVisibility(Player * player);

    Player * GetCellOwner(unsigned int r, unsigned int c) const;

    bool IsCellChanging(unsigned int r, unsigned int c) const;

    void CreateObject(unsigned int layerId, unsigned int objId,
                      unsigned int r0, unsigned int c0,
                      unsigned int rows, unsigned int cols);

    // cell conquest
    bool CanConquestCell(const Cell2D & cell, Player * player);
    void StartConquestCell(const Cell2D & cell, Player * player);
    void ConquestCell(const Cell2D & cell, Player * player);

    // resource generator conquest
    bool CanConquestResourceGenerator(const Cell2D & start, const Cell2D & end, Player * player);
    void StartConquestResourceGenerator(const Cell2D & start, const Cell2D & end, Player * player);
    void ConquestResourceGenerator(const Cell2D & start, const Cell2D & end, Player * player);

    // unit create
    bool CanCreateUnit(GameObject * gen, Player * player);
    Cell2D GetNewUnitDestination(GameObject * gen);
    void StartCreateUnit(const Cell2D & dest, Player * player);
    void CreateUnit(const Cell2D & dest, Player * player);

    // unit destroy
    bool CanDestroyUnit(const Cell2D & cell, Player * player);
    void DestroyUnit(const Cell2D & cell, Player * player);

    // unit upgrade
    bool CanUpgradeUnit(const Cell2D & cell, Player * player);
    void StartUpgradeUnit(const Cell2D & cell, Player * player);
    void UpgradeUnit(const Cell2D & cell);

    // move units
    bool CanUnitMove(const Cell2D & start, const Cell2D & end, Player * player) const;
    bool MoveUnits(const Cell2D & start, const Cell2D & end, Player * player);

    const GameMapCell & GetCell(unsigned int r, unsigned int c) const;
    unsigned int GetNumRows() const;
    unsigned int GetNumCols() const;

    void CheckGameEnd();

private:
    void StopCellChange(GameMapCell & gcell);

    int DefineCellType(const GameMapCell & cell);

    void UpdateLinkedCells(Player * player);

    void UpdateInfluencedCells(int row, int col);

private:
    std::vector<GameMapCell> mCells;
    std::vector<GameObject *> mObjects;

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
 * @brief Checks if a cell is in the process of changing (upgrade in progress).
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
