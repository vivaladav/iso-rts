#include "ProgressBarResources.h"

#include "GameUIData.h"

#include <sgl/graphic/Image.h>
#include <sgl/graphic/TextureManager.h>

namespace game
{

ProgressBarResources::ProgressBarResources(float min, float max, sgl::sgui::Widget * parent)
    : sgl::sgui::ProgressBar(min, max, parent)
{
    using namespace  sgl::graphic;

    auto * tm = TextureManager::Instance();

    // background
    mBg = new Image(tm->GetSprite(SpriteFileResourcesBar, IND_RESBAR_MINIBAR_BG));
    RegisterRenderable(mBg);

    SetSize(mBg->GetWidth(), mBg->GetHeight());

    // bar
    mBar = new Image(tm->GetSprite(SpriteFileResourcesBar, IND_RESBAR_MINIBAR_VAL));
    RegisterRenderable(mBar);
    mBarW = mBar->GetWidth();
    mBarH = mBar->GetHeight();

    SetBarColor();
}

void ProgressBarResources::HandlePositionChanged()
{
    const int x = GetScreenX();
    const int y = GetScreenY();
    const int border = 2;

    mBg->SetPosition(x, y);
    mBar->SetPosition(x + border, y + border);
}

void ProgressBarResources::HandleProgressUpdate()
{
    const float perc = GetValuePerc();

    mBar->SetWidth(static_cast<int>(mBarW * perc / 100.f));

    SetBarColor();
}

void ProgressBarResources::SetBarColor()
{
    const float perc = GetValuePerc();

    const int numColors = 5;

    const unsigned int colors[numColors] =
    {
        0xb2554dff,
        0xb2774dff,
        0xb2b24dff,
        0x80b24dff,
        0x4db253ff
    };

    const float mult = static_cast<float>(numColors) / 100.f;

    if(perc < 100.f)
    {
        const int colorInd = static_cast<int>(perc * mult);
        mBar->SetColor(colors[colorInd]);
    }
    else
        mBar->SetColor(colors[numColors - 1]);
}

} // namespace game
