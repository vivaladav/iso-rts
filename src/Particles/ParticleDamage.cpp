#include "Particles/ParticleDamage.h"

#include "Particles/DataParticleDamage.h"

#include <cmath>

namespace game
{

void ParticleDamage::SetData(const DataParticleDamage & data)
{
    mRenderable.SetTexture(data.tex);
    mRenderable.SetRotation(data.angle);
    mRenderable.SetColor(data.color);

    SetScale(data.scale);
    SetStart(data.x0, data.y0);

    mSpeed = data.speed;
    mVelX = data.velX;
    mVelY = data.velY;
    mDecaySpeed = data.decaySpeed;

    const float maxAlpha = 255.f;
    mAlpha = maxAlpha;
}

void ParticleDamage::SetScale(float scale)
{
    mRenderable.RestoreOriginalSize();
    mRenderable.Scale(scale);
}

void ParticleDamage::SetStart(int x0, int y0)
{
    // adjust initial position based on rotation
    const int rdX = roundf((mRenderable.GetWidth() - mRenderable.GetRotatedWidth()) * 0.5f);
    const int rdY = roundf((mRenderable.GetHeight() - mRenderable.GetRotatedHeight()) * 0.5f);

    mPosXf = x0 - rdX;
    mPosYf = y0 - rdY;

    // set initial position
    mRenderable.SetPosition(static_cast<int>(mPosXf), static_cast<int>(mPosYf));
}

void ParticleDamage::Update(float delta)
{
    const float speed = mSpeed * delta;
    const float alphaDecay = mDecaySpeed * delta;

    // update alpha
    mAlpha -= alphaDecay;
    mRenderable.SetAlpha(static_cast<unsigned char>(mAlpha));

    // move particle
    mPosXf += mVelX * speed;
    const int posX = roundf(mPosXf);
    mRenderable.SetX(posX);

    mPosYf += mVelY * speed;
    int posY = roundf(mPosYf);
    mRenderable.SetY(posY);

    // DONE!
    const int minAlpha = 1.f;

    if(mAlpha < minAlpha)
        SetDone();
}

} // namespace game
