#include "GameMap.h"

#include "Cell2D.h"
#include "Game.h"
#include "GameConstants.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "IsoObject.h"
#include "Player.h"
#include "AI/ConquerPath.h"
#include "AI/ObjectPath.h"
#include "AI/WallBuildPath.h"
#include "GameObjects/Base.h"
#include "GameObjects/Blobs.h"
#include "GameObjects/BlobsGenerator.h"
#include "GameObjects/DefensiveTower.h"
#include "GameObjects/Diamonds.h"
#include "GameObjects/DiamondsGenerator.h"
#include "GameObjects/PracticeTarget.h"
#include "GameObjects/RadarStation.h"
#include "GameObjects/ResourceGenerator.h"
#include "GameObjects/SceneObject.h"
#include "GameObjects/Unit.h"
#include "GameObjects/Wall.h"
#include "Screens/ScreenGame.h"

#include <utilities/UniformDistribution.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <string>
#include <unordered_set>
#include <vector>

#include <iostream>

namespace game
{

// NOTE these will be replaced by dynamic values soon
constexpr int COST_CONQUEST_CELL = 2;
constexpr int COST_CONQUEST_RES_GEN = 4;

// ==================== PUBLIC METHODS ====================

GameMap::GameMap(Game * game, ScreenGame * sg, IsoMap * isoMap)
    : mGame(game)
    , mScreenGame(sg)
    , mIsoMap(isoMap)
{
    SetSize(isoMap->GetNumRows(), isoMap->GetNumCols());
}

GameMap::~GameMap()
{
    for(GameObject * obj : mObjects)
        delete obj;

    for(CollectableGenerator * cg : mCollGen)
        delete cg;

    for(ObjectPath * op : mPaths)
        delete op;

    for(ConquerPath * cp : mConquerPaths)
        delete cp;
}

bool GameMap::IsCellWalkable(unsigned int r, unsigned int c) const
{
    const unsigned int ind = r * mCols + c;

    return mCells[ind].walkable;
}

bool GameMap::IsAnyNeighborCellWalkable(unsigned int r, unsigned int c) const
{
    const bool checkN = r > 0;
    const bool checkS = r < (mRows - 1);
    const bool checkW = c > 0;
    const bool checkE = c < (mCols - 1);

    return (checkN && IsCellWalkable(r - 1, c)) ||
           (checkS && IsCellWalkable(r + 1, c)) ||
           (checkW && IsCellWalkable(r, c - 1)) ||
           (checkE && IsCellWalkable(r, c + 1)) ||
           (checkN && checkW && IsCellWalkable(r - 1, c - 1)) ||
           (checkN && checkE && IsCellWalkable(r - 1, c + 1)) ||
           (checkS && checkW && IsCellWalkable(r + 1, c - 1)) ||
           (checkS && checkE && IsCellWalkable(r + 1, c + 1));
}

void GameMap::SetCellWalkable(unsigned int r, unsigned int c, bool val)
{
    const unsigned int ind = r * mCols + c;

    mCells[ind].walkable = val;
}

bool GameMap::IsCellObjectVisited(unsigned int cellInd) const
{
    if(cellInd < mRows * mCols)
        return mCells[cellInd].obj && mCells[cellInd].obj->IsVisited();
    else
        return false;
}

void GameMap::SetSize(unsigned int rows, unsigned int cols)
{
    const unsigned int size = rows * cols;

    if(size == mCells.size())
        return ;

    mRows = rows;
    mCols = cols;

    mCells.resize(size);

    // set cell coordinates
    int index = 0;

    for(unsigned int r = 0; r < rows; ++r)
    {
        for(unsigned int c = 0; c < cols; ++c)
        {
            GameMapCell & cell = mCells[index++];

            cell.row = r;
            cell.col = c;
        }
    }

    // init player(s) visibility map
    // NOTE for now only for human player
    mGame->GetPlayerByIndex(0)->InitVisibility(mRows, mCols);
}

void GameMap::SyncMapCells()
{
    const unsigned int size = mRows * mCols;

    for(unsigned int i = 0; i < size; ++i)
    {
        const auto type = static_cast<CellTypes>(mIsoMap->GetCellType(i));

        mCells[i].currType = type;
        mCells[i].basicType = type;

        // DIAMONDS GENERATOR
        if(DIAMONDS_SOURCE == type)
        {
           const int row = i / mCols;
           const int col = i % mCols;

           auto dg = new DiamondsGenerator(this);
           dg->SetCell(row, col);

           mCollGen.emplace_back(dg);
        }
        else if(BLOBS_SOURCE == type)
        {
           const int row = i / mCols;
           const int col = i % mCols;

           auto dg = new BlobsGenerator(this);
           dg->SetCell(row, col);

           mCollGen.emplace_back(dg);
        }
    }
}

void GameMap::ApplyLocalVisibility()
{
    Player * p = mGame->GetLocalPlayer();

    ApplyVisibility(p);
}

void GameMap::ApplyVisibility(Player * player)
{
    // update cells
    const unsigned int totCells = mRows * mCols;

    for(unsigned int ind = 0; ind < totCells; ++ind)
    {
        const GameMapCell & cell = mCells[ind];
        UpdateCellType(ind, cell);
    }

    // update objects
    for(GameObject * go : mObjects)
        ApplyVisibilityToObject(player, go);
}

void GameMap::ApplyLocalVisibilityToObject(GameObject * go)
{
    Player * p = mGame->GetLocalPlayer();

    ApplyVisibilityToObject(p, go);
}

void GameMap::ApplyVisibilityToObject(Player * player, GameObject * go)
{
    const int rTL = go->GetRow1();
    const int cTL = go->GetCol1();
    const int rBR = go->GetRow0();
    const int cBR = go->GetCol0();

    IsoObject * obj = go->GetIsoObject();
    IsoLayer * layer = obj->GetLayer();

    bool visible = false;

    for(int r = rTL; r <= rBR; ++r)
    {
        const int indBase = r * mCols;

        for(int c = cTL; c <= cBR; ++c)
        {
            const int ind = indBase + c;

            if(player->IsCellVisible(ind))
            {
                visible = true;
                break;
            }

            if(visible)
                break;
        }
    }

    // update visibility if status changed
    if(visible != go->IsVisible())
    {
        // hide objects if not visited or not a structure
        if(!visible && (!go->IsVisited() || !go->IsStructure()))
            layer->SetObjectVisible(obj, false);
        else
            layer->SetObjectVisible(obj, true);

        go->SetVisible(visible);
    }

    // update visited flag independently to avoid problems on init
    if(visible)
        go->SetVisited();
}

void GameMap::CreateObjectFromFile(unsigned int layerId, MapObjectId objId,
                                   unsigned int r0, unsigned int c0,
                                   unsigned int rows, unsigned int cols)
{
    if(objId >= MapObjectId::BASE_P1 && objId <= MapObjectId::BASE_P3)
    {
        const int playerInd = static_cast<int>(objId) - static_cast<int>(MapObjectId::BASE_P1);

        Player * owner =  mGame->GetPlayerByIndex(playerInd);

        // no player in this map
        if(nullptr == owner)
            return ;

        CreateObject(layerId, OBJ_BASE, owner, r0, c0, rows, cols);
    }
    else if(MapObjectId::GEN_ENERGY == objId)
        CreateObject(layerId, OBJ_RES_GEN_ENERGY, nullptr, r0, c0, rows, cols);
    else if(MapObjectId::GEN_MATERIAL1 == objId)
        CreateObject(layerId, OBJ_RES_GEN_MATERIAL1, nullptr, r0, c0, rows, cols);
    else if(MapObjectId::RADAR_STATION == objId)
        CreateObject(layerId, OBJ_RADAR_STATION, nullptr, r0, c0, rows, cols);
    else if(objId >= MapObjectId::MOUNTAIN_FIRST && objId <= MapObjectId::MOUNTAIN_LAST)
    {
        const int objInd = static_cast<int>(objId) - static_cast<int>(MapObjectId::MOUNTAIN_FIRST);

        CreateObject(layerId, OBJ_ROCKS_FIRST + objInd, nullptr, r0, c0, rows, cols);
    }
    else if(MapObjectId::PRACTICE_TARGET == objId)
        CreateObject(layerId, OBJ_PRACTICE_TARGET, nullptr, r0, c0, rows, cols);
}

GameObject * GameMap::CreateObject(unsigned int layerId, unsigned int objId, Player * owner,
                                   unsigned int r0, unsigned int c0,
                                   unsigned int rows, unsigned int cols)
{
    // object origin is out of map
    if(r0 >= mRows || c0 >= mCols)
        return nullptr;

    // full size is out of map
    const unsigned int r1 = 1 + r0 - rows;
    const unsigned int c1 = 1 + c0 - cols;

    if(r1 >= mRows || c1 >= mCols)
        return nullptr;

    const unsigned int ind0 = r0 * mCols + c0;

    GameMapCell & gcell = mCells[ind0];

    // cell is already full
    if(gcell.obj)
        return nullptr;

    // create game object
    GameObject * obj = nullptr;

    if(OBJ_RES_GEN_ENERGY == objId)
        obj = new ResourceGenerator(ResourceType::ENERGY, rows, cols);
    else if(OBJ_RES_GEN_MATERIAL1 == objId)
        obj = new ResourceGenerator(ResourceType::MATERIAL1, rows, cols);
    else if(OBJ_BASE == objId)
    {
        obj = new Base(rows, cols);

        // base cells update
        for(unsigned int r = r1; r <= r0; ++r)
        {
            const unsigned int indBase = r * mCols;

            for(unsigned int c = c1; c <= c0; ++c)
            {
                const unsigned int ind = indBase + c;
                mCells[ind].owner = owner;
                mCells[ind].linked = true;

                UpdateInfluencedCells(r, c);
            }
        }

        owner->SetBaseCell(Cell2D(r0, c0));
        owner->SumCells(rows * cols);
    }
    else if(OBJ_RADAR_STATION == objId)
        obj = new RadarStation(rows, cols);
    else if(OBJ_PRACTICE_TARGET == objId)
        obj = new PracticeTarget(rows, cols);
    else if(objId >= OBJ_ROCKS_FIRST && objId <= OBJ_ROCKS_LAST)
        obj = new SceneObject(static_cast<GameObjectType>(objId), rows, cols);
    else if(objId >= OBJ_WALL_FIRST && objId <= OBJ_WALL_LAST)
        obj = new Wall(static_cast<GameObjectType>(objId), rows, cols);
    else if(OBJ_DEF_TOWER == objId)
        obj = new DefensiveTower(rows, cols);
    else if(OBJ_DIAMONDS == objId)
        obj = new Diamonds;
    else if(OBJ_BLOBS == objId)
        obj  = new Blobs;

    // assign owner
    obj->SetOwner(owner);

    // set object properties
    obj->SetCell(&mCells[ind0]);
    obj->SetSize(rows, cols);

    // links to other objects
    obj->SetGameMap(this);
    obj->SetScreen(mScreenGame);

    // store object in map list and in registry
    mObjects.push_back(obj);
    mObjectsSet.insert(obj);

    // generic cells update
    for(unsigned int r = r1; r <= r0; ++r)
    {
        const unsigned int indBase = r * mCols;

        for(unsigned int c = c1; c <= c0; ++c)
        {
            const unsigned int ind = indBase + c;

            GameMapCell & cell = mCells[ind];

            cell.walkable = false;
            cell.obj = obj;

            // update cell image
            UpdateCellType(ind, cell);
        }
    }

    // create object in iso map
    mIsoMap->GetLayer(layerId)->AddObject(obj->GetIsoObject(), r0, c0);

    // update visibility map
    // NOTE only for human player for now
    Player * localPlayer = mGame->GetLocalPlayer();

    if(owner == localPlayer)
        AddPlayerObjVisibility(obj, localPlayer);

    return obj;
}

bool GameMap::RemoveAndDestroyObject(GameObject * obj)
{
    auto it = std::find(mObjects.begin(), mObjects.end(), obj);

    // object not found
    if(mObjects.end() == it)
        return false;

    // remove from objects list and set and then destroy it
    mObjects.erase(it);
    mObjectsSet.erase(obj);

    DestroyObject(obj);

    return true;
}

bool GameMap::AreObjectsAdjacent(const GameObject * obj1, const GameObject * obj2) const
{
    // expand obj1 area by 1
    const int expRowTL = obj1->GetRow1() - 1;
    const int expColTL = obj1->GetCol1() - 1;
    const int expRowBR = obj1->GetRow0() + 1;
    const int expColBR = obj1->GetCol0() + 1;

    // check if expanded area and obj2 intersect
    return expRowTL <= obj2->GetRow0() &&
           obj2->GetRow1() <= expRowBR &&
           expColTL <= obj2->GetCol0() &&
           obj2->GetCol1() <= expColBR;
}

bool GameMap::AreCellsAdjacent(const Cell2D & cell1, const Cell2D & cell2) const
{
    const int distR = std::abs(cell1.row - cell2.row);
    const int distC = std::abs(cell1.col - cell2.col);

    const int maxDist = 1;

    return distR <= maxDist && distC <= maxDist;
}

bool GameMap::CanConquerCell(const Cell2D & cell, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);

    // out of bounds
    if(!(r < mRows && c < mCols))
        return false;

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    // already changing
    if(gcell.changing)
        return false;

    // player already owns the cell
    if(gcell.owner == player)
        return false;

    // check if player has enough energy
    if(COST_CONQUEST_CELL > player->GetStat(Player::Stat::ENERGY).GetIntValue())
        return false;

    return true;
}

void GameMap::StartConquerCell(const Cell2D & cell, Player * player)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    // take player's energy
    player->GetStat(Player::Stat::ENERGY).SumValue(-COST_CONQUEST_CELL);

    // mark cell as changing
    gcell.changing = true;
}

void GameMap::ConquerCell(const Cell2D & cell, Player * player)
{
    // check if cell was of another faction
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];
    bool stolen = gcell.owner != nullptr && gcell.owner != player;

    // make cell empty
    ClearCell(gcell);

    // assign owner
    gcell.owner = player;

    // update player
    player->SumCells(1);

    // reset cell's changing flag
    gcell.changing = false;

    // propagate effects of conquest
    UpdateInfluencedCells(cell.row, cell.col);

    UpdateLinkedCells(player);

    // update visibility map if local player
    if(player == mGame->GetPlayerByIndex(0))
    {
        AddPlayerCellVisibility(gcell, player);

        ApplyVisibility(player);
    }
    else if(stolen)
    {
        DelPlayerCellVisibility(gcell, player);

        ApplyVisibility(player);
    }
}

void GameMap::ConquerCells(ConquerPath * path)
{
    path->Start();

    mConquerPaths.emplace_back(path);
}

bool GameMap::AbortCellConquest(GameObject * obj)
{
    for(auto path : mConquerPaths)
    {
        if(path->GetObject() == obj)
        {
            path->Abort();
            return true;
        }
    }

    return false;
}

bool GameMap::CanBuildStructure(const Cell2D & cell, Player * player, GameObjectType structure)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);

    // out of bounds
    if(!(r < mRows && c < mCols))
        return false;

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    // already changing
    if(gcell.changing)
        return false;

    // cell is full
    if(!gcell.walkable)
        return false;

    // TODO check cost

    return true;
}

void GameMap::StartBuildStructure(const Cell2D & cell, Player * player, GameObjectType structure)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    // TODO take player's material
    //player->GetStat(Player::Stat::ENERGY).SumValue(-COST_CONQUEST_CELL);

    // mark cell as changing
    gcell.changing = true;
}

void GameMap::BuildStructure(const Cell2D & cell, Player * player, GameObjectType structure)
{
    // check if cell was of another faction
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];
    bool stolen = gcell.owner != nullptr && gcell.owner != player;

    // make cell empty
    ClearCell(gcell);

    // assign owner
    gcell.owner = player;

    // update player
    player->SumCells(1);

    // reset cell's changing flag
    gcell.changing = false;
    // propagate effects of conquest
    UpdateInfluencedCells(cell.row, cell.col);

    UpdateLinkedCells(player);

    // add object wall
    CreateObject(OBJECTS, structure, player, cell.row, cell.col, 1, 1);

    // update this wall type and the ones surrounding it
    UpdateWalls(cell);

    // update visibility map if local player
    if(player == mGame->GetPlayerByIndex(0))
    {
        AddPlayerCellVisibility(gcell, player);

        ApplyVisibility(player);
    }
    else if(stolen)
    {
        DelPlayerCellVisibility(gcell, player);

        ApplyVisibility(player);
    }
}

bool GameMap::CanBuildWall(const Cell2D & cell, Player * player, unsigned int level)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);

    // out of bounds
    if(!(r < mRows && c < mCols))
        return false;

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    // already changing
    if(gcell.changing)
        return false;

    // cell is full
    if(!gcell.walkable)
        return false;

    // check if player has enough energy and material
    const int costMat = Wall::GetCostMaterial(level);
    const int costEne = Wall::GetCostEnergy(level);

    return player->GetStat(Player::Stat::MATERIAL).GetIntValue() >= costMat  &&
           player->GetStat(Player::Stat::ENERGY).GetIntValue() >= costEne;
}

void GameMap::StartBuildWall(const Cell2D & cell, Player * player, unsigned int level)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    // take player's material
    const int costEne = Wall::GetCostEnergy(level);
    player->GetStat(Player::Stat::ENERGY).SumValue(-costEne);

    const int costMat = Wall::GetCostMaterial(level);
    player->GetStat(Player::Stat::MATERIAL).SumValue(-costMat);

    // mark cell as changing
    gcell.changing = true;
}

void GameMap::BuildWall(const Cell2D & cell, Player * player, GameObjectType planned)
{
    // check if cell was of another faction
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];
    bool stolen = gcell.owner != nullptr && gcell.owner != player;

    // make cell empty
    ClearCell(gcell);

    // assign owner
    gcell.owner = player;

    // update player
    player->SumCells(1);

    // reset cell's changing flag
    gcell.changing = false;
    // propagate effects of conquest
    UpdateInfluencedCells(cell.row, cell.col);

    UpdateLinkedCells(player);

    // add object wall
    CreateObject(OBJECTS, planned, player, cell.row, cell.col, 1, 1);

    // update this wall type and the ones surrounding it
    UpdateWalls(cell);

    // update visibility map if local player
    if(player == mGame->GetPlayerByIndex(0))
    {
        AddPlayerCellVisibility(gcell, player);

        ApplyVisibility(player);
    }
    else if(stolen)
    {
        DelPlayerCellVisibility(gcell, player);

        ApplyVisibility(player);
    }
}

void GameMap::BuildWalls(WallBuildPath * path)
{
    path->Start();

    mWallBuildPaths.emplace_back(path);
}

bool GameMap::AbortBuildWalls(GameObject * obj)
{
    for(auto path : mWallBuildPaths)
    {
        if(path->GetObject() == obj)
        {
            path->Abort();
            return true;
        }
    }

    return false;
}

bool GameMap::CanConquerStructure(const Cell2D & start, const Cell2D & end, Player * player)
{
    const unsigned int r0 = static_cast<unsigned int>(start.row);
    const unsigned int c0 = static_cast<unsigned int>(start.col);
    const unsigned int r1 = static_cast<unsigned int>(end.row);
    const unsigned int c1 = static_cast<unsigned int>(end.col);

    const int ind0 = r0 * mCols + c0;
    GameMapCell & gcell0 = mCells[ind0];

    const Unit * unit = gcell0.GetUnit();

    // start has no unit
    if(nullptr == unit)
        return false;

    // not player's unit
    if(unit->GetOwner() != player)
        return false;

    const int ind1 = r1 * mCols + c1;
    GameMapCell & gcell1 = mCells[ind1];

    // end is empty
    if(nullptr == gcell1.obj)
        return false;

    // target object can't be conquered
    if(!gcell1.obj->CanBeConquered())
        return false;

    // player already owns the res gen
    if(gcell1.owner == player)
        return false;

    // TEMP - no conquest while another is in progress
    if(gcell1.changing)
        return false;

    return true;
}

void GameMap::StartConquerStructure(const Cell2D & start, const Cell2D & end, Player * player)
{
    // take player's energy
    player->GetStat(Player::Stat::ENERGY).SumValue(-COST_CONQUEST_RES_GEN);

    // mark start as changing
    const int ind0 = start.row * mCols + start.col;
    mCells[ind0].changing = true;

    // mark object cells as changing
    const int ind1 = end.row * mCols + end.col;
    GameObject * obj = mCells[ind1].obj;

    for(int r = obj->GetRow1(); r <= obj->GetRow0(); ++r)
    {
        const unsigned int indBase = r * mCols;

        for(int c = obj->GetCol1(); c <= obj->GetCol0(); ++c)
        {
            const unsigned int ind = indBase + c;
            mCells[ind].changing = true;
        }
    }
}

void GameMap::AbortConquerStructure(const Cell2D & unitCell, GameObject * target)
{
    // mark start as not changing
    const int ind0 = unitCell.row * mCols + unitCell.col;
    mCells[ind0].changing = false;

    // mark object cells as not changing
    for(int r = target->GetRow1(); r <= target->GetRow0(); ++r)
    {
        const unsigned int indBase = r * mCols;

        for(int c = target->GetCol1(); c <= target->GetCol0(); ++c)
        {
            const unsigned int ind = indBase + c;
            mCells[ind].changing = false;
        }
    }

    // stop progress bar
    mScreenGame->CancelProgressBar(unitCell);
}

void GameMap::ConquerStructure(const Cell2D & start, const Cell2D & end, Player * player)
{
    const int ind = end.row * mCols + end.col;
    GameMapCell & gcell1 = mCells[ind];

    GameObject * obj = gcell1.obj;

    // assign owner to cells
    for(int r = obj->GetRow1(); r <= obj->GetRow0(); ++r)
    {
        const unsigned int indBase = r * mCols;

        for(int c = obj->GetCol1(); c <= obj->GetCol0(); ++c)
        {
            const unsigned int ind = indBase + c;
            mCells[ind].owner = player;
            mCells[ind].changing = false;

            UpdateInfluencedCells(r, c);
        }
    }

    // assign owner to object
    obj->SetOwner(player);

    // update player
    player->SumCells(1);
    player->AddResourceGenerator(ind, static_cast<ResourceGenerator *>(gcell1.obj));

    // reset start changing flag
    const int ind0 = start.row * mCols + start.col;
    mCells[ind0].changing = false;

    // update map
    UpdateLinkedCells(player);

    // update visibility
    Player * localPlayer = mGame->GetPlayerByIndex(0);

    if(player == localPlayer)
    {
        AddPlayerObjVisibility(obj, localPlayer);

        // TODO this should be optimized as it's affecting the whole map
        // when only a small portion is changed
        ApplyVisibility(localPlayer);
    }
}

bool GameMap::CanCreateUnit(const UnitData & data, GameObject * gen, Player * player)
{
    // generator is not owned by the player
    if(gen->GetOwner() != player)
        return false;

    // only base can generate units (for now)
    if(gen->GetObjectType() != OBJ_BASE)
        return false;

    // generator is already busy
    if(gen->IsBusy())
       return false;

    // check if player has enough resources
    if(data.costEnergy > player->GetStat(Player::Stat::ENERGY).GetIntValue() ||
       data.costMaterial > player->GetStat(Player::Stat::MATERIAL).GetIntValue())
        return false;

    // check if there's at least 1 free cell where to place the new unit
    // NOTE this must be the last test or the code below needs to be changed
    const int r1 = gen->GetRow1() > 0 ? gen->GetRow1() - 1 : 0;
    const int c1 = gen->GetCol1() > 0 ? gen->GetCol1() - 1 : 0;
    const int r0 = gen->GetRow0() < static_cast<int>(mRows - 1) ? gen->GetRow0() + 1 : mRows - 1;
    const int c0 = gen->GetCol0() < static_cast<int>(mCols - 1) ? gen->GetCol0() + 1 : mCols - 1;

    const int indBaseTop = r1 * mCols;
    const int indBaseBottom = r0 * mCols;

    // NOTE for simplicity corner cells are overlapping and sometimes checked twice.
    // This can be optimized, but it's probably not worth it for now.

    // check right (top to bottom)
    for(int r = r1; r <= r0; ++r)
    {
        if(mCells[r * mCols + c0].walkable)
            return true;
    }

    // check top (left to right)
    for(int c = c1; c <= c0; ++c)
    {
        if(mCells[indBaseTop + c].walkable)
            return true;
    }

    // check left (bottom to top)
    for(int r = r0; r >= r1; --r)
    {
        if(mCells[r * mCols + c1].walkable)
            return true;
    }

    // check bottom (left to right)
    for(int c = c1; c <= c0; ++c)
    {
        if(mCells[indBaseBottom + c].walkable)
            return true;
    }

    // free cell test failed
    return false;
}

Cell2D GameMap::GetNewUnitDestination(GameObject * gen)
{
    const int r1 = gen->GetRow1() > 0 ? gen->GetRow1() - 1 : 0;
    const int c1 = gen->GetCol1() > 0 ? gen->GetCol1() - 1 : 0;
    const int r0 = gen->GetRow0() < static_cast<int>(mRows - 1) ? gen->GetRow0() + 1 : mRows - 1;
    const int c0 = gen->GetCol0() < static_cast<int>(mCols - 1) ? gen->GetCol0() + 1 : mCols - 1;

    const int indBaseTop = r1 * mCols;
    const int indBaseBottom = r0 * mCols;

    const int halfRows = mRows / 2;
    const int halfCols = mCols / 2;

    // NOTE for simplicity corner cells are overlapping and sometimes checked twice.
    // This can be optimized, but it's probably not worth it for now.


    // BOTTOM of the map
    if(r0 > halfRows)
    {
        // LEFT of the map
        if(c0 < halfCols)
        {
            // check right (top to bottom)
            for(int r = r1; r <= r0; ++r)
            {
                if(mCells[r * mCols + c0].walkable)
                    return Cell2D(r, c0);
            }

            // check top (right to left)
            for(int c = c0; c >= c1; --c)
            {
                if(mCells[indBaseTop + c].walkable)
                    return Cell2D(r1, c);
            }

            // check bottom (right to left)
            for(int c = c0; c >= c1; --c)
            {
                if(mCells[indBaseBottom + c].walkable)
                    return Cell2D(r0, c);
            }

            // check left (top to bottom)
            for(int r = r1; r <= r0; ++r)
            {
                if(mCells[r * mCols + c1].walkable)
                    return Cell2D(r, c1);
            }
        }
        // RIGHT of the map
        else
        {
            // check left (top to bottom)
            for(int r = r1; r <= r0; ++r)
            {
                if(mCells[r * mCols + c1].walkable)
                    return Cell2D(r, c1);
            }

            // check top (left to right)
            for(int c = c1; c <= c0; ++c)
            {
                if(mCells[indBaseTop + c].walkable)
                    return Cell2D(r1, c);
            }

            // check right (top to bottom)
            for(int r = r1; r <= r0; ++r)
            {
                if(mCells[r * mCols + c0].walkable)
                    return Cell2D(r, c0);
            }

            // check bottom (left to right)
            for(int c = c1; c <= c0; ++c)
            {
                if(mCells[indBaseBottom + c].walkable)
                    return Cell2D(r0, c);
            }
        }
    }
    // TOP of the map
    else
    {
        // LEFT of the map
        if(c0 < halfCols)
        {
            // check right (bottom to top)
            for(int r = r0; r >= r1; --r)
            {
                if(mCells[r * mCols + c0].walkable)
                    return Cell2D(r, c0);
            }

            // check bottom (right to left)
            for(int c = c0; c >= c1; --c)
            {
                if(mCells[indBaseBottom + c].walkable)
                    return Cell2D(r0, c);
            }

            // check top (right to left)
            for(int c = c0; c >= c1; --c)
            {
                if(mCells[indBaseTop + c].walkable)
                    return Cell2D(r1, c);
            }

            // check left (bottom to top)
            for(int r = r0; r >= r1; --r)
            {
                if(mCells[r * mCols + c1].walkable)
                    return Cell2D(r, c1);
            }
        }
        // RIGHT of the map
        else
        {
            // check left (bottom to top)
            for(int r = r0; r >= r1; --r)
            {
                if(mCells[r * mCols + c1].walkable)
                    return Cell2D(r, c1);
            }

            // check bottom (left to right)
            for(int c = c1; c <= c0; ++c)
            {
                if(mCells[indBaseBottom + c].walkable)
                    return Cell2D(r0, c);
            }

            // check top (left to right)
            for(int c = c1; c <= c0; ++c)
            {
                if(mCells[indBaseTop + c].walkable)
                    return Cell2D(r1, c);
            }

            // check right (bottom to top)
            for(int r = r0; r >= r1; --r)
            {
                if(mCells[r * mCols + c0].walkable)
                    return Cell2D(r, c0);
            }
        }
    }

    // failed to find a spot
    return Cell2D(-1, -1);
}

void GameMap::StartCreateUnit(const UnitData & data, GameObject * gen, const Cell2D & dest, Player * player)
{
    const int ind = dest.row * mCols + dest.col;
    GameMapCell & gcell = mCells[ind];

    // make player pay
    player->GetStat(Player::Stat::ENERGY).SumValue(-data.costEnergy);
    player->GetStat(Player::Stat::MATERIAL).SumValue(-data.costMaterial);

    // mark cell as changing
    gcell.changing = true;

    // mark generator as busy
    gen->SetBusy(true);
}

void GameMap::CreateUnit(const UnitData & data, GameObject * gen, const Cell2D & dest, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(dest.row);
    const unsigned int c = static_cast<unsigned int>(dest.col);

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    Unit * unit = new Unit(data, 1, 1);
    unit->SetOwner(player);
    unit->SetCell(&mCells[ind]);

    // links to other objects
    unit->SetGameMap(this);
    unit->SetScreen(mScreenGame);

    // update cell
    gcell.obj = unit;
    gcell.walkable = false;
    gcell.changing = false;

    mIsoMap->GetLayer(OBJECTS)->AddObject(unit->GetIsoObject(), r, c);

    mObjects.push_back(unit);

    // update generator
    gen->SetBusy(false);

    // update player
    player->SumUnits(1);
    player->SumTotalUnitsLevel(unit->GetUnitLevel() + 1);

    // update visibility map
    // NOTE only for human player for now
    Player * localPlayer = mGame->GetLocalPlayer();

    if(unit->GetOwner() == localPlayer)
        AddPlayerObjVisibility(unit, localPlayer);

    ApplyVisibility(localPlayer);
}

bool GameMap::CanUpgradeUnit(GameObject * obj, Player * player)
{
    // this should never happen
    if(nullptr == obj)
        return false;

    // object is not an unit
    if(obj->GetObjectType() != OBJ_UNIT)
        return false;

    auto unit = static_cast<Unit *>(obj);

    // check if reached max level for units
    const int unitLevel = unit->GetUnitLevel();

    if(MAX_UNITS_LEVEL == unitLevel)
        return false;

    // check if player has enough energy
    const int cost = COST_UNIT_UPGRADE[unitLevel];

    if(cost > player->GetStat(Player::Stat::ENERGY).GetIntValue())
        return false;

    return true;
}

void GameMap::StartUpgradeUnit(GameObject * obj, Player * player)
{
    const int ind = obj->GetRow0() * mCols + obj->GetCol0();
    GameMapCell & gcell = mCells[ind];

    // make player pay
    const Unit * unit = static_cast<Unit *>(obj);
    const int unitLevel = unit->GetUnitLevel();
    const int cost = -COST_UNIT_UPGRADE[unitLevel];
    player->GetStat(Player::Stat::ENERGY).SumValue(cost);

    // mark cell as changing
    gcell.changing = true;
}

void GameMap::UpgradeUnit(const Cell2D & cell)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);
    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    Unit * unit = gcell.GetUnit();
    unit->IncreaseUnitLevel();

    // update player
    gcell.owner->SumTotalUnitsLevel(1);

    // reset cell's changing flag
    gcell.changing = false;
}

bool GameMap::CanUnitMove(const Cell2D & start, const Cell2D & end, Player * player) const
{
    const unsigned int r0 = static_cast<unsigned int>(start.row);
    const unsigned int c0 = static_cast<unsigned int>(start.col);

    // start out of bounds
    if(!(r0 < mRows && c0 < mCols))
        return false;

    const unsigned int r1 = static_cast<unsigned int>(end.row);
    const unsigned int c1 = static_cast<unsigned int>(end.col);

    // end out of bounds
    if(!(r1 < mRows && c1 < mCols))
        return false;

    const int diffR = abs(end.row - start.row);
    const int diffC = abs(end.col - start.col);

    // end too far - units can only move to next cell
    if(diffR > 1 || diffC > 1)
        return false;

    const int ind0 = r0 * mCols + c0;
    const GameMapCell & gcell0 = mCells[ind0];
    const Unit * unit0 = gcell0.GetUnit();

    // start has no units
    if(nullptr == unit0)
        return false;

    // trying to move an enemy unit
    if(unit0->GetOwner() != player)
        return false;

    const int ind1 = r1 * mCols + c1;
    const GameMapCell & gcell1 = mCells[ind1];

    // end not a walkable cell
    if(!gcell1.walkable)
        return false;

    const Unit * unit1 = gcell1.GetUnit();

    // fail if destination is full or has different level units
    if(unit1 != nullptr)
        return false;

    // all good
    return true;
}

bool GameMap::MoveUnit(ObjectPath * path)
{
    GameObject * obj = path->GetObject();

    const int ind = obj->GetRow0() * mCols + obj->GetCol0();

    // object is not in its cell !?
    if(mCells[ind].obj != obj)
        return false;

    // start path
    path->Start();
    mPaths.emplace_back(path);

    return true;
}

bool GameMap::AbortMove(GameObject * obj)
{
    for(auto path : mPaths)
    {
        if(path->GetObject() == obj)
        {
            path->Abort();
            return true;
        }
    }

    return false;
}

Cell2D GameMap::GetCloseMoveTarget(const Cell2D & start, const Cell2D & end) const
{
    // get all walkable cells around end
    const int rowTL = end.row - 1 > 0 ? end.row - 1 : 0;
    const int colTL = end.col - 1 > 0 ? end.col - 1 : 0;
    const int rowBR = end.row + 1 < static_cast<int>(mRows - 1) ? end.row + 1 : mRows - 1;
    const int colBR = end.col + 1 < static_cast<int>(mCols - 1) ? end.col + 1 : mCols - 1;

    std::vector<Cell2D> walkalbes;
    const int maxWalkables = 8;
    walkalbes.reserve(maxWalkables);

    for(int r = rowTL; r <= rowBR; ++r)
    {
        const int indBase = r * mCols;

        for(int c = colTL; c <= colBR; ++c)
        {
            const int ind = indBase + c;

            if(mCells[ind].walkable)
                walkalbes.emplace_back(r, c);
        }
    }

    return GetClosestCell(start, walkalbes);
}

Cell2D GameMap::GetAdjacentMoveTarget(const Cell2D & start, const GameObject * target) const
{
    const Cell2D tl(target->GetRow1(), target->GetCol1());
    const Cell2D br(target->GetRow0(), target->GetCol0());

    return GetAdjacentMoveTarget(start, tl, br);
}

Cell2D GameMap::GetAdjacentMoveTarget(const Cell2D & start, const Cell2D & targetTL, const Cell2D & targetBR) const
{
    // get all walkable cells around target
    const int tRows = targetBR.row - targetTL.row + 1;
    const int tCols = targetBR.col - targetTL.col + 1;

    const int rowTL = targetTL.row - 1 > 0 ? targetTL.row - 1 : 0;
    const int colTL = targetTL.col - 1 > 0 ? targetTL.col - 1 : 0;
    const int rowBR = targetBR.row + 1 < static_cast<int>(mRows - 1) ? targetBR.row + 1 : mRows - 1;
    const int colBR = targetBR.col + 1 < static_cast<int>(mCols - 1) ? targetBR.col + 1 : mCols - 1;

    std::vector<Cell2D> walkalbes;
    const int maxWalkables = (tCols + 2) * 2 + tRows;
    walkalbes.reserve(maxWalkables);

    for(int r = rowTL; r <= rowBR; ++r)
    {
        const int indBase = r * mCols;

        for(int c = colTL; c <= colBR; ++c)
        {
            const int ind = indBase + c;

            if(mCells[ind].walkable)
                walkalbes.emplace_back(r, c);
        }
    }

    return GetClosestCell(start, walkalbes);
}

void GameMap::CheckGameEnd()
{
    const int numPlayers = mGame->GetNumPlayers();
    int defeated = 0;

    // check for game over and defeated opponents
    for(int i = 0; i < numPlayers; ++i)
    {
        const Player * p = mGame->GetPlayerByIndex(i);

        if(p->GetNumCells() == 0)
        {
            if(p->IsLocal())
            {
                mScreenGame->GameOver();
                return;
            }
            else
                ++defeated;
        }
    }

    // check for victory - assuming players is always > 1
    if(defeated == (numPlayers - 1))
        mScreenGame->GameWon();
}

void GameMap::Update(float delta)
{
    // -- game objects --
    auto itObj = mObjects.begin();

    while(itObj != mObjects.end())
    {
        GameObject * obj = *itObj;

        obj->Update(delta);

        if(obj->IsDestroyed())
        {
            GameObject * obj = *itObj;

            // erase object from vector and set
            itObj = mObjects.erase(itObj);
            mObjectsSet.erase(obj);

            DestroyObject(obj);
        }
        else
            ++itObj;
    }

    for(CollectableGenerator * dg : mCollGen)
        dg->Update(delta);

    // paths
    UpdateObjectPaths(delta);

    // conquer paths
    UpdateConquerPaths(delta);

    // wall building paths
    UpdateWallBuildPaths(delta);
}

// ==================== PRIVATE METHODS ====================

void GameMap::ClearCell(GameMapCell & gcell)
{
    // destroy any generator
    auto it = std::find_if(mCollGen.begin(), mCollGen.end(), [gcell](CollectableGenerator * gen)
    {
        return gen->GetRow() == gcell.row && gen->GetCol() == gcell.col;
    });


    if(it != mCollGen.end())
    {
        delete *it;
        mCollGen.erase(it);
    }

    gcell.currType = EMPTY;
}

void GameMap::StopCellChange(GameMapCell & gcell)
{
    gcell.changing = false;

    const Cell2D cell(gcell.row, gcell.col);
    mScreenGame->CancelProgressBar(cell);
}

void GameMap::UpdateCellType(unsigned int ind, const GameMapCell & cell)
{
    const int cellType = DefineCellType(ind, cell);
    mIsoMap->SetCellType(ind, cellType);
}

int GameMap::DefineCellType(unsigned int ind, const GameMapCell & cell)
{
    // if cell is not visible it's always Fog Of War
    if(!mGame->GetPlayerByIndex(0)->IsCellVisible(ind))
        return FOG_OF_WAR;

    // scene cell
    if(SCENE_ROCKS == cell.currType || DIAMONDS_SOURCE == cell.currType || BLOBS_SOURCE == cell.currType)
        return cell.currType;

    const PlayerFaction ownerFaction = cell.owner ? cell.owner->GetFaction() : NO_FACTION;

    int type = EMPTY;

    switch(ownerFaction)
    {
        case FACTION_1:
            if(cell.linked)
                type = F1_CONNECTED;
            else
                type = F1;
        break;

        case FACTION_2:
            if(cell.linked)
                type = F2_CONNECTED;
            else
                type = F2;
        break;

        case FACTION_3:
            if(cell.linked)
                type = F3_CONNECTED;
            else
                type = F3;
        break;

        // no owner
        default:
        {
            if(0 == cell.influencer)
                type = F1_INFLUENCED;
            else if(1 == cell.influencer)
                type = F2_INFLUENCED;
            else if(2 == cell.influencer)
                type = F3_INFLUENCED;
            // no influence
            else
                type = cell.basicType;
        }
        break;
    }

    return type;
}

void GameMap::UpdateLinkedCells(Player * player)
{
    std::unordered_set<GameObject *> objs;

    // CLEAR ALL LINKED STATUS
    for(GameMapCell & cell : mCells)
    {
        if(cell.owner == player)
        {
            cell.linked = false;

            if(cell.obj != nullptr)
                objs.insert(cell.obj);
        }
    }

    // reset all objects linked flag and remove visibility
    for(GameObject * obj : objs)
    {
        DelPlayerObjVisibility(obj, player);

        obj->SetLinked(false);
    }

    // FIND LINKED CELLS
    std::vector<unsigned int> todo;
    std::unordered_set<unsigned int> done;

    const Cell2D & home = player->GetBaseCell();
    const unsigned int indHome = home.row * mCols + home.col;
    todo.push_back(indHome);

    while(!todo.empty())
    {
        unsigned int currInd = todo.back();
        todo.pop_back();

        GameMapCell & currCell = mCells[currInd];
        currCell.linked = true;

        // notify current object
        if(currCell.obj != nullptr)
            currCell.obj->SetLinked(true);

        // add TOP
        unsigned int r = currCell.row - 1;

        if(r < mRows)
        {
            const unsigned int ind = currInd - mCols;

            if(mCells[ind].owner == player && done.find(ind) == done.end())
                todo.push_back(ind);
        }

        // add BOTTOM
        r = currCell.row + 1;

        if(r < mRows)
        {
            unsigned int ind = currInd + mCols;

            if(mCells[ind].owner == player && done.find(ind) == done.end())
                todo.push_back(ind);
        }

        // add LEFT
        unsigned int c = currCell.col - 1;

        if(c < mCols)
        {
            const unsigned int ind = currInd - 1;

            if(mCells[ind].owner == player && done.find(ind) == done.end())
                todo.push_back(ind);
        }

        // add RIGHT
        c = currCell.col + 1;

        if(c < mCols)
        {
            const unsigned int ind = currInd + 1;

            if(mCells[ind].owner == player && done.find(ind) == done.end())
                todo.push_back(ind);
        }

        // add current to done
        done.insert(currInd);
    }

    // UPDATE INFLUENCE
    for(unsigned int ind : done)
    {
        const int row = ind / mCols;
        const int col = ind % mCols;

        UpdateInfluencedCells(row, col);
    }

    // UPDATE ALL CELLS IMAGE
    const unsigned int totCells = mRows * mCols;

    for(unsigned int ind = 0; ind < totCells; ++ind)
    {
        const GameMapCell & cell = mCells[ind];

        if(cell.owner == player || cell.influencer != NO_FACTION)
            UpdateCellType(ind, cell);
    }

    // UPDATE OBJECTS VISIBILITY
    for(GameObject * obj : objs)
        AddPlayerObjVisibility(obj, player);

    ApplyVisibility(player);
}

void GameMap::UpdateInfluencedCells(int row, int col)
{
    const unsigned int ind0 = row * mCols + col;
    GameMapCell & gcell = mCells[ind0];

    // not linked cells have no influence
    if(!gcell.linked)
        return ;

    const PlayerFaction faction = gcell.owner->GetFaction();

    const unsigned int r0 = (row > 0) ? row - 1 : row;
    const unsigned int c0 = (col > 0) ? col - 1 : col;

    const unsigned int r1Inc = row + 2;
    const unsigned int r1 = (r1Inc < mRows) ? r1Inc : mRows;

    const unsigned int c1Inc = col + 2;
    const unsigned int c1 = (c1Inc < mCols) ? c1Inc : mCols;

    for(unsigned int r = r0; r < r1; ++r)
    {
        const unsigned int indBase = r * mCols;

        for(unsigned int c = c0; c < c1; ++c)
        {
            const unsigned int ind = indBase + c;
            GameMapCell & gc = mCells[ind];

            // not walkable empty cell
            if(!gc.walkable && nullptr == gc.obj)
                continue;

            // update map of influence
            gc.influencers[faction] = true;

            // count active influencers to see if there's only one
            int influencers = 0;

            for(auto it : gc.influencers)
            {
                if(it.second)
                    ++influencers;
            }

            if(influencers == 1)
                gc.influencer = faction;
            else
                gc.influencer = NO_FACTION;

            UpdateCellType(ind, gc);
        }
    }
}

bool GameMap::MoveObjToCell(GameObject * obj, int row, int col)
{
    // TODO support objects covering more than 1 cell
    const int ind0 = obj->GetRow0() * mCols + obj->GetCol0();

    // object is not in its cell !?
    if(mCells[ind0].obj != obj)
        return false;

    // remove object from current cell
    mCells[ind0].obj = nullptr;
    mCells[ind0].walkable = true;

    // add object to new cell
    const int ind1 = row * mCols + col;

    obj->SetCell(&mCells[ind1]);

    mCells[ind1].obj = obj;
    mCells[ind1].walkable = false;

    return true;
}

Cell2D GameMap::GetClosestCell(const Cell2D & start, const std::vector<Cell2D> targets) const
{
    // failed to find any walkable
    if(targets.empty())
        return Cell2D(-1, -1);

    // get closest cell
    int minInd = 0;
    int minDist = std::abs(start.row - targets[minInd].row) +
                  std::abs(start.col - targets[minInd].col);

    for(unsigned int i = 1; i < targets.size(); ++i)
    {
        const int dist = std::abs(start.row - targets[i].row) +
                         std::abs(start.col - targets[i].col);

        if(dist < minDist)
        {
            minDist = dist;
            minInd = i;
        }
    }

    return targets[minInd];
}

void GameMap::DestroyObject(GameObject * obj)
{
    // update visibility map
    // NOTE only local player for now
    Player * localPlayer = mGame->GetLocalPlayer();

    if(obj->GetOwner() == localPlayer)
        DelPlayerObjVisibility(obj, localPlayer);

    // generic cells update
    for(int r = obj->GetRow1(); r <= obj->GetRow0(); ++r)
    {
        const unsigned int indBase = r * mCols;

        for(int c = obj->GetCol1(); c <= obj->GetCol0(); ++c)
        {
            const unsigned int ind = indBase + c;

            GameMapCell & cell = mCells[ind];

            cell.walkable = true;
            cell.obj = nullptr;

            // update cell image
            UpdateCellType(ind, cell);
        }
    }

    // remove iso object from layer
    IsoObject * isoObj = obj->GetIsoObject();
    IsoLayer * layer = isoObj->GetLayer();
    layer->ClearObject(isoObj);

    // finally delete the object
    delete obj;
}

void  GameMap::AddVisibilityToCell(Player * player, int ind)
{
    player->AddVisibility(ind);
}

void GameMap::DelVisibilityToCell(Player * player, int ind)
{
    player->RemVisibility(ind);
}

void GameMap::AddPlayerObjVisibility(GameObject * obj, Player * player)
{
    using namespace std::placeholders;

    // player is bounded to the function object so I don't need to propagate it as param
    PropagatePlayerObjVisibility(obj,
                                 std::bind(&GameMap::AddVisibilityToCell, this, player, _1));
}

void GameMap::DelPlayerObjVisibility(GameObject * obj, Player * player)
{
    using namespace std::placeholders;

    // player is bounded to the function object so I don't need to propagate it as param
    PropagatePlayerObjVisibility(obj,
                                 std::bind(&GameMap::DelVisibilityToCell, this, player, _1));
}

void GameMap::PropagatePlayerObjVisibility(GameObject * obj, std::function<void(int)> visFun)
{
    const Cell2D cell1(obj->GetRow1(), obj->GetCol1());
    const Cell2D cell0(obj->GetRow0(), obj->GetCol0());

    const int objVisLvl = obj->GetVisibilityLevel();

    PropagatePlayerVisibility(cell1, cell0, objVisLvl, visFun);
}

void GameMap::AddPlayerCellVisibility(const GameMapCell & cell, Player * player)
{
    using namespace std::placeholders;

    // player is bounded to the function object so I don't need to propagate it as param
    PropagatePlayerCellVisibility(cell,
                                  std::bind(&GameMap::AddVisibilityToCell, this, player, _1));
}

void GameMap::DelPlayerCellVisibility(const GameMapCell & cell, Player * player)
{
    using namespace std::placeholders;

    // player is bounded to the function object so I don't need to propagate it as param
    PropagatePlayerCellVisibility(cell,
                                  std::bind(&GameMap::DelVisibilityToCell, this, player, _1));
}

void GameMap::PropagatePlayerCellVisibility(const GameMapCell & cell, std::function<void(int)> visFun)
{
    const Cell2D c(cell.row, cell.col);

    // TODO get visibility from player or base
    const int visLvl = 0;

    PropagatePlayerVisibility(c, c, visLvl, visFun);
}

void GameMap::PropagatePlayerVisibility(const Cell2D & cell1, const Cell2D & cell0,
                                        int visLevel, std::function<void(int)> visFun)
{
    const int objRows = 1 + cell0.row - cell1.row;
    const int objCols = 1 + cell0.col - cell1.col;

    const int visHalfLen0 = 2;
    const int visLenInc = 1;
    const int visExtSide = visHalfLen0 + visLenInc * visLevel;

    const int visLenMax = objRows + visExtSide * 2;

    const int visSideCols = (visLenMax - 1) / 2;

    // object columns
    int minCol = cell1.col;
    int maxCol = cell0.col;

    if(1 == objCols && visLevel % 2 == 1)
    {
        if(minCol > 0)
            minCol -= 1;

        if(maxCol < static_cast<int>(mCols - 1))
            maxCol += 1;
    }

    int minRow = cell1.row - visExtSide;
    int maxRow = cell0.row + visExtSide;

    for(int c = minCol; c <= maxCol; ++c)
    {
        for(int r = minRow; r <= maxRow; ++r)
        {
            if(r < 0 || r >= static_cast<int>(mRows))
                continue;

            const int ind = r * mCols + c;

            visFun(ind);
        }
    }

    // left columns
    if(minCol - 1 > -1)
    {
        int minColL = minCol - visSideCols;
        int maxColL = minCol - 1;

        if(minColL < 0)
            minColL = 0;

        int minRowL = minRow;
        int maxRowL = maxRow;

        for(int c = maxColL; c >= minColL; --c)
        {
            ++minRowL;
            --maxRowL;

            for(int r = minRowL; r <= maxRowL; ++r)
            {
                if(r < 0 || r >= static_cast<int>(mRows))
                    continue;

                const int ind = r * mCols + c;

                visFun(ind);
            }
        }
    }

    // right columns
    if(maxCol + 1 < static_cast<int>(mCols))
    {
        int minColR = maxCol + 1;
        int maxColR = maxCol + visSideCols;

        if(maxColR >= static_cast<int>(mCols))
            maxColR = mCols - 1;

        int minRowR = minRow;
        int maxRowR = maxRow;

        for(int c = minColR; c <= maxColR; ++c)
        {
            ++minRowR;
            --maxRowR;

            for(int r = minRowR; r <= maxRowR; ++r)
            {
                if(r < 0 || r >= static_cast<int>(mRows))
                    continue;

                const int ind = r * mCols + c;

                visFun(ind);
            }
        }
    }
}

void GameMap::UpdateObjectPaths(float delta)
{
    auto itPath = mPaths.begin();

    while(itPath != mPaths.end())
    {
        ObjectPath * path = *itPath;

        path->Update(delta);

        const ObjectPath::PathState state = path->GetState();

        if(state == ObjectPath::PathState::COMPLETED || state == ObjectPath::PathState::ABORTED)
        {
            delete path;
            itPath = mPaths.erase(itPath);
        }
        else if(state == ObjectPath::PathState::FAILED)
        {
            // TODO try to recover from failed path
            delete path;
            itPath = mPaths.erase(itPath);
        }
        else
            ++itPath;
    }
}

void GameMap::UpdateConquerPaths(float delta)
{
    auto itCP = mConquerPaths.begin();

    while(itCP != mConquerPaths.end())
    {
        ConquerPath * path = *itCP;

        path->Update(delta);

        const ConquerPath::ConquerState state = path->GetState();

        if(state == ConquerPath::ConquerState::COMPLETED || state == ConquerPath::ConquerState::ABORTED)
        {
            delete path;
            itCP = mConquerPaths.erase(itCP);
        }
        else if(state == ConquerPath::ConquerState::FAILED)
        {
            // TODO try to recover from failed path
            delete path;
            itCP = mConquerPaths.erase(itCP);
        }
        else
            ++itCP;
    }
}

void GameMap::UpdateWallBuildPaths(float delta)
{
    auto it = mWallBuildPaths.begin();

    while(it != mWallBuildPaths.end())
    {
        WallBuildPath * path = *it;

        path->Update(delta);

        const WallBuildPath::BuildState state = path->GetState();

        if(state == WallBuildPath::BuildState::COMPLETED || state == WallBuildPath::BuildState::ABORTED)
        {
            delete path;
            it = mWallBuildPaths.erase(it);
        }
        else if(state == WallBuildPath::BuildState::FAILED)
        {
            // TODO try to recover from failed path
            delete path;
            it = mWallBuildPaths.erase(it);
        }
        else
            ++it;
    }
}

void GameMap::UpdateWalls(const Cell2D & center)
{
    const int rows = static_cast<int>(mRows);
    const int cols = static_cast<int>(mCols);

    // cell out of map !?
    if(center.row < 0 || center.col < 0 || center.row >= rows || center.col >= cols)
        return ;

    UpdateWall(center);

    // update north
    if(center.row > 0)
        UpdateWall({center.row - 1, center.col});

    // update south
    if(center.row + 1 < rows)
    UpdateWall({center.row + 1, center.col});

    // update west
    if(center.col > 0)
        UpdateWall({center.row, center.col - 1});

    // update east
    if(center.col + 1 < cols)
        UpdateWall({center.row, center.col + 1});
}

void GameMap::UpdateWall(const Cell2D & cell)
{
    GameObject * obj = GetCell(cell.row, cell.col).obj;

    // no wall here
    if(nullptr == obj || obj->GetObjectType() != OBJ_WALL)
        return ;

    Wall * w = static_cast<Wall *>(obj);

    const GameObject * objN = (cell.row - 1 >= 0) ? GetCell(cell.row - 1, cell.col).obj : nullptr;
    const bool wallN = objN && (objN->GetObjectType() == OBJ_WALL || objN->GetObjectType() == OBJ_DEF_TOWER);

    const GameObject * objS = (cell.row + 1 < static_cast<int>(mRows)) ? GetCell(cell.row + 1, cell.col).obj : nullptr;
    const bool wallS = objS && (objS->GetObjectType() == OBJ_WALL || objS->GetObjectType() == OBJ_DEF_TOWER);

    const GameObject * objW = (cell.col - 1 >= 0) ? GetCell(cell.row, cell.col - 1).obj : nullptr;
    const bool wallW = objW && (objW->GetObjectType() == OBJ_WALL || objW->GetObjectType() == OBJ_DEF_TOWER);

    const GameObject * objE = (cell.col + 1 < static_cast<int>(mCols)) ? GetCell(cell.row, cell.col + 1).obj : nullptr;
    const bool wallE = objE && (objE->GetObjectType() == OBJ_WALL || objE->GetObjectType() == OBJ_DEF_TOWER);

    enum Flags
    {
        NORTH   = 0x1,
        SOUTH   = 0x2,
        WEST    = 0x4,
        EAST    = 0x8
    };

    enum Configurations
    {
        HORIZ_1 = WEST,
        HORIZ_2 = EAST,
        HORIZ_3 = WEST + EAST,

        VERT_1 = NORTH,
        VERT_2 = SOUTH,
        VERT_3 = NORTH + SOUTH,

        TL = EAST + SOUTH,
        TR = WEST + SOUTH,
        BL = EAST+ NORTH,
        BR = WEST + NORTH,

        INTN = NORTH + WEST + EAST,
        INTS = SOUTH + WEST + EAST,
        INTW = NORTH + SOUTH + WEST,
        INTE = NORTH + SOUTH + EAST,

        CROSS = NORTH + SOUTH + WEST + EAST
    };

    const int conf = NORTH * static_cast<int>(wallN) +
                     SOUTH * static_cast<int>(wallS) +
                     WEST * static_cast<int>(wallW) +
                     EAST * static_cast<int>(wallE);

    switch(conf)
    {
        case HORIZ_1:
        case HORIZ_2:
        case HORIZ_3:
            w->SetWallType(OBJ_WALL_HORIZ);
        break;

        case VERT_1:
        case VERT_2:
        case VERT_3:
            w->SetWallType(OBJ_WALL_VERT);
        break;

        case TL:
            w->SetWallType(OBJ_WALL_TL);
        break;
        case TR:
            w->SetWallType(OBJ_WALL_TR);
        break;
        case BL:
            w->SetWallType(OBJ_WALL_BL);
        break;
        case BR:
            w->SetWallType(OBJ_WALL_BR);
        break;

        case INTN:
            w->SetWallType(OBJ_WALL_INTN);
        break;
        case INTS:
            w->SetWallType(OBJ_WALL_INTS);
        break;
        case INTW:
            w->SetWallType(OBJ_WALL_INTW);
        break;
        case INTE:
            w->SetWallType(OBJ_WALL_INTE);
        break;

        case CROSS:
            w->SetWallType(OBJ_WALL_CROSS);
        break;

        default:
            // do nothing for any other case
        break;
    }
}

} // namespace game
