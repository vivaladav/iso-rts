#include "AI/ObjectPath.h"

#include "GameMap.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "IsoObject.h"
#include "Player.h"
#include "GameObjects/GameObject.h"

#include <cmath>

namespace game
{

void ObjectPath::InitNextMoveStep()
{
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
    isoObj->SetX(static_cast<int>(std::roundf(mObjX)));
    isoObj->SetY(static_cast<int>(std::roundf(mObjY)));

    // handle reached target
    if(0 == todo)
    {
        Player * player = mGameMap->GetObjectOwner(mObj);

        mGameMap->DelPlayerObjVisibility(mObj, player);

        const unsigned int targetInd = mCells[mNextCell - 1];
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

        // handle next step or termination
        if(mNextCell < mCells.size())
        {
            const unsigned int nextInd = mCells[mNextCell];
            const unsigned int nextRow = nextInd / mIsoMap->GetNumCols();
            const unsigned int nextCol = nextInd % mIsoMap->GetNumCols();

            // check if next destination is walkable
            const GameMapCell & nextCell = mGameMap->GetCell(nextRow, nextCol);

            if(nextCell.walkable)
                InitNextMoveStep();
            else
                mState = FAILED;
        }
        else
        {
            mState = COMPLETED;

            mOnCompleted();
        }
    }
}

void ObjectPath::UpdatePathCost()
{
    // TODO proper cost computation
    mCost = (mCells.size() - 1) * 0.5f;
}

} // namespace game
