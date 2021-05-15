#include "Particles/ParticleSingleLaser.h"

#include <cmath>

namespace game
{

ParticleSingleLaser::ParticleSingleLaser()
{

}

void ParticleSingleLaser::SetStartAndTarget(int x0, int y0, int tx, int ty)
{
    // define velocity
    const float dx = static_cast<float>(tx - x0);
    const float dy = static_cast<float>(ty - y0);
    const float len = sqrtf(dx * dx + dy * dy);

    mVelX = dx / len;
    mVelY = dy / len;

    // store positions
    mPosXf = static_cast<float>(x0);
    mPosYf = static_cast<float>(y0);
    mTargetX = tx;
    mTargetY = ty;

    // set initial position
    SetPosition(x0, y0);

    mDoneX = false;
    mDoneY = false;
}

void ParticleSingleLaser::Move(float delta)
{
    const float speed = mSpeed * delta;

    if(!mDoneX)
    {
        mPosXf += mVelX * speed;

        int posX = roundf(mPosXf);

        if((mVelX > 0.f && posX >= mTargetX) || (mVelX < 0.f && posX <= mTargetX) || mVelX == 0.f)
        {
            posX = mTargetX;
            mDoneX = true;
        }

        SetX(posX);
    }

    if(!mDoneY)
    {
        mPosYf += mVelY * speed;

        int posY = roundf(mPosYf);

        if((mVelY > 0.f && posY >= mTargetY) || (mVelY < 0.f && posY <= mTargetY) || mVelY == 0.f)
        {
            posY = mTargetY;
            mDoneY = true;
        }

        SetY(posY);
    }
}

} // namespace game
