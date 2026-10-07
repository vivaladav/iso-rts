#include "Particles/ParticleHitPoints.h"

#include "Particles/DataParticleHitPoints.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/TexturedRenderable.h>
#include <sgl/graphic/TextureManager.h>

#include <cmath>

namespace game
{

ParticleHitPoints::~ParticleHitPoints()
{
    delete mTxt;
}

void ParticleHitPoints::SetData(const DataParticleHitPoints & data)
{
    using namespace sgl;

    // init data
    mSpeed = -data.speed;
    mDecaySpeed = data.decaySpeed;
    mMaxDistance = data.maxDistance;

    SetDelay(data.delay);

    const float maxAlpha = 255.f;
    mAlpha = maxAlpha;

    // clear visuals
    delete mTxt;

    // TEXT
    const int fontSize = 24;
    auto fm = graphic::FontManager::Instance();
    graphic::Font * font = fm->GetFont("Lato-Bold.ttf", fontSize, graphic::Font::NORMAL);

    // miss
    if(data.miss)
    {
        mTxt = new graphic::Text("MISS", font);
        mTxt->SetColor(0xefef90ff);
    }
    // hit
    else
    {
        const unsigned int colors[] = { 0xeff0f5ff, 0xff8a80ff };

        mTxt = new graphic::Text(std::to_string(data.value).c_str(), font);
        mTxt->SetColor(colors[data.fatal]);
    }

    SetStart(data.x0, data.y0);
}

void ParticleHitPoints::SetStart(int x0, int y0)
{
    mPosXf = x0;
    mPosYf = y0;
    mPosY0f = y0;

    const int halfW = mTxt->GetWidth() / 2;

    // text
    const int txtX = x0 - halfW;
    const int txtY = y0;
    mTxt->SetPosition(txtX, txtY);
}

void ParticleHitPoints::Update(float delta)
{
    const float speed = mSpeed * delta;
    const float alphaDecay = mDecaySpeed * delta;

    // move graphics
    mPosYf += speed;
    int posY = roundf(mPosYf);

    mTxt->SetY(posY);

    const float dist = mPosY0f - mPosYf;

    if(dist > mMaxDistance)
        SetDone();

    // update alpha
    const float minAlpha = 1.f;

    mAlpha -= alphaDecay;
    const int alpha = static_cast<unsigned char>(mAlpha);

    mTxt->SetAlpha(alpha);

    if(mAlpha < minAlpha)
        SetDone();
}

void ParticleHitPoints::Render()
{
    mTxt->Render();
}

} // namespace game
