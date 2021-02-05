#include "AI/ObjectPath.h"

#include "GameMap.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "IsoObject.h"
#include "GameObjects/GameObject.h"

#include <cmath>

#include <iostream>

namespace game
{

void ObjectPath::InitNextMoveStep()
{
    const IsoObject * isoObj = mObj->GetIsoObject();
    const IsoLayer * layerObj = isoObj->GetLayer();

    mObjX = isoObj->GetX();
    mObjY = isoObj->GetY();

    const Cell2D & nc = mCells[mNextCell];
    const lib::core::Point2D target = layerObj->GetObjectPosition(isoObj, nc.row, nc.col);
    mTargetX = target.x;
    mTargetY = target.y;

    mVelX = (mTargetX - mObjX) * mObj->GetSpeed();
    mVelY = (mTargetY - mObjY) * mObj->GetSpeed();

    std::cout << "ObjectPath::InitNextMoveStep - VEL: " << mVelX << "," << mVelY <<
                 " | NEXT CELL: " << mNextCell <<
                 " | POS: " << mObjX << "," << mObjY <<
                 " | END: " << mTargetX << "," << mTargetY <<
                 std::endl;

    mState = RUNNING;

    ++mNextCell;
}

void ObjectPath::Update(float delta)
{
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
    //std::cout << "ObjectPath::Update - POS: " << mObjX << "," << mObjY << std::endl;
    IsoObject * isoObj = mObj->GetIsoObject();
    isoObj->SetX(static_cast<int>(std::roundf(mObjX)));
    isoObj->SetY(static_cast<int>(std::roundf(mObjY)));

    // handle reached target
    if(0 == todo)
    {
        Player * player = mGameMap->GetObjectOwner(mObj);

        mGameMap->DelPlayerObjVisibility(mObj, player);

        const Cell2D & nc = mCells[mNextCell - 1];
        mObj->SetCell(&mGameMap->GetCell(nc.row, nc.col));

        mGameMap->AddPlayerObjVisibility(mObj, player);

        mGameMap->ApplyVisibility(player);

        if(mNextCell < mCells.size())
            InitNextMoveStep();
        else
            mState = COMPLETED;
    }
}

} // namespace game
