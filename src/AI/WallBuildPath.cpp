#include "AI/WallBuildPath.h"

#include "GameConstants.h"
#include "GameMap.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "IsoObject.h"
#include "Player.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Unit.h"
#include "Indicators/WallIndicator.h"
#include "Screens/ScreenGame.h"

#include <cmath>

namespace game
{

WallBuildPath::~WallBuildPath()
{
    // delete the ConquestIndicators
    for(auto ind : mIndicators)
        delete ind;
}

void WallBuildPath::CreateIndicators()
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS1);

    Player * player = mObj->GetOwner();

    const PlayerFaction faction = player->GetFaction();

    for(unsigned int i = 1; i < mCells.size(); ++i)
    {
        auto ind = new WallIndicator;
        mIndicators.emplace_back(ind);

        // add indicator to layer
        const unsigned int pathInd = mCells[i];
        const unsigned int indRow = pathInd / mIsoMap->GetNumCols();
        const unsigned int indCol = pathInd % mIsoMap->GetNumCols();

        layer->AddObject(ind, indRow, indCol);

        ind->SetFaction(faction);
    }
}

void WallBuildPath::InitNextBuild()
{
    mState = BUILDING;

    const unsigned int nextInd = mCells[mNextCell];
    const unsigned int nextRow = nextInd / mIsoMap->GetNumCols();
    const unsigned int nextCol = nextInd % mIsoMap->GetNumCols();
    const Cell2D nextCell(nextRow, nextCol);

    Player * player = mObj->GetOwner();

    // check if conquest is possible
    if(!mGameMap->CanConquerCell(nextCell, player))
    {
        //TransitionToMoveStep();

        return ;
    }

    // start conquest
    mGameMap->StartConquerCell(nextCell, player);

    // TODO get conquer time from unit
    constexpr float TIME_CONQ_CELL = 2.f;
    mScreen->CreateProgressBar(nextCell, TIME_CONQ_CELL, player, [this, nextCell, player]
    {
        mGameMap->ConquerCell(nextCell, player);

        //TransitionToMoveStep();
    });
}

void WallBuildPath::UpdatePathCost()
{
    // TODO proper cost computation
    mEnergyCost = (mCells.size() - 1) * 5.f;
    mMaterialCost = (mCells.size() - 1) * 10.f;
}

void WallBuildPath::Start()
{
    // do nothing if already started
    if(mState != READY)
        return ;

    CreateIndicators();

    // stat conquering first cell
    InitNextBuild();
}

void WallBuildPath::Update(float delta)
{
    /*
    // only do something while moving
    if(mState != BUILDING)
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
    IsoObject * isoObj = mObj->GetIsoObject();
    isoObj->SetX(static_cast<int>(std::roundf(mObjX)));
    isoObj->SetY(static_cast<int>(std::roundf(mObjY)));

    // handle reached target
    if(0 == todo)
    {
        // remove the ConquestIndicator from the new cell
        IsoLayer * layerOverlay = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS1);
        layerOverlay->ClearObject(mConquestIndicators[mNextCell - 1]);

        // handle object movement in maps
        Player * player = mObj->GetOwner();

        mGameMap->DelPlayerObjVisibility(mObj, player);

        const unsigned int targetInd = mCells[mNextCell];
        const unsigned int targetRow = targetInd / mIsoMap->GetNumCols();
        const unsigned int targetCol = targetInd % mIsoMap->GetNumCols();
        const GameMapCell & targetCell = mGameMap->GetCell(targetRow, targetCol);

        // collect collectable object, if any
        if(targetCell.walkable && targetCell.obj != nullptr)
        {
            player->HandleCollectable(targetCell.obj);

            mGameMap->DestroyObject(targetCell.obj);
        }

        // handle moving object
        IsoLayer * layer = mObj->GetIsoObject()->GetLayer();
        layer->MoveObject(mObj->GetRow0(), mObj->GetCol0(), targetRow, targetCol, false);

        mGameMap->MoveObjToCell(mObj, targetRow, targetCol);

        mGameMap->AddPlayerObjVisibility(mObj, player);

        mGameMap->ApplyVisibility(player);

        // init next conquest
        InitNextBuild();
    }
    */
}

} // namespace game
