#include "GameMap.h"

#include "Cell2D.h"
#include "Game.h"
#include "GameConstants.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "Player.h"
#include "GameObjects/Base.h"
#include "GameObjects/ResourceGenerator.h"
#include "GameObjects/SceneObject.h"
#include "GameObjects/Unit.h"
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
    mGame->GetPlayer(0)->InitVisibility(mRows, mCols);
}

void GameMap::SyncMapCells()
{
    const unsigned int size = mRows * mCols;

    for(unsigned int i = 0; i < size; ++i)
    {
        const auto type = static_cast<CellTypes>(mIsoMap->GetCellType(i));

        mCells[i].basicType = type;
        mCells[i].walkable = type == EMPTY;
    }
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
    IsoLayer * layer = mIsoMap->GetLayer(OBJECTS);

    for(GameObject * go : mObjects)
    {
        const int rTL = go->GetRow1();
        const int cTL = go->GetCol1();
        const int rBR = go->GetRow0();
        const int cBR = go->GetCol0();

        IsoObject * obj = go->GetIsoObject();

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

        // show or hide object
        if(visible != go->IsVisible())
        {
            // hide objects if not visited or not a structure
            if(!visible && (!go->IsVisited() || !go->IsStructure()))
                layer->SetObjectVisible(obj, false);
            else
                layer->SetObjectVisible(obj, true);

            go->SetVisible(visible);
        }
    }
}

void GameMap::CreateObject(unsigned int layerId, unsigned int objId,
                           unsigned int r0, unsigned int c0,
                           unsigned int rows, unsigned int cols)
{
    // object origin is out of map
    if(r0 >= mRows || c0 >= mCols)
        return ;

    // full size is out of map
    const unsigned int r1 = 1 + r0 - rows;
    const unsigned int c1 = 1 + c0 - cols;

    if(r1 >= mRows || c1 >= mCols)
        return ;

    const unsigned int ind0 = r0 * mCols + c0;

    GameMapCell & gcell = mCells[ind0];

    // cell is already full
    if(gcell.obj)
        return;

    // create game object
    GameObject * obj = nullptr;

    if(OBJ_RES_GEN_ENERGY == objId)
        obj = new ResourceGenerator(ResourceType::ENERGY, rows, cols);
    else if(OBJ_RES_GEN_MATERIAL1 == objId)
        obj = new ResourceGenerator(ResourceType::MATERIAL1, rows, cols);
    else if(objId >= OBJ_BASE_F1 && objId <= OBJ_BASE_F3)
    {
        const int owner = objId - OBJ_BASE_F1;

        obj = new Base(owner, rows, cols);

        Player * player = mGame->GetPlayer(owner);

        // base cells update
        for(unsigned int r = r1; r <= r0; ++r)
        {
            const unsigned int indBase = r * mCols;

            for(unsigned int c = c1; c <= c0; ++c)
            {
                const unsigned int ind = indBase + c;
                mCells[ind].owner = player;
                mCells[ind].linked = true;

                UpdateInfluencedCells(r, c);
            }
        }

        player->SetBaseCell(Cell2D(r0, c0));
        player->SumCells(rows * cols);
    }
    else if(objId >= OBJ_MOUNTAIN_FIRST && objId <= OBJ_MOUNTAIN_LAST)
        obj = new SceneObject(static_cast<GameObjectType>(objId), rows, cols);

    // set object properties
    obj->SetCell(&mCells[ind0]);
    obj->SetSize(rows, cols);

    // store object in map list
    mObjects.push_back(obj);

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
    Player * localPlayer = mGame->GetPlayer(0);

    if(obj->GetOwner() == localPlayer->GetPlayerId())
        AddPlayerObjVisibility(obj, localPlayer);
    else
        UpdateSceneObjVisibility(obj, localPlayer);
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
    if(COST_CONQUEST_CELL > player->GetEnergy())
        return false;

    return true;
}

void GameMap::StartConquerCell(const Cell2D & cell, Player * player)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    // take player's energy
    player->SumEnergy(-COST_CONQUEST_CELL);

    // mark cell as changing
    gcell.changing = true;
}

void GameMap::ConquerCell(const Cell2D & cell, Player * player)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    bool stolen = gcell.owner != nullptr && gcell.owner != player;

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
    if(player == mGame->GetPlayer(0))
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

bool GameMap::CanConquerResourceGenerator(const Cell2D & start, const Cell2D & end, Player * player)
{
    const unsigned int r0 = static_cast<unsigned int>(start.row);
    const unsigned int c0 = static_cast<unsigned int>(start.col);

    // start out of bounds
    if(r0 >= mRows || c0 >= mCols)
        return false;

    const unsigned int r1 = static_cast<unsigned int>(end.row);
    const unsigned int c1 = static_cast<unsigned int>(end.col);

    // end out of bounds
    if(r1 >= mRows || c1 >= mCols)
        return false;

    // check if player has enough energy
    if(COST_CONQUEST_RES_GEN > player->GetEnergy())
        return false;

    const int diffR = abs(end.row - start.row);

    // end too far - units can only conquest to next cell
    if(diffR > 1)
        return false;

    const int diffC = abs(end.col - start.col);

    // end too far - units can only conquest to next cell
    if(diffC > 1)
         return false;

    const int ind0 = r0 * mCols + c0;
    GameMapCell & gcell0 = mCells[ind0];

    const Unit * unit = gcell0.GetUnit();

    // start has no unit
    if(nullptr == unit)
        return false;

    // not player's unit
    if(unit->GetOwner() != player->GetPlayerId())
        return false;

    const int ind1 = r1 * mCols + c1;
    GameMapCell & gcell1 = mCells[ind1];

    // end has no resource generator
    if(!gcell1.HasResourceGenerator())
        return false;

    // player already owns the res gen
    if(gcell1.owner == player)
        return false;

    // TEMP - no conquest while another is in progress
    if(gcell1.changing)
        return false;

    return true;
}

void GameMap::StartConquerResourceGenerator(const Cell2D & start, const Cell2D & end, Player * player)
{
    // take player's energy
    player->SumEnergy(-COST_CONQUEST_RES_GEN);

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

void GameMap::ConquerResourceGenerator(const Cell2D & start, const Cell2D & end, Player * player)
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
    obj->SetOwner(player->GetPlayerId());

    // update player
    player->SumCells(1);
    player->AddResourceGenerator(ind, static_cast<ResourceGenerator *>(gcell1.obj));

    // reset start changing flag
    const int ind0 = start.row * mCols + start.col;
    mCells[ind0].changing = false;

    // update map
    UpdateLinkedCells(player);

    // update visibility
    Player * localPlayer = mGame->GetPlayer(0);

    if(player == localPlayer)
    {
        AddPlayerObjVisibility(obj, localPlayer);

        // TODO this should be optimized as it's affecting the whole map
        // when only a small portion is changed
        ApplyVisibility(localPlayer);
    }
}

bool GameMap::CanCreateUnit(GameObject * gen, Player * player)
{
    // generator is not owned by the player
    if(gen->GetOwner() != player->GetPlayerId())
        return false;

    // only base can generate units (for now)
    if(gen->GetObjectType() != OBJ_BASE)
        return false;

    // generator is already busy
    if(gen->IsBusy())
       return false;

    // check if player has enough resources
    if(ENERGY_NEW_UNIT > player->GetEnergy() ||
       MATERIAL_NEW_UNIT > player->GetMaterial())
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

void GameMap::StartCreateUnit(const Cell2D & dest, Player * player)
{
    const int ind = dest.row * mCols + dest.col;
    GameMapCell & gcell = mCells[ind];

    // make player pay
    player->SumEnergy(-ENERGY_NEW_UNIT);
    player->SumMaterial(-MATERIAL_NEW_UNIT);

    // mark cell as changing
    gcell.changing = true;
}

void GameMap::CreateUnit(const Cell2D & dest, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(dest.row);
    const unsigned int c = static_cast<unsigned int>(dest.col);

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    Unit * unit = new Unit(player->GetPlayerId(), 1, 1);
    unit->SetCell(&mCells[ind]);

    // update cell
    gcell.obj = unit;
    gcell.walkable = false;
    gcell.changing = false;

    mIsoMap->GetLayer(OBJECTS)->AddObject(unit->GetIsoObject(), r, c);

    mObjects.push_back(unit);

    // update player
    player->SumUnits(1);
    player->SumTotalUnitsLevel(unit->GetUnitLevel() + 1);

    // update visibility map
    // NOTE only for human player for now
    Player * localPlayer = mGame->GetPlayer(0);

    if(unit->GetOwner() == localPlayer->GetPlayerId())
        AddPlayerObjVisibility(unit, localPlayer);
    else
        UpdateSceneObjVisibility(unit, localPlayer);

    ApplyVisibility(localPlayer);
}

bool GameMap::CanDestroyUnit(const Cell2D & cell, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);

    // out of bounds
    if(!(r < mRows && c < mCols))
        return false;

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    // not own cell or no units -> exit
    if(gcell.owner != player || !gcell.HasUnit())
        return false;

    return true;
}

void GameMap::DestroyUnit(const Cell2D & cell, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);
    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    const Unit * unit = gcell.GetUnit();
    const int unitLevel = unit->GetUnitLevel();

    // update player
    player->SumUnits(-1);
    player->SumTotalUnitsLevel(-(unitLevel + 1));

    // remove units from cell
    delete gcell.obj;
    gcell.obj = nullptr;

    // destroy object
    mIsoMap->GetLayer(OBJECTS)->ClearObject(r, c);
}

bool GameMap::CanUpgradeUnit(const Cell2D & cell, Player * player)
{
    const unsigned int r = static_cast<unsigned int>(cell.row);
    const unsigned int c = static_cast<unsigned int>(cell.col);

    // out of bounds
    if(!(r < mRows && c < mCols))
        return false;

    const int ind = r * mCols + c;
    GameMapCell & gcell = mCells[ind];

    const Unit * unit = gcell.GetUnit();

    // cell already changing, not own cell or no unit -> exit
    if(gcell.changing ||
       gcell.owner != player ||
       nullptr == unit)
        return false;

    // check if reached max level for units
    const int unitLevel = unit->GetUnitLevel();

    if(MAX_UNITS_LEVEL == unitLevel)
        return false;

    // check if player has enough energy
    const int cost = COST_UNIT_UPGRADE[unitLevel];

    if(cost > player->GetEnergy())
        return false;

    return true;
}

void GameMap::StartUpgradeUnit(const Cell2D & cell, Player * player)
{
    const int ind = cell.row * mCols + cell.col;
    GameMapCell & gcell = mCells[ind];

    // make player pay
    const Unit * unit = gcell.GetUnit();
    const int unitLevel = unit->GetUnitLevel();
    const int cost = COST_UNIT_UPGRADE[unitLevel];
    player->SumEnergy(-cost);

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
    if(unit0->GetOwner() != player->GetPlayerId())
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

bool GameMap::MoveUnits(const Cell2D & start, const Cell2D & end, Player * player)
{
    if(!CanUnitMove(start, end, player))
        return false;

    const int ind0 = start.row * mCols + start.col;
    GameMapCell & gcell0 = mCells[ind0];

    const int ind1 = end.row * mCols + end.col;
    GameMapCell & gcell1 = mCells[ind1];

    const bool emptyDest = nullptr == gcell1.obj;

    // move to empty cell
    if(emptyDest)
    {
        if(player == mGame->GetPlayer(0))
            DelPlayerObjVisibility(gcell0.obj, player);

        gcell1.obj = gcell0.obj;
        gcell1.walkable = false;

        gcell0.obj = nullptr;
        gcell0.walkable = true;

        IsoLayer * layerUnits = mIsoMap->GetLayer(OBJECTS);
        layerUnits->MoveObject(gcell0.row, gcell0.col, gcell1.row, gcell1.col);

        gcell1.obj->SetCell(&gcell1);

        if(player == mGame->GetPlayer(0))
        {
            AddPlayerObjVisibility(gcell1.obj, player);
            ApplyVisibility(player);
        }
    }
    else
        return false;

    // check for victory or game over
    CheckGameEnd();

    return true;
}

void GameMap::CheckGameEnd()
{
    const int numPlayers = mGame->GetNumPlayers();
    int defeated = 0;

    // check for game over and defeated opponents
    for(int i = 0; i < numPlayers; ++i)
    {
        const Player * p = mGame->GetPlayer(i);

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

// ==================== PRIVATE METHODS ====================

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
    if(!mGame->GetPlayer(0)->IsCellVisible(ind))
        return FOG_OF_WAR;

    // scene cell
    if(SCENE == cell.basicType)
        return SCENE;

    const int ownerId = cell.owner ? cell.owner->GetPlayerId() : -1;

    int type = EMPTY;

    switch(ownerId)
    {
        case 0:
            if(cell.linked)
                type = F1_CONNECTED;
            else
                type = F1;
        break;

        case 1:
            if(cell.linked)
                type = F2_CONNECTED;
            else
                type = F2;
        break;

        case 2:
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
                type = EMPTY;
        }
        break;
    }

    return type;
}

void GameMap::UpdateLinkedCells(Player * player)
{
    // CLEAR ALL LINKED STATUS
    for(GameMapCell & cell : mCells)
    {
        if(cell.owner == player)
            cell.linked = false;
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

        if(cell.owner == player || cell.influencer != -1)
        UpdateCellType(ind, cell);
    }
}

void GameMap::UpdateInfluencedCells(int row, int col)
{
    const unsigned int ind0 = row * mCols + col;
    GameMapCell & gcell = mCells[ind0];

    // not linked cells have no influence
    if(!gcell.linked)
        return ;

    const int ownerId = gcell.owner->GetPlayerId();

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
            gc.influencers[ownerId] = true;

            // count active influencers to see if there's only one
            int influencers = 0;

            for(auto it : gc.influencers)
            {
                if(it.second)
                    ++influencers;
            }

            if(influencers == 1)
                gc.influencer = ownerId;
            else
                gc.influencer = -1;

            UpdateCellType(ind, gc);
        }
    }
}

void GameMap::AddPlayerObjVisibility(GameObject * obj, Player * player)
{
    const int objRows = obj->GetRows();

    const int objVisLvl = obj->GetVisibilityLevel();

    const int visHalfLen0 = 2;
    const int visLenInc = 1;
    const int visExtSide = visHalfLen0 + visLenInc * objVisLvl;

    const int visLenMax = objRows + visExtSide * 2;

    const int visSideCols = (visLenMax - 1) / 2;

    // object columns
    int minCol = obj->GetCol1();
    int maxCol = obj->GetCol0();

    if(objVisLvl % 2 == 1)
    {
        if(minCol > 0)
            minCol -= 1;

        if(maxCol < static_cast<int>(mCols - 1))
            maxCol += 1;
    }

    int minRow = obj->GetRow1() - visExtSide;
    int maxRow = obj->GetRow0() + visExtSide;

    for(int c = minCol; c <= maxCol; ++c)
    {
        for(int r = minRow; r <= maxRow; ++r)
        {
            if(r < 0 || r >= static_cast<int>(mRows))
                continue;

            const int ind = r * mCols + c;

            player->AddVisibility(ind);

            // if there's any object mark it as visited
            if(mCells[ind].obj != nullptr)
                mCells[ind].obj->SetVisited();
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

        for(int c = minColL; c <= maxColL; ++c)
        {
            ++minRowL;
            --maxRowL;

            for(int r = minRowL; r <= maxRowL; ++r)
            {
                if(r < 0 || r >= static_cast<int>(mRows))
                    continue;

                const int ind = r * mCols + c;

                player->AddVisibility(ind);

                // if there's any object mark it as visited
                if(mCells[ind].obj != nullptr)
                    mCells[ind].obj->SetVisited();
            }
        }
    }

    // right columns
    if(maxCol + 1 < static_cast<int>(mCols))
    {
        int minColR = maxCol + 1;
        int maxColR = maxCol + visSideCols;

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

                player->AddVisibility(ind);

                // if there's any object mark it as visited
                if(mCells[ind].obj != nullptr)
                    mCells[ind].obj->SetVisited();
            }
        }
    }
}

void GameMap::DelPlayerObjVisibility(GameObject * obj, Player * player)
{
    const int radius = obj->GetVisibilityLevel();

    const int rowTL = (obj->GetRow1() - radius) > 0 ? (obj->GetRow1() - radius) : 0;
    const int colTL = (obj->GetCol1() - radius) > 0 ? (obj->GetCol1() - radius) : 0;
    const int rowBR = (obj->GetRow0() + radius + 1) < static_cast<int>(mRows) ? (obj->GetRow0() + radius + 1) : mRows;
    const int colBR = (obj->GetCol0() + radius + 1) < static_cast<int>(mCols) ? (obj->GetCol0() + radius + 1) : mCols;

    // add the visibility of the object to the map
    for(int r = rowTL; r < rowBR; ++r)
    {
        const int indBase = r * mCols;

        for(int c = colTL; c < colBR; ++c)
        {
            const int ind = indBase + c;

            player->RemVisibility(ind);
        }
    }
}

void GameMap::UpdateSceneObjVisibility(GameObject * obj, Player * player)
{
    const int rTL = obj->GetRow1();
    const int cTL = obj->GetCol1();
    const int rBR = obj->GetRow0();
    const int cBR = obj->GetCol0();

    // check if any cell is visible
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
        }

        if(visible)
            break;
    }
}

void GameMap::AddPlayerCellVisibility(const GameMapCell & cell, Player * player)
{
    const int radius = 1;

    const int rowTL = (cell.row - radius) > 0 ? (cell.row - radius) : 0;
    const int colTL = (cell.col - radius) > 0 ? (cell.col - radius) : 0;
    const int rowBR = (cell.row + radius + 1) < static_cast<int>(mRows) ? (cell.row + radius + 1) : mRows;
    const int colBR = (cell.col + radius + 1) < static_cast<int>(mCols) ? (cell.col + radius + 1) : mCols;

    // add the visibility of the object to the map
    for(int r = rowTL; r < rowBR; ++r)
    {
        const int indBase = r * mCols;

        for(int c = colTL; c < colBR; ++c)
        {
            const int ind = indBase + c;

            player->AddVisibility(ind);

            // if there's any object mark it as visited
            if(mCells[ind].obj != nullptr)
                mCells[ind].obj->SetVisited();
        }
    }
}

void GameMap::DelPlayerCellVisibility(const GameMapCell & cell, Player * player)
{
    const int radius = 1;

    const int rowTL = (cell.row - radius) > 0 ? (cell.row - radius) : 0;
    const int colTL = (cell.col - radius) > 0 ? (cell.col - radius) : 0;
    const int rowBR = (cell.row + radius + 1) < static_cast<int>(mRows) ? (cell.row + radius + 1) : mRows;
    const int colBR = (cell.col + radius + 1) < static_cast<int>(mCols) ? (cell.col + radius + 1) : mCols;

    // add the visibility of the object to the map
    for(int r = rowTL; r < rowBR; ++r)
    {
        const int indBase = r * mCols;

        for(int c = colTL; c < colBR; ++c)
        {
            const int ind = indBase + c;

            player->RemVisibility(ind);
        }
    }
}

} // namespace game
