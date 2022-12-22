#pragma once

#include "GameMapCell.h"

#include <sgl/ai/IPathMap.h>

#include <functional>
#include <unordered_set>
#include <vector>

namespace game
{

class CollectableGenerator;
class ConquerPath;
class Game;
class GameObject;
class IsoMap;
class ObjectPath;
class Player;
class ScreenGame;
class Unit;
class WallBuildPath;

struct Cell2D;
struct GameMapCell;
struct ObjectData;

enum GameObjectType : unsigned int;
enum UnitType : unsigned int;
enum class MapObjectId : unsigned int;

/// Class that handles most of the logic of what happens on the game map.
class GameMap : public sgl::ai::IPathMap
{
public:
    GameMap(Game * game, ScreenGame * sg, IsoMap * isoMap);
    ~GameMap();

    bool HasObject(unsigned int r, unsigned int c) const;

    const std::vector<GameMapCell>  & GetCells() const;
    const std::vector<GameObject *> & GetObjects() const;

    bool IsCellWalkable(unsigned int cellInd) const;
    bool IsCellWalkable(unsigned int r, unsigned int c) const override;
    bool IsAnyNeighborCellWalkable(unsigned int r, unsigned int c) const;

    void SetCellWalkable(unsigned int r, unsigned int c, bool val);

    bool IsCellObjectVisited(unsigned int cellInd) const;
    bool IsCellObjectVisited(unsigned int r, unsigned int c) const;

    void SetSize(unsigned int rows, unsigned int cols);

    void SyncMapCells();

    void ApplyLocalVisibility();
    void ApplyVisibility(Player * player);

    void ApplyLocalVisibilityToObject(GameObject * go);
    void ApplyVisibilityToObject(Player * player, GameObject * go);

    Player * GetCellOwner(unsigned int r, unsigned int c) const;

    bool IsCellChanging(unsigned int r, unsigned int c) const;
    void SetCellChanging(unsigned int r, unsigned int c, bool changing);

    void CreateObjectFromFile(unsigned int layerId, MapObjectId objId,
                              unsigned int r0, unsigned int c0,
                              unsigned int rows, unsigned int cols);

    GameObject * CreateObject(unsigned int layerId, unsigned int objId, Player * owner,
                              unsigned int r0, unsigned int c0,
                              unsigned int rows, unsigned int cols);

    bool HasObject(GameObject * obj) const;

    bool RemoveAndDestroyObject(GameObject * obj);

    bool AreObjectsAdjacent(const GameObject * obj1, const GameObject * obj2) const;
    bool AreCellsAdjacent(const Cell2D & cell1, const Cell2D & cell2) const;

    // cell conquest
    bool CanConquerCell(const Cell2D & cell, Player * player);
    void StartConquerCell(const Cell2D & cell, Player * player);
    void ConquerCell(const Cell2D & cell, Player * player);
    void ConquerCells(ConquerPath * path);
    bool AbortCellConquest(GameObject * obj);

    // structure building
    bool CanBuildStructure(Unit * unit, const Cell2D & cell, Player * player, const ObjectData & data);
    void StartBuildStructure(const Cell2D & cell, Player * player, const ObjectData & data);
    void BuildStructure(const Cell2D & cell, Player * player, const ObjectData & data);

    // wall building
    bool CanBuildWall(const Cell2D & cell, Player * player, unsigned int level);
    void StartBuildWall(const Cell2D & cell, Player * player, unsigned int level);
    void BuildWall(const Cell2D & cell, Player * player, GameObjectType planned);
    void BuildWalls(WallBuildPath * path);
    bool AbortBuildWalls(GameObject * obj);

    // structure conquest
    bool CanConquerStructure(Unit * unit, const Cell2D & end, Player * player);
    void StartConquerStructure(const Cell2D & start, const Cell2D & end, Player * player);
    void AbortConquerStructure(const Cell2D & unitCell, GameObject * target);
    void ConquerStructure(const Cell2D & start, const Cell2D & end, Player * player);

    // unit create
    bool CanCreateUnit(const ObjectData & data, GameObject * gen, Player * player);
    Cell2D GetNewUnitDestination(GameObject * gen);
    void StartCreateUnit(const ObjectData & data, GameObject * gen, const Cell2D & dest, Player * player);
    void CreateUnit(const ObjectData & data, GameObject * gen, const Cell2D & dest, Player * player);

    // unit upgrade
    bool CanUpgradeUnit(GameObject * obj, Player * player);
    void StartUpgradeUnit(GameObject * obj, Player * player);
    void UpgradeUnit(const Cell2D & cell);

    // move units
    bool CanUnitMove(const Cell2D & start, const Cell2D & end, Player * player) const;
    bool MoveUnit(ObjectPath * path);
    bool AbortMove(GameObject * obj);

    Cell2D GetCloseMoveTarget(const Cell2D & start, const Cell2D & end) const;
    Cell2D GetAdjacentMoveTarget(const Cell2D & start, const GameObject * target) const;
    Cell2D GetAdjacentMoveTarget(const Cell2D & start, const Cell2D & targetTL, const Cell2D & targetBR) const;

    const GameMapCell & GetCell(unsigned int r, unsigned int c) const;

    bool MoveObjectDown(GameObject * obj);
    bool MoveObjectUp(GameObject * obj);

    unsigned int GetNumRows() const;
    unsigned int GetNumCols() const;

    void CheckGameEnd();

    void Update(float delta);

private:
    void ClearCell(GameMapCell & gcell);

    void StopCellChange(GameMapCell & gcell);

    void UpdateCellType(unsigned int ind, const GameMapCell & cell);
    int DefineCellType(unsigned int ind, const GameMapCell & cell);

    void UpdateLinkedCells(Player * player);

    void UpdateInfluencedCells(int row, int col);

    bool MoveObjToCell(GameObject * obj, int row, int col);

    Cell2D GetClosestCell(const Cell2D & start, const std::vector<Cell2D> targets) const;

    void DestroyObject(GameObject * obj);

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

    void UpdateObjectPaths(float delta);
    void UpdateConquerPaths(float delta);
    void UpdateWallBuildPaths(float delta);

    void UpdateWalls(const Cell2D & center);
    void UpdateWall(const Cell2D & cell);

private:
    // to access visibility functions
    friend class ObjectPath;
    friend class ConquerPath;

    std::vector<GameMapCell> mCells;
    std::vector<GameObject *> mObjects;
    std::unordered_set<GameObject *> mObjectsSet;
    std::vector<CollectableGenerator *> mCollGen;
    std::vector<ObjectPath *> mPaths;
    std::vector<ConquerPath *> mConquerPaths;
    std::vector<WallBuildPath *> mWallBuildPaths;

    Game * mGame = nullptr;
    ScreenGame * mScreenGame = nullptr;

    IsoMap * mIsoMap = nullptr;

    unsigned int mRows = 0;
    unsigned int mCols = 0;
};

// ==================== INLINE METHODS ====================

inline bool GameMap::HasObject(unsigned int r, unsigned int c) const
{
    const unsigned int ind = r * mCols + c;

    return ind < mCells.size() && mCells[ind].objTop != nullptr;
}

inline const std::vector<GameMapCell> & GameMap::GetCells() const
{
    return mCells;
}

inline const std::vector<GameObject *> & GameMap::GetObjects() const
{
    return mObjects;
}

inline bool GameMap::IsCellWalkable(unsigned int cellInd) const
{
    return mCells[cellInd].walkable;
}

inline bool GameMap::IsCellObjectVisited(unsigned int r, unsigned int c) const
{
    const unsigned int cellInd = r * mCols + c;
    return IsCellObjectVisited(cellInd);
}

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

inline void GameMap::SetCellChanging(unsigned int r, unsigned int c, bool changing)
{
    if(r < mRows && c < mCols)
        mCells[r * mCols + c].changing = changing;
}

inline bool GameMap::HasObject(GameObject * obj) const
{
    return mObjectsSet.find(obj) != mObjectsSet.end();
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
