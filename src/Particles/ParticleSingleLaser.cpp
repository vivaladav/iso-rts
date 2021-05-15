#include "Particles/ParticleSingleLaser.h"

#include "Particles/DataParticleSingleLaser.h"

#include <cmath>

namespace game
{

void ParticleSingleLaser::SetData(const DataParticleSingleLaser & data)
{
    SetTexture(data.tex);
    SetRotation(data.angle);
    SetSpeed(data.speed);
    // this has to be called after SetRotation
    SetStartAndTarget(data.x0, data.y0, data.targetX, data.targetY);
}

void ParticleSingleLaser::SetStartAndTarget(int x0, int y0, int tx, int ty)
{
    // adjust initial position based on rotation
    const double rot = GetRotation();
    int x = x0;
    int y = y0;

    const int rdX = roundf((GetWidth() - GetRotatedWidth()) * 0.5f);
    const int rdY = roundf((GetHeight() - GetRotatedHeight()) * 0.5f);

    if(rot < 90)
    {
        x = x0 - rdX;
        y = y0 - rdY;
    }
    else if(rot < 180)
    {
        x = x0 - GetRotatedWidth() - rdX;
        y = y0 - rdY;
    }
    else if(rot < 270)
    {
        x = x0 - GetRotatedWidth() - rdX;
        y = y0;
    }
    // rot < 360
    else
    {
        x = x0;
        y = y0;
    }

    // define velocity
    const float dx = static_cast<float>(tx - x);
    const float dy = static_cast<float>(ty - y);
    const float len = sqrtf(dx * dx + dy * dy);

    mVelX = dx / len;
    mVelY = dy / len;

    // store positions
    mPosXf = static_cast<float>(x);
    mPosYf = static_cast<float>(y);
    mTargetX = tx;
    mTargetY = ty;

    // set initial position
    SetPosition(x, y);

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
