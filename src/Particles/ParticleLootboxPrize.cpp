#include "Particles/ParticleLootboxPrize.h"

#include "GameData.h"
#include "GameObjects/LootBox.h"
#include "Particles/DataParticleLootboxPrize.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/TexturedRenderable.h>
#include <sgl/graphic/TextureManager.h>

#include <cmath>
#include <sstream>

namespace game
{

ParticleLootboxPrize::ParticleLootboxPrize()
    : mIcon(new sgl::graphic::TexturedRenderable)
{
}

void ParticleLootboxPrize::SetData(const DataParticleLootboxPrize & data)
{
    using namespace sgl;

    // init data
    mSpeed = -data.speed;
    mDecaySpeed = data.decaySpeed;

    const float maxAlpha = 255.f;
    mAlpha = maxAlpha;

    // clear visuals
    delete mTxt;

    // type dependent data
    unsigned int iconTexId = 0;
    unsigned int color = 0;

    switch(data.type)
    {
        case LootBox::LB_BLOBS:
            iconTexId = ID_PART_ICON_BLOBS;
            color = 0xe481e4ff;
        break;

        case LootBox::LB_DIAMONDS:
            iconTexId = ID_PART_ICON_DIAMONDS;
            color = 0xe4e481ff;
        break;

        case LootBox::LB_ENERGY:
            iconTexId = ID_PART_ICON_ENERGY;
            color = 0xe4cb81ff;
        break;

        case LootBox::LB_MATERIAL:
            iconTexId = ID_PART_ICON_MATERIAL;
            color = 0xa3c2bfff;
        break;

        case LootBox::LB_MONEY:
            iconTexId = ID_PART_ICON_MONEY;
            color = 0x81e481ff;
        break;

        default: break;
    }

    // ICON
    auto tex = sgl::graphic::TextureManager::Instance()->GetSprite(SpriteFileParticles, iconTexId);
    mIcon->SetTexture(tex);
    mIcon->SetColor(color);

    // TEXT
    const int fontSize = 20;
    auto fm = graphic::FontManager::Instance();
    graphic::Font * font = fm->GetFont("Lato-Bold.ttf", fontSize, graphic::Font::NORMAL);

    std::ostringstream ss;
    ss << "+" << data.quantity;
    mTxt = new graphic::Text(ss.str().c_str(), font);
    mTxt->SetColor(color);

    SetStart(data.x0, data.y0);
}

void ParticleLootboxPrize::SetStart(int x0, int y0)
{
    mPosXf = x0;
    mPosYf = y0;

    const int margin = 5;
    const int totW = mIcon->GetWidth() + margin + mTxt->GetWidth();
    const int halfW = totW / 2;

    // text
    const int txtX = x0 - halfW;
    const int txtY = y0;
    mTxt->SetPosition(txtX, txtY);

    // icon
    const int iconX = txtX + mTxt->GetWidth() + margin;
    const int iconY = y0;
    mIcon->SetPosition(iconX, iconY);

}

void ParticleLootboxPrize::Update(float delta)
{
    const float speed = mSpeed * delta;
    const float alphaDecay = mDecaySpeed * delta;

    // move graphics
    mPosYf += speed;
    int posY = roundf(mPosYf);

    mIcon->SetY(posY);
    mTxt->SetY(posY);

    // update alpha
    const int minAlpha = 1.f;

    mAlpha -= alphaDecay;
    const int alpha = static_cast<unsigned char>(mAlpha);

    mIcon->SetAlpha(alpha);
    mTxt->SetAlpha(alpha);

    // DONE!
    if(mAlpha < minAlpha)
         SetDone();
}

void ParticleLootboxPrize::Render()
{
    mIcon->Render();
    mTxt->Render();
}

} // namespace game
