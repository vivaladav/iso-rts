#include "AI/ConquerPath.h"

#include "GameConstants.h"
#include "GameMap.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "IsoObject.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Unit.h"
#include "Screens/ScreenGame.h"

#include <cmath>

namespace game
{

void ConquerPath::InitNextConquest()
{
    mState = CONQUERING;

    const unsigned int nextInd = mCells[mNextCell];
    const unsigned int nextRow = nextInd / mIsoMap->GetNumCols();
    const unsigned int nextCol = nextInd % mIsoMap->GetNumCols();
    const Cell2D nextCell(nextRow, nextCol);

    Player * player = mGameMap->GetObjectOwner(mObj);

    // check if conquest is possible
    if(!mGameMap->CanConquerCell(nextCell, player))
    {
        TransitionToMoveStep();

        return ;
    }

    // start conquest
    mGameMap->StartConquerCell(nextCell, player);

    // TODO get conquer time from unit
    constexpr float TIME_CONQ_CELL = 2.f;
    mScreen->CreateProgressBar(nextCell, TIME_CONQ_CELL, player, [this, nextCell, player]
    {
        mGameMap->ConquerCell(nextCell, player);

        TransitionToMoveStep();
    });
}

void ConquerPath::TransitionToMoveStep()
{
    ++mNextCell;

    if(mNextCell < mCells.size())
        InitNextMoveStep();
    else
    {
        mState = COMPLETED;
        static_cast<Unit *>(mObj)->SetActiveAction(UnitAction::MOVE);
    }
}

void ConquerPath::InitNextMoveStep()
{
    mState = MOVING;

    const IsoObject * isoObj = mObj->GetIsoObject();
    const IsoLayer * layerObj = isoObj->GetLayer();

    mObjX = isoObj->GetX();
    mObjY = isoObj->GetY();

    const unsigned int nextInd = mCells[mNextCell];
    const unsigned int nextRow = nextInd / mIsoMap->GetNumCols();
    const unsigned int nextCol = nextInd % mIsoMap->GetNumCols();
    const lib::core::Point2D target = layerObj->GetObjectPosition(isoObj, nextRow, nextCol);
    mTargetX = target.x;
    mTargetY = target.y;

    mVelX = (mTargetX - mObjX) * mObj->GetSpeed();
    mVelY = (mTargetY - mObjY) * mObj->GetSpeed();
}

void ConquerPath::Start()
{
    // do nothing if already started
    if(mState != READY)
        return ;

    // stat conquering first cell
    InitNextConquest();
}

void ConquerPath::Update(float delta)
{
    // only do something while moving
    if(mState != MOVING)
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
        Player * player = mGameMap->GetObjectOwner(mObj);

        mGameMap->DelPlayerObjVisibility(mObj, player);

        const unsigned int targetInd = mCells[mNextCell];
        const unsigned int targetRow = targetInd / mIsoMap->GetNumCols();
        const unsigned int targetCol = targetInd % mIsoMap->GetNumCols();

        IsoLayer * layer = mObj->GetIsoObject()->GetLayer();
        layer->MoveObject(mObj->GetRow0(), mObj->GetCol0(), targetRow, targetCol, false);

        mGameMap->MoveObjToCell(mObj, targetRow, targetCol);

        mGameMap->AddPlayerObjVisibility(mObj, player);

        mGameMap->ApplyVisibility(player);

        // init next conquest
        InitNextConquest();
    }
}

} // namespace game
