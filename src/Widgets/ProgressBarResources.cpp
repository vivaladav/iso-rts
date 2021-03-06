#include "ProgressBarResources.h"

#include "Player.h"

#include <graphic/Camera.h>
#include <graphic/Image.h>

namespace game
{

ProgressBarResources::ProgressBarResources(float min, float max, lib::sgui::Widget * parent)
    : lib::sgui::ProgressBar(min, max, parent)
{
    using namespace  lib::graphic;

    // background
    mBg = new Image("data/img/UI/progress_bar-resources-bg.png");
    RegisterRenderable(mBg);

    SetSize(mBg->GetWidth(), mBg->GetHeight());

    // bar
    mBar = new Image("data/img/UI/progress_bar-resources-bar.png");
    RegisterRenderable(mBar);
    mBarW = mBar->GetWidth();
    mBarH = mBar->GetHeight();

    SetBarColor();
}

ProgressBarResources::~ProgressBarResources()
{
    delete mBg;
    delete mBar;
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

void ProgressBarResources::OnRender()
{
    mBg->Render();
    mBar->Render();
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
