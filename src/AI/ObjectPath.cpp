#include "AI/ObjectPath.h"

#include "IsoLayer.h"
#include "IsoMap.h"
#include "IsoObject.h"
#include "GameObjects/GameObject.h"

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
    IsoObject * isoObj = mObj->GetIsoObject();
    isoObj->SetX(static_cast<int>(mObjX));
    isoObj->SetY(static_cast<int>(mObjY));

    // handle reached target
    if(0 == todo)
    {
        if(mNextCell < mCells.size())
            InitNextMoveStep();
        else
            mState = COMPLETED;
    }
}

} // namespace game
