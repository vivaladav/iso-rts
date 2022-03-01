#include "AI/ConquerPath.h"

#include "GameConstants.h"
#include "GameMap.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "IsoObject.h"
#include "Player.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Unit.h"
#include "Indicators/ConquestIndicator.h"
#include "Screens/ScreenGame.h"

#include <cmath>

namespace game
{

ConquerPath::~ConquerPath()
{
    // delete the ConquestIndicators
    for(auto ind : mConquestIndicators)
        delete ind;
}

void ConquerPath::CreateIndicators()
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS1);

    Player * player = mUnit->GetOwner();

    const PlayerFaction faction = player->GetFaction();

    // do not create indicator for cell 0 as it's current under conquest
    for(unsigned int i = 1; i < mCells.size(); ++i)
    {
        ConquestIndicator * ind = new ConquestIndicator;
            mConquestIndicators.emplace_back(ind);

        // add indicator to layer
        const unsigned int pathInd = mCells[i];
        const unsigned int indRow = pathInd / mIsoMap->GetNumCols();
        const unsigned int indCol = pathInd % mIsoMap->GetNumCols();

        layer->AddObject(ind, indRow, indCol);

        ind->SetFaction(faction);
    }
}

void ConquerPath::InitNextConquest()
{
    mState = CONQUER_CELLING;

    const unsigned int nextInd = mCells[mNextCell];
    const unsigned int nextRow = nextInd / mIsoMap->GetNumCols();
    const unsigned int nextCol = nextInd % mIsoMap->GetNumCols();
    const Cell2D nextCell(nextRow, nextCol);

    Player * player = mUnit->GetOwner();

    // check if conquest is possible
    if(!mGameMap->CanConquerCell(nextCell, player))
    {
        TransitionToMoveStep();

        return ;
    }

    if(!mUnit->HasEnergyForAction(CONQUER_CELL))
    {
        mState = FAILED;

        // clear action data once the action is completed
        mScreen->SetObjectActionCompleted(mUnit);

        // clear indicators
        IsoLayer * layerOverlay = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS1);
        layerOverlay->ClearObjects();

        return ;
    }

    // start conquest
    mGameMap->StartConquerCell(nextCell, player);

    // TODO get conquer time from unit
    constexpr float TIME_CONQ_CELL = 0.5f;
    mScreen->CreateProgressBar(nextCell, TIME_CONQ_CELL, player, [this, nextCell, player]
    {
        mGameMap->ConquerCell(nextCell, player);

        mUnit->ConsumeEnergy(CONQUER_CELL);

        TransitionToMoveStep();
    });
}

void ConquerPath::TransitionToMoveStep()
{
    ++mNextCell;

    if(mNextCell < mCells.size())
    {
        const unsigned int nextInd = mCells[mNextCell];
        const unsigned int nextRow = nextInd / mIsoMap->GetNumCols();
        const unsigned int nextCol = nextInd % mIsoMap->GetNumCols();

        // check if next destination is walkable
        const GameMapCell & nextCell = mGameMap->GetCell(nextRow, nextCol);

        bool canMove = nextCell.walkable && mUnit->HasEnergyForAction(MOVE);

        if(canMove)
            InitNextMoveStep();
        else
        {
            mState = FAILED;

            // clear action data once the action is completed
            mScreen->SetObjectActionCompleted(mUnit);

            // clear indicators
            IsoLayer * layerOverlay = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS1);
            layerOverlay->ClearObjects();
        }
    }
    else
    {
        mState = COMPLETED;

        // clear action data once the action is completed
        mScreen->SetObjectActionCompleted(mUnit);
    }
}

void ConquerPath::InitNextMoveStep()
{
    mState = MOVING;

    const IsoObject * isoObj = mUnit->GetIsoObject();
    const IsoLayer * layerObj = isoObj->GetLayer();

    mObjX = isoObj->GetX();
    mObjY = isoObj->GetY();

    const unsigned int nextInd = mCells[mNextCell];
    const unsigned int nextRow = nextInd / mIsoMap->GetNumCols();
    const unsigned int nextCol = nextInd % mIsoMap->GetNumCols();
    const sgl::core::Pointd2D target = layerObj->GetObjectPosition(isoObj, nextRow, nextCol);
    mTargetX = target.x;
    mTargetY = target.y;

    mVelX = (mTargetX - mObjX) * mUnit->GetSpeed();
    mVelY = (mTargetY - mObjY) * mUnit->GetSpeed();
}

void ConquerPath::UpdatePathCost()
{
    // TODO proper cost computation
    mCost = (mCells.size() - 1) * 0.5f;
}

void ConquerPath::FinishAbortion()
{
    // clear progress bar
    const unsigned int nextInd = mCells[mNextCell];
    const unsigned int nextRow = nextInd / mIsoMap->GetNumCols();
    const unsigned int nextCol = nextInd % mIsoMap->GetNumCols();
    const Cell2D cell(nextRow, nextCol);

    mScreen->CancelProgressBar(cell);

    mGameMap->SetCellChanging(nextRow, nextCol, false);

    // clear indicators
    IsoLayer * layerOverlay = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS1);
    layerOverlay->ClearObjects();

    // set new state
    mState = ABORTED;
}

void ConquerPath::Start()
{
    // do nothing if already started
    if(mState != READY)
        return ;

    CreateIndicators();

    // stat conquering first cell
    InitNextConquest();
}

void ConquerPath::Abort()
{
    if(CONQUER_CELLING == mState)
        FinishAbortion();
    else
        mState = ABORTING;
}

void ConquerPath::Update(float delta)
{
    // only do something while moving
    if(mState != MOVING && mState != ABORTING)
        return ;

    int todo = 2;

    // -- X --
    mObjX += mVelX * delta;

    if(mVelX < 0.f)
    {
        if(mObjX < mTargetX)
        {
            --todo;
            mObjX = mTargetX;
        }
    }
    else if(mVelX > 0.f)
    {
        if(mObjX > mTargetX)
        {
            --todo;
            mObjX = mTargetX;
        }
    }
    else
        --todo;

    // -- Y --
    mObjY += mVelY * delta;

    if(mVelY < 0.f)
    {
        if(mObjY < mTargetY)
        {
            --todo;
            mObjY = mTargetY;
        }
    }
    else if(mVelY > 0.f)
    {
        if(mObjY > mTargetY)
        {
            --todo;
            mObjY = mTargetY;
        }
    }
    else
        --todo;

    // position object
    IsoObject * isoObj = mUnit->GetIsoObject();
    isoObj->SetX(static_cast<int>(std::roundf(mObjX)));
    isoObj->SetY(static_cast<int>(std::roundf(mObjY)));

    // handle reached target
    if(0 == todo)
    {
        // remove the ConquestIndicator from the new cell
        IsoLayer * layerOverlay = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS1);
        layerOverlay->ClearObject(mConquestIndicators[mNextCell - 1]);

        // handle object movement in maps
        Player * player = mUnit->GetOwner();

        mGameMap->DelPlayerObjVisibility(mUnit, player);

        const unsigned int targetInd = mCells[mNextCell];
        const unsigned int targetRow = targetInd / mIsoMap->GetNumCols();
        const unsigned int targetCol = targetInd % mIsoMap->GetNumCols();
        const GameMapCell & targetCell = mGameMap->GetCell(targetRow, targetCol);

        // collect collectable object, if any
        if(targetCell.walkable && targetCell.obj != nullptr)
        {
            player->HandleCollectable(targetCell.obj);

            mGameMap->RemoveAndDestroyObject(targetCell.obj);
        }

        // handle moving object
        mGameMap->MoveObjToCell(mUnit, targetRow, targetCol);
        mGameMap->AddPlayerObjVisibility(mUnit, player);
        mGameMap->ApplyVisibility(player);

        mUnit->ConsumeEnergy(MOVE);

        // init next step
        if(ABORTING == mState)
            FinishAbortion();
        else
            InitNextConquest();
    }
}

} // namespace game
