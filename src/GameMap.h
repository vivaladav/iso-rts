#pragma once

#include "GameMapCell.h"

#include <functional>
#include <vector>

namespace game
{

class Game;
class GameObject;
class IsoMap;
class ObjectPath;
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

    void SyncMapCells();

    void ApplyVisibility(Player * player);

    Player * GetCellOwner(unsigned int r, unsigned int c) const;
    Player * GetObjectOwner(const GameObject * obj) const;

    bool IsCellChanging(unsigned int r, unsigned int c) const;

    void CreateObject(unsigned int layerId, unsigned int objId,
                      unsigned int r0, unsigned int c0,
                      unsigned int rows, unsigned int cols);

    // cell conquest
    bool CanConquerCell(const Cell2D & cell, Player * player);
    void StartConquerCell(const Cell2D & cell, Player * player);
    void ConquerCell(const Cell2D & cell, Player * player);

    // resource generator conquest
    bool CanConquerResourceGenerator(const Cell2D & start, const Cell2D & end, Player * player);
    void StartConquerResourceGenerator(const Cell2D & start, const Cell2D & end, Player * player);
    void ConquerResourceGenerator(const Cell2D & start, const Cell2D & end, Player * player);

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

    void Update(float delta);

private:
    void StopCellChange(GameMapCell & gcell);

    void UpdateCellType(unsigned int ind, const GameMapCell & cell);
    int DefineCellType(unsigned int ind, const GameMapCell & cell);

    void UpdateLinkedCells(Player * player);

    void UpdateInfluencedCells(int row, int col);

    // -- player visibility --
    void AddVisibilityToCell(Player * player, int ind);
    void DelVisibilityToCell(Player * player, int ind);

    void AddPlayerObjVisibility(GameObject * obj, Player * player);
    void DelPlayerObjVisibility(GameObject * obj, Player * player);
    void PropagatePlayerObjVisibility(GameObject * obj, std::function<void(int)> visFun);


    void AddPlayerCellVisibility(const GameMapCell & cell, Player * player);
    void DelPlayerCellVisibility(const GameMapCell & cell, Player * player);
    void PropagatePlayerCellVisibility(const GameMapCell & cell, std::function<void(int)> visFun);

    void PropagatePlayerVisibility(const Cell2D & cell1, const Cell2D & cell0, int visLevel,
                                   std::function<void(int)> visFun);

private:
    // to access visibility functions
    friend class ObjectPath;

    std::vector<GameMapCell> mCells;
    std::vector<GameObject *> mObjects;
    std::vector<ObjectPath *> mPaths;

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
