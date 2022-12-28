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
#include "GameObjects/ObjectData.h"
#include "GameObjects/PracticeTarget.h"
#include "GameObjects/RadarStation.h"
#include "GameObjects/ResourceGenerator.h"
#include "GameObjects/ResourceStorage.h"
#include "GameObjects/SceneObject.h"
#include "GameObjects/Unit.h"
#include "GameObjects/Wall.h"
#include "GameObjects/WallGate.h"
#include "Screens/ScreenGame.h"
#include "Widgets/MiniMap.h"

#include <sgl/utilities/UniformDistribution.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

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

bool GameMap::IsCellVisibleToLocalPlayer(unsigned int ind) const
{
    Player * p = mGame->GetLocalPlayer();
    return p->IsCellVisible(ind);
}

bool GameMap::IsCellVisibleToLocalPlayer(unsigned int r, unsigned int c) const
{
    const unsigned int ind = (r * mCols) + c;

    Player * p = mGame->GetLocalPlayer();
    return p->IsCellVisible(ind);
}

bool GameMap::IsAnyCellVisibleToLocalPlayer(unsigned int rTL, unsigned int cTL,
                                            unsigned int rBR, unsigned int cBR) const
{
    Player * p = mGame->GetLocalPlayer();

    for(unsigned int r = rTL; r <= rBR; ++r)
    {
        const unsigned int indBase = (r * mCols);

        for(unsigned int c = cTL; c <= cBR; ++c)
        {
            const unsigned int ind = indBase + c;

            if(p->IsCellVisible(ind))
                return true;
        }
    }

    return false;
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
        return mCells[cellInd].objTop && mCells[cellInd].objTop->IsVisited();
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

    // init players visibility map
    for(int i = 0; i < mGame->GetNumPlayers(); ++i)
        mGame->GetPlayerByIndex(i)->InitVisibility(mRows, mCols);
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
    if(player != mGame->GetLocalPlayer())
        return ;

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
        Player * objPlayer = go->GetOwner();
        MiniMap * mm = mScreenGame->GetMiniMap();

        // hide objects if not visited or not a structure
        if(!visible && (!go->IsVisited() || !go->IsStructure()))
        {
            layer->SetObjectVisible(obj, false);

            if(objPlayer != nullptr && mm != nullptr)
                mm->RemoveElement(go->GetRow0(), go->GetCol0());
        }
        else
        {
            layer->SetObjectVisible(obj, true);

            if(mm != nullptr)
            {
                if(objPlayer != nullptr)
                {
                    const PlayerFaction faction = objPlayer->GetFaction();
                    const MiniMap::MiniMapElemType type = static_cast<MiniMap::MiniMapElemType>(MiniMap::MME_FACTION1 + faction);
                    mm->AddElement(go->GetRow0(), go->GetCol0(), go->GetRows(), go->GetCols(), type, faction);
                }
                else
                {
                    const MiniMap::MiniMapElemType type = go->CanBeConquered() ? MiniMap::MME_CONQUERABLE : MiniMap::MME_SCENE;
                    mm->AddElement(go->GetRow0(), go->GetCol0(), go->GetRows(), go->GetCols(),
                                   type, NO_FACTION);
                }
            }
        }

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
    else if(MapObjectId::STORAGE_ENERGY == objId)
        CreateObject(layerId, OBJ_RES_STORAGE_ENERGY, nullptr, r0, c0, rows, cols);
    else if(MapObjectId::STORAGE_MATERIAL == objId)
        CreateObject(layerId, OBJ_RES_STORAGE_MATERIAL, nullptr, r0, c0, rows, cols);
    else if(MapObjectId::STORAGE_DIAMONDS == objId)
        CreateObject(layerId, OBJ_RES_STORAGE_DIAMONDS, nullptr, r0, c0, rows, cols);
    else if(MapObjectId::STORAGE_BLOBS == objId)
        CreateObject(layerId, OBJ_RES_STORAGE_BLOBS, nullptr, r0, c0, rows, cols);
    else if(MapObjectId::RADAR_STATION == objId)
        CreateObject(layerId, OBJ_RADAR_STATION, nullptr, r0, c0, rows, cols);
    else if(objId >= MapObjectId::MOUNTAINS_FIRST && objId <= MapObjectId::MOUNTAINS_LAST)
    {
        const int objInd = static_cast<int>(objId) - static_cast<int>(MapObjectId::MOUNTAINS_FIRST);

        CreateObject(layerId, OBJ_MOUNTAINS_FIRST + objInd, nullptr, r0, c0, rows, cols);
    }
    else if(objId >= MapObjectId::MOUNTAIN_FIRST && objId <= MapObjectId::MOUNTAIN_LAST)
    {
        const int objInd = static_cast<int>(objId) - static_cast<int>(MapObjectId::MOUNTAIN_FIRST);

        CreateObject(layerId, OBJ_ROCKS_FIRST + objInd, nullptr, r0, c0, rows, cols);
    }
    else if(MapObjectId::PRACTICE_TARGET == objId)
        CreateObject(layerId, OBJ_PRACTICE_TARGET, nullptr, r0, c0, rows, cols);
    else if(objId >= MapObjectId::UNIT_FIRST && objId <= MapObjectId::UNIT_LAST)
    {
        Player * p = nullptr;

        unsigned int td = 0;

        if(objId < MapObjectId::UNIT_1_P2)
        {
            p = mGame->GetPlayerByIndex(0);
            td = static_cast<int>(objId) - static_cast<int>(MapObjectId::UNIT_1_P1);
        }
        else if(objId < MapObjectId::UNIT_1_P3)
        {
            p = mGame->GetPlayerByIndex(1);
            td = static_cast<int>(objId) - static_cast<int>(MapObjectId::UNIT_1_P2);
        }
        else
        {
            p = mGame->GetPlayerByIndex(2);
            td = static_cast<int>(objId) - static_cast<int>(MapObjectId::UNIT_1_P3);
        }

        const UnitType type = static_cast<UnitType>(UnitType::UNIT_1 + td);
        const ObjectData & data = p->GetAvailableUnit(type);
        const Cell2D dest(r0, c0);
        CreateUnit(data, nullptr, dest, p);
    }
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
    if(gcell.objTop)
        return nullptr;

    // create game object
    GameObject * obj = nullptr;

    if(OBJ_RES_GEN_ENERGY == objId)
        obj = new ResourceGenerator(RESG_ENERGY, ResourceType::RES_ENERGY, rows, cols);
    else if(OBJ_RES_GEN_MATERIAL1 == objId)
        obj = new ResourceGenerator(RESG_MATERIAL, ResourceType::RES_MATERIAL1, rows, cols);
    else if(OBJ_RES_GEN_ENERGY_SOLAR == objId)
        obj = new ResourceGenerator(RESG_ENERGY_SOLAR, ResourceType::RES_ENERGY, rows, cols);
    else if(OBJ_RES_GEN_MATERIAL_EXTRACTOR == objId)
        obj = new ResourceGenerator(RESG_MATERIAL_EXTRACTOR, ResourceType::RES_MATERIAL1, rows, cols);
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
    else if(objId >= OBJ_MOUNTAINS_FIRST && objId <= OBJ_MOUNTAINS_LAST)
        obj = new SceneObject(static_cast<GameObjectType>(objId), rows, cols);
    else if(objId >= OBJ_ROCKS_FIRST && objId <= OBJ_ROCKS_LAST)
        obj = new SceneObject(static_cast<GameObjectType>(objId), rows, cols);
    else if(objId >= OBJ_WALL_FIRST && objId <= OBJ_WALL_LAST)
        obj = new Wall(static_cast<GameObjectType>(objId), rows, cols);
    else if(objId >= OBJ_WALL_GATE_FIRST && objId <= OBJ_WALL_GATE_LAST)
        obj = new WallGate(static_cast<GameObjectType>(objId), rows, cols);
    else if(OBJ_DEF_TOWER == objId)
    {
        const ObjectData & data = owner->GetAvailableStructure(STRUCT_DEF_TOWER);
        obj = new DefensiveTower(data);
    }
    else if(OBJ_DIAMONDS == objId)
        obj = new Diamonds;
    else if(OBJ_BLOBS == objId)
        obj  = new Blobs;
    else if(OBJ_RES_STORAGE_ENERGY == objId)
        obj = new ResourceStorage(RES_ENERGY, 1, 1);
    else if(OBJ_RES_STORAGE_MATERIAL == objId)
        obj = new ResourceStorage(RES_MATERIAL1, 1, 1);
    else if(OBJ_RES_STORAGE_DIAMONDS == objId)
        obj = new ResourceStorage(RES_DIAMONDS, 1, 1);
    else if(OBJ_RES_STORAGE_BLOBS == objId)
        obj = new ResourceStorage(RES_BLOBS, 1, 1);
    // this should never happen
    else
    {
        std::cerr << "[ERR] GameMap::CreateObject - unknown obj id: " << objId << std::endl;
        return nullptr;
    }

    // assign owner
    obj->SetOwner(owner);

    if(owner != nullptr)
    {
        if(obj->GetObjectType() == OBJ_RES_GEN)
            owner->AddResourceGenerator(static_cast<ResourceGenerator *>(obj));
        else if(obj->IsStructure())
            owner->AddStructure(static_cast<Structure *>(obj));
    }


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
            cell.objTop = obj;
            cell.owner = owner;

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

bool GameMap::AreObjectsOrthoAdjacent(const GameObject * obj1, const GameObject * obj2) const
{
    const Cell2D TL1(obj1->GetRow1(), obj1->GetCol1());
    const Cell2D BR1(obj1->GetRow0(), obj1->GetCol0());
    const Cell2D TL2(obj2->GetRow1(), obj2->GetCol1());
    const Cell2D BR2(obj2->GetRow0(), obj2->GetCol0());

    const int maxD = 1;

    // obj1 above obj2
    if(BR1.row < TL2.row)
    {
        // obj1 left of obj2 OR obj1 right of obj2
        if(BR1.col < TL2.col || TL1.col > BR2.col)
            return false;
        // same cols
        else
            return (TL2.row - BR1.row) == maxD;
    }
    // obj1 below obj2
    else if(TL1.row > BR1.row)
    {
        // obj1 left of obj2 OR obj1 right of obj2
        if(BR1.col < TL2.col || TL1.col > BR2.col)
            return false;
        // same cols
        else
            return (BR2.row - TL1.row) == maxD;
    }
    // same rows
    else
    {
        // obj1 left of obj2
        if(BR1.col < TL2.col)
            return (TL2.col - BR1.col) == maxD;
        // obj1 right of obj2
        else if(TL1.col > BR2.col)
            return (TL1.col - BR2.col) == maxD;
        // same cols, overlapping objects? -> this should never happen
        else
            return false;
    }
}

bool GameMap::AreCellsOrthoAdjacent(const Cell2D & cell1, const Cell2D & cell2) const
{
    const int distR = std::abs(cell1.row - cell2.row);
    const int distC = std::abs(cell1.col - cell2.col);

    const int maxDist = 1;

    return (distR == 0 && distC == maxDist) || (distR == maxDist && distC == 0);
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

    // check if unit has enough energy - LAST CHECK
    return player->HasEnough(Player::Stat::MATERIAL, COST_CONQUEST_CELL);
}

void GameMap::StartConquerCell(const Cell2D & cell, Player * player)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    // take player's energy
    player->SumResource(Player::Stat::MATERIAL, -COST_CONQUEST_CELL);

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
    Player * prevOwner = gcell.owner;
    gcell.owner = player;

    // update player
    player->SumCells(1);

    // reset cell's changing flag
    gcell.changing = false;

    // propagate effects of conquest
    UpdateInfluencedCells(cell.row, cell.col);

    UpdateLinkedCells(player);

    // update visibility map
    AddPlayerCellVisibility(gcell, player);

    if(stolen)
        DelPlayerCellVisibility(gcell, prevOwner);

    ApplyLocalVisibility();
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
        if(path->GetUnit() == obj)
        {
            path->Abort();
            return true;
        }
    }

    return false;
}

bool GameMap::CanBuildStructure(Unit * unit, const Cell2D & cell, Player * player, const ObjectData & data)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);

    // out of bounds
    if((data.rows - 1) > r || (data.cols - 1) > c || r >= mRows || c >= mCols)
        return false;

    // check costs
    const bool costOk = player->HasEnough(Player::ENERGY, data.costs[RES_ENERGY]) &&
                        player->HasEnough(Player::MATERIAL, data.costs[RES_MATERIAL1]) &&
                        player->HasEnough(Player::DIAMONDS, data.costs[RES_DIAMONDS]) &&
                        player->HasEnough(Player::BLOBS, data.costs[RES_BLOBS]);

    if(!costOk)
        return false;

    // check unit's energy
    if(!unit->HasEnergyForAction(BUILD_STRUCTURE))
        return false;

    // check cells
    const unsigned int r0 = 1 + cell.row - data.rows;
    const unsigned int c0 = 1 + cell.col - data.cols;

    for(int r = r0; r <= cell.row; ++r)
    {
        const int idx0 = r * mCols;

        for(int c = c0; c <= cell.col; ++c)
        {
            const int idx = idx0 + c;
            const GameMapCell & gcell = mCells[idx];

            // already changing or occupied
            if(gcell.changing || !gcell.walkable)
                return false;
        }
    }

    return true;
}

void GameMap::StartBuildStructure(const Cell2D & cell, Player * player, const ObjectData & data)
{
    // mark cell as changing
    const unsigned int r0 = 1 + cell.row - data.rows;
    const unsigned int c0 = 1 + cell.col - data.cols;

    for(int r = r0; r <= cell.row; ++r)
    {
        const int idx0 = r * mCols;

        for(int c = c0; c <= cell.col; ++c)
        {
            const int idx = idx0 + c;
            GameMapCell & gcell = mCells[idx];
            gcell.changing = true;
        }
    }

    // make player pay
    if(data.costs[RES_ENERGY] > 0)
        player->SumResource(Player::ENERGY, -data.costs[RES_ENERGY]);
    if(data.costs[RES_MATERIAL1] > 0)
        player->SumResource(Player::MATERIAL, -data.costs[RES_MATERIAL1]);
    if(data.costs[RES_DIAMONDS] > 0)
        player->SumResource(Player::DIAMONDS, -data.costs[RES_DIAMONDS]);
    if(data.costs[RES_BLOBS] > 0)
        player->SumResource(Player::BLOBS, -data.costs[RES_BLOBS]);
}

void GameMap::BuildStructure(const Cell2D & cell, Player * player, const ObjectData & data)
{
    // check if cell was of another faction
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];
    bool stolen = gcell.owner != nullptr && gcell.owner != player;

    // make cell empty
    ClearCell(gcell);

    // assign owner
    Player * prevOwner = gcell.owner;
    gcell.owner = player;

    // update player
    player->SumCells(1);

    // reset cell's changing flag
    gcell.changing = false;

    // add object wall
    const GameObjectType got = Structure::StructureToGameObject(static_cast<StructureType>(data.objType));

    GameObject * obj = CreateObject(OBJECTS2, got, player, cell.row, cell.col, data.rows, data.cols);

    // propagate effects of conquest
    UpdateInfluencedCells(cell.row, cell.col);

    UpdateLinkedCells(player);

    // update surrounding walls if building an object that can connect
    if(obj->GetObjectType() == OBJ_DEF_TOWER || obj->GetObjectType() == OBJ_WALL_GATE)
        UpdateWalls(cell);

    // update visibility map
    AddPlayerCellVisibility(gcell, player);

    if(stolen)
        DelPlayerCellVisibility(gcell, prevOwner);

    ApplyLocalVisibility();
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

    return player->HasEnough(Player::Stat::MATERIAL, costMat)  &&
           player->HasEnough(Player::Stat::ENERGY, costEne);
}

void GameMap::StartBuildWall(const Cell2D & cell, Player * player, unsigned int level)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    // take player's material
    const int costEne = Wall::GetCostEnergy(level);
    player->SumResource(Player::Stat::ENERGY, -costEne);

    const int costMat = Wall::GetCostMaterial(level);
    player->SumResource(Player::Stat::MATERIAL, -costMat);

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
    Player * prevOwner = gcell.owner;
    gcell.owner = player;

    // update player
    player->SumCells(1);

    // reset cell's changing flag
    gcell.changing = false;
    // propagate effects of conquest
    UpdateInfluencedCells(cell.row, cell.col);

    UpdateLinkedCells(player);

    // add object wall
    const int rows = 1;
    const int cols = 1;
    CreateObject(OBJECTS2, planned, player, cell.row, cell.col, rows, cols);

    // update minimap
    if(IsCellVisibleToLocalPlayer(cell.row, cell.col))
    {
        const PlayerFaction faction = player->GetFaction();
        const MiniMap::MiniMapElemType type = static_cast<MiniMap::MiniMapElemType>(MiniMap::MME_FACTION1 + faction);
        MiniMap * mm = mScreenGame->GetMiniMap();
        mm->AddElement(cell.row, cell.col, rows, cols, type, faction);
    }

    // update this wall type and the ones surrounding it
    UpdateWalls(cell);

    // update visibility map
    AddPlayerCellVisibility(gcell, player);

    if(stolen)
        DelPlayerCellVisibility(gcell, prevOwner);

    ApplyLocalVisibility();
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
        if(path->GetUnit() == obj)
        {
            path->Abort();
            return true;
        }
    }

    return false;
}

bool GameMap::CanConquerStructure(Unit * unit, const Cell2D & end, Player * player)
{
    const unsigned int r1 = static_cast<unsigned int>(end.row);
    const unsigned int c1 = static_cast<unsigned int>(end.col);

    // no unit
    if(nullptr == unit)
        return false;

    // not player's unit
    if(unit->GetOwner() != player)
        return false;

    // unit doesn't have enough energy
    if(!unit->HasEnergyForAction(CONQUER_STRUCTURE))
        return false;

    const int ind1 = r1 * mCols + c1;
    GameMapCell & gcell1 = mCells[ind1];

    // end is empty
    if(nullptr == gcell1.objTop)
        return false;

    // target object can't be conquered
    if(!gcell1.objTop->CanBeConquered())
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
    player->SumResource(Player::Stat::ENERGY, -COST_CONQUEST_RES_GEN);

    // mark start as changing
    const int ind0 = start.row * mCols + start.col;
    mCells[ind0].changing = true;

    // mark object cells as changing
    const int ind1 = end.row * mCols + end.col;
    GameObject * obj = mCells[ind1].objTop;

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

    GameObject * obj = gcell1.objTop;

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
    Player * prevOwner = obj->GetOwner();
    obj->SetOwner(player);

    // update player
    player->SumCells(1);

    if(gcell1.objTop->GetObjectType() == OBJ_RES_GEN)
        player->AddResourceGenerator(gcell1.GetResourceGenerator());

    // reset start changing flag
    const int ind0 = start.row * mCols + start.col;
    mCells[ind0].changing = false;

    // update map
    UpdateLinkedCells(player);

    // update visibility
    if(prevOwner && prevOwner != player)
        DelPlayerObjVisibility(obj, prevOwner);

    AddPlayerObjVisibility(obj, player);

    ApplyLocalVisibility();
}

bool GameMap::CanCreateUnit(const ObjectData & data, GameObject * gen, Player * player)
{
    // generator is not owned by the player
    if(gen->GetOwner() != player)
        return false;

    // already has enough units
    if(player->GetNumUnits() == player->GetMaxUnits())
        return false;

    // only base can generate units (for now)
    if(gen->GetObjectType() != OBJ_BASE)
        return false;

    // generator is already busy
    if(gen->IsBusy())
       return false;

    // check if player has enough resources
    if(!player->HasEnough(Player::Stat::ENERGY, data.costs[RES_ENERGY]) ||
       !player->HasEnough(Player::Stat::MATERIAL, data.costs[RES_MATERIAL1]) ||
       !player->HasEnough(Player::Stat::DIAMONDS, data.costs[RES_DIAMONDS]) ||
       !player->HasEnough(Player::Stat::BLOBS, data.costs[RES_BLOBS]))
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

void GameMap::StartCreateUnit(const ObjectData & data, GameObject * gen, const Cell2D & dest, Player * player)
{
    const int ind = dest.row * mCols + dest.col;
    GameMapCell & gcell = mCells[ind];

    // make player pay
    player->SumResource(Player::Stat::ENERGY, -data.costs[RES_ENERGY]);
    player->SumResource(Player::Stat::MATERIAL, -data.costs[RES_MATERIAL1]);
    player->SumResource(Player::Stat::DIAMONDS, -data.costs[RES_DIAMONDS]);
    player->SumResource(Player::Stat::BLOBS, -data.costs[RES_BLOBS]);

    // mark cell as changing
    gcell.changing = true;

    // mark generator as busy
    gen->SetBusy(true);
}

void GameMap::CreateUnit(const ObjectData & data, GameObject * gen, const Cell2D & dest, Player * player)
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
    gcell.objTop = unit;
    gcell.walkable = false;
    gcell.changing = false;

    mIsoMap->GetLayer(OBJECTS2)->AddObject(unit->GetIsoObject(), r, c);

    // store unit in map list and in registry
    mObjects.push_back(unit);
    mObjectsSet.insert(unit);

    // update generator, if any
    if(gen)
        gen->SetBusy(false);

    // update player
    player->AddUnit(unit);
    player->SumTotalUnitsLevel(unit->GetUnitLevel() + 1);

    // update visibility map
    AddPlayerObjVisibility(unit, player);

    ApplyLocalVisibility();
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

    // check if player has enough energy - LAST CHECK
    const int cost = COST_UNIT_UPGRADE[unitLevel];
    return player->HasEnough(Player::Stat::ENERGY, cost);
}

void GameMap::StartUpgradeUnit(GameObject * obj, Player * player)
{
    const int ind = obj->GetRow0() * mCols + obj->GetCol0();
    GameMapCell & gcell = mCells[ind];

    // make player pay
    const Unit * unit = static_cast<Unit *>(obj);
    const int unitLevel = unit->GetUnitLevel();
    const int cost = -COST_UNIT_UPGRADE[unitLevel];
    player->SumResource(Player::Stat::ENERGY, cost);

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
    if(mCells[ind].objTop != obj)
        return false;

    // start path
    path->Start();
    const bool started = path->GetState() == ObjectPath::RUNNING;

    if(started)
        mPaths.emplace_back(path);

    return started;
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

Cell2D GameMap::GetAdjacentMoveTarget(const Cell2D & start, const Cell2D & target) const
{
    return GetAdjacentMoveTarget(start, target, target);
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
    const int maxWalkables = (tCols + 2 + tRows) * 2;
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

Cell2D GameMap::GetOrthoAdjacentMoveTarget(const Cell2D & start, const GameObject * target) const
{
    const Cell2D tl(target->GetRow1(), target->GetCol1());
    const Cell2D br(target->GetRow0(), target->GetCol0());

    return GetOrthoAdjacentMoveTarget(start, tl, br);
}

Cell2D GameMap::GetOrthoAdjacentMoveTarget(const Cell2D & start, const Cell2D & target) const
{
    return GetOrthoAdjacentMoveTarget(start, target, target);
}

Cell2D GameMap::GetOrthoAdjacentMoveTarget(const Cell2D & start, const Cell2D & targetTL, const Cell2D & targetBR) const
{
    const int tRows = targetBR.row - targetTL.row + 1;
    const int tCols = targetBR.col - targetTL.col + 1;

    std::vector<Cell2D> walkalbes;
    const int maxWalkables = (tRows + tCols) * 2;
    walkalbes.reserve(maxWalkables);

    // TOP cells
    if(targetTL.row > 0)
    {
        const int r = targetTL.row - 1;
        const int indBase = r * mCols;

        for(int c = targetTL.col; c <= targetBR.col; ++c)
        {
            const int ind = indBase + c;

            if(mCells[ind].walkable)
                walkalbes.emplace_back(r, c);
        }
    }

    // BOTTOM cells
    if(targetBR.row < (mRows - 1))
    {
        const int r = targetBR.row + 1;
        const int indBase = r * mCols;

        for(int c = targetTL.col; c <= targetBR.col; ++c)
        {
            const int ind = indBase + c;

            if(mCells[ind].walkable)
                walkalbes.emplace_back(r, c);
        }
    }

    // LEFT cells
    if(targetTL.col > 0)
    {
        const int c = targetTL.col - 1;

        for(int r = targetTL.row; r <= targetBR.row; ++r)
        {
            const int ind = (r * mCols) + c;

            if(mCells[ind].walkable)
                walkalbes.emplace_back(r, c);
        }
    }

    // RIGHT cells
    if(targetBR.col < (mCols - 1))
    {
        const int c = targetBR.col + 1;

        for(int r = targetTL.row; r <= targetBR.row; ++r)
        {
            const int ind = (r * mCols) + c;

            if(mCells[ind].walkable)
                walkalbes.emplace_back(r, c);
        }
    }

    return GetClosestCell(start, walkalbes);
}

bool GameMap::MoveObjectDown(GameObject * obj)
{
    const unsigned int ind = (obj->GetRow0() * mCols) + obj->GetCol0();
    GameMapCell & gc = mCells[ind];

    if(obj != gc.objTop)
        return false;

    if(gc.objBottom)
        return false;

    // move object
    gc.objBottom = gc.objTop;
    gc.objTop = nullptr;

    return true;
}

bool GameMap::MoveObjectUp(GameObject * obj)
{
    const unsigned int ind = (obj->GetRow0() * mCols) + obj->GetCol0();
    GameMapCell & gc = mCells[ind];

    if(obj != gc.objBottom)
        return false;

    if(gc.objTop)
        return false;

    // move object
    gc.objTop = gc.objBottom;
    gc.objBottom = nullptr;

    return true;
}

int GameMap::ApproxDistance(const Cell2D & c1, const Cell2D & c2) const
{
    return std::abs(c1.row - c2.row) + std::abs(c1.col - c2.col);
}

int GameMap::ApproxDistance(GameObject * obj1, GameObject * obj2) const
{
    return std::abs(obj1->GetRow0() - obj2->GetRow0()) +
           std::abs(obj1->GetCol0() - obj2->GetCol0());
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
    if(!mGame->GetLocalPlayer()->IsCellVisible(ind))
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
    std::unordered_map<GameObject *, bool> objsLink;

    // CLEAR ALL LINKED STATUS
    for(GameMapCell & cell : mCells)
    {
        if(cell.owner == player)
        {
            cell.linked = false;

            if(cell.objTop != nullptr)
                objs.insert(cell.objTop);
        }
    }

    // reset all objects linked flag and remove visibility
    for(GameObject * obj : objs)
    {
        DelPlayerObjVisibility(obj, player);

        objsLink.insert({obj, false});
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

        // mark object as linked
        if(currCell.objTop != nullptr)
            objsLink[currCell.objTop] = true;

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

    // UPDATE LINK STATUS
    for(GameObject * obj : objs)
        obj->SetLinked(objsLink[obj]);

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

   ApplyLocalVisibility();
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
            if(!gc.walkable && nullptr == gc.objTop)
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
    if(mCells[ind0].objTop != obj)
        return false;

    // update minimap
    MiniMap * mm = mScreenGame->GetMiniMap();
    mm->MoveElement(obj->GetRow0(), obj->GetCol0(), row, col);

    // move object in iso map
    IsoLayer * layer = obj->GetIsoObject()->GetLayer();
    layer->MoveObject(obj->GetRow0(), obj->GetCol0(), row, col, false);

    // remove object from current cell
    mCells[ind0].objTop = nullptr;
    mCells[ind0].walkable = true;

    // add object to new cell
    const int ind1 = row * mCols + col;

    obj->SetCell(&mCells[ind1]);

    mCells[ind1].objTop = obj;
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
    Player * localPlayer = mGame->GetLocalPlayer();
    Player * owner = obj->GetOwner();

    // update visibility map
    // NOTE only local player for now
    if(obj->GetOwner() == localPlayer)
        DelPlayerObjVisibility(obj, localPlayer);

    // remove unit from player
    if(obj->GetObjectType() == OBJ_UNIT)
        owner->RemoveUnit(static_cast<Unit *>(obj));
    // remove structure
    else if(obj->IsStructure())
        owner->RemoveStructure(static_cast<Structure *>(obj));

    // generic cells update
    for(int r = obj->GetRow1(); r <= obj->GetRow0(); ++r)
    {
        const unsigned int indBase = r * mCols;

        for(int c = obj->GetCol1(); c <= obj->GetCol0(); ++c)
        {
            const unsigned int ind = indBase + c;

            GameMapCell & cell = mCells[ind];

            cell.walkable = true;
            cell.objTop = nullptr;

            // update cell image
            UpdateCellType(ind, cell);
        }
    }

    // remove iso object from layer
    IsoObject * isoObj = obj->GetIsoObject();
    IsoLayer * layer = isoObj->GetLayer();
    layer->ClearObject(isoObj);

    // remove object from mini map
    mScreenGame->GetMiniMap()->RemoveElement(obj->GetRow0(), obj->GetCol0());

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
    GameObject * obj = GetCell(cell.row, cell.col).objTop;

    // no wall or gate here
    if(nullptr == obj || (obj->GetObjectType() != OBJ_WALL && obj->GetObjectType() != OBJ_WALL_GATE))
        return ;

    const GameObject * objN = (cell.row - 1 >= 0) ? GetCell(cell.row - 1, cell.col).objTop : nullptr;
    const bool wallN = objN && (objN->GetObjectType() == OBJ_WALL ||
                                objN->GetObjectType() == OBJ_DEF_TOWER ||
                                objN->GetObjectType() == OBJ_WALL_GATE);

    const GameObject * objS = (cell.row + 1 < static_cast<int>(mRows)) ? GetCell(cell.row + 1, cell.col).objTop : nullptr;
    const bool wallS = objS && (objS->GetObjectType() == OBJ_WALL ||
                                objS->GetObjectType() == OBJ_DEF_TOWER ||
                                objS->GetObjectType() == OBJ_WALL_GATE);

    const GameObject * objW = (cell.col - 1 >= 0) ? GetCell(cell.row, cell.col - 1).objTop : nullptr;
    const bool wallW = objW && (objW->GetObjectType() == OBJ_WALL ||
                                objW->GetObjectType() == OBJ_DEF_TOWER ||
                                objW->GetObjectType() == OBJ_WALL_GATE);

    const GameObject * objE = (cell.col + 1 < static_cast<int>(mCols)) ? GetCell(cell.row, cell.col + 1).objTop : nullptr;
    const bool wallE = objE && (objE->GetObjectType() == OBJ_WALL ||
                                objE->GetObjectType() == OBJ_DEF_TOWER ||
                                objE->GetObjectType() == OBJ_WALL_GATE);

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

    if(obj->GetObjectType() == OBJ_WALL)
    {
        auto wall = static_cast<Wall *>(obj);

        switch(conf)
        {
            case HORIZ_1:
            case HORIZ_2:
            case HORIZ_3:
                wall->SetWallType(OBJ_WALL_HORIZ);
            break;

            case VERT_1:
            case VERT_2:
            case VERT_3:
                wall->SetWallType(OBJ_WALL_VERT);
            break;

            case TL:
                wall->SetWallType(OBJ_WALL_TL);
            break;
            case TR:
                wall->SetWallType(OBJ_WALL_TR);
            break;
            case BL:
                wall->SetWallType(OBJ_WALL_BL);
            break;
            case BR:
                wall->SetWallType(OBJ_WALL_BR);
            break;

            case INTN:
                wall->SetWallType(OBJ_WALL_INTN);
            break;
            case INTS:
                wall->SetWallType(OBJ_WALL_INTS);
            break;
            case INTW:
                wall->SetWallType(OBJ_WALL_INTW);
            break;
            case INTE:
                wall->SetWallType(OBJ_WALL_INTE);
            break;

            case CROSS:
                wall->SetWallType(OBJ_WALL_CROSS);
            break;

            default:
                // do nothing for any other case
            break;
        }
    }
    else if(obj->GetObjectType() == OBJ_WALL_GATE)
    {
        auto gate = static_cast<WallGate *>(obj);

        switch(conf)
        {
            case HORIZ_1:
            case HORIZ_2:
            case HORIZ_3:
                gate->SetGateType(OBJ_WALL_GATE_HORIZ);
            break;

            case VERT_1:
            case VERT_2:
            case VERT_3:
                gate->SetGateType(OBJ_WALL_GATE_VERT);
            break;

            default:
                // do nothing for any other case
            break;
        }
    }
}

} // namespace game
