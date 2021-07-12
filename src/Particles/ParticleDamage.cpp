#include "Particles/ParticleDamage.h"

#include "GameMap.h"
#include "GameObjects/GameObject.h"
#include "Particles/DataParticleDamage.h"

#include <cmath>

namespace game
{

void ParticleDamage::SetData(const DataParticleDamage & data)
{
    SetTexture(data.tex);
    SetRotation(data.angle);
    SetColor(data.color);

    SetStart(data.x0, data.y0);

    mSpeed = data.speed;
    mVelX = data.velX;
    mVelY = data.velY;
    mDecaySpeed = data.decaySpeed;

    const float maxAlpha = 255.f;
    mAlpha = maxAlpha;
}

void ParticleDamage::SetStart(int x0, int y0)
{
    // adjust initial position based on rotation
    const int rdX = roundf((GetWidth() - GetRotatedWidth()) * 0.5f);
    const int rdY = roundf((GetHeight() - GetRotatedHeight()) * 0.5f);

    mPosXf = x0 - rdX;
    mPosYf = y0 - rdY;

    // set initial position
    SetPosition(static_cast<int>(mPosXf), static_cast<int>(mPosYf));
}

void ParticleDamage::Update(float delta)
{
    const float speed = mSpeed * delta;
    const float alphaDecay = mDecaySpeed * delta;

    // update alpha
    mAlpha -= alphaDecay;
    SetAlpha(static_cast<unsigned char>(mAlpha));

    // move particle
    mPosXf += mVelX * speed;
    const int posX = roundf(mPosXf);
    SetX(posX);

    mPosYf += mVelY * speed;
    int posY = roundf(mPosYf);
    SetY(posY);

    // DONE!
    const int minAlpha = 1.f;

    if(mAlpha < minAlpha)
        SetDone();
}

void ParticleDamage::OnDone()
{
}

} // namespace game
