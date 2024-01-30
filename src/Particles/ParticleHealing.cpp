#include "Particles/ParticleHealing.h"

#include "GameMap.h"
#include "GameObjects/GameObject.h"
#include "Particles/DataParticleHealing.h"

#include <cmath>

namespace game
{

void ParticleHealing::SetData(const DataParticleHealing & data)
{
    mRenderable.SetTexture(data.tex);
    SetSpeed(data.speed);
    SetStartAndTarget(data.x0, data.y0, data.targetX, data.targetY);

    mHealer = data.healer;
    mGameMap = data.map;
    mTarget = data.target;
    mHeal = data.heal;
}

void ParticleHealing::SetStartAndTarget(int x0, int y0, int tx, int ty)
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
    mRenderable.SetPosition(x0, y0);

    // reset done flags
    mDoneX = false;
    mDoneY = false;
}

void ParticleHealing::Update(float delta)
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

        mRenderable.SetX(posX);
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

        mRenderable.SetY(posY);
    }

    // DONE!
    if(mDoneX && mDoneY)
        SetDone();
}

void ParticleHealing::OnDone()
{
    // target is gone
    if(!mGameMap->HasObject(mTarget))
        return ;

    mTarget->SumHealth(mHeal);
}

} // namespace game
