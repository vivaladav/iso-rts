#include "CellProgressBar.h"

#include "Player.h"

#include <graphic/Camera.h>
#include <graphic/Image.h>

namespace game
{

CellProgressBar::CellProgressBar(PlayerFaction faction, lib::sgui::Widget * parent)
    : CellProgressBar(faction, 0, 100.f, parent)
{
    SetCamera(lib::graphic::Camera::GetDefaultCamera());

    HandleProgressUpdate();
}

CellProgressBar::CellProgressBar(PlayerFaction faction, float min, float max, lib::sgui::Widget * parent)
    : lib::sgui::ProgressBar(min, max, parent)
{
    using namespace lib::graphic;

    // CellProgressBar is part of the game scene
    SetCamera(Camera::GetDefaultCamera());

    mBg = new Image("data/img/cell_bar_bg.png");
    RegisterRenderable(mBg);

    SetSize(mBg->GetWidth(), mBg->GetHeight());

    if(FACTION_1 == faction)
        mBar = new Image("data/img/cell_bar-p1.png");
    else if(FACTION_2 == faction)
        mBar = new Image("data/img/cell_bar-p2.png");
    else
        mBar = new Image("data/img/cell_bar-p3.png");

    RegisterRenderable(mBar);

    mBarW = mBar->GetWidth();
    mBarH = mBar->GetHeight();
}

void CellProgressBar::HandlePositionChanged()
{
    const int x = GetScreenX();
    const int y = GetScreenY();
    const int border = 1;

    mBg->SetPosition(x, y);
    mBar->SetPosition(x + border, y + border);
}

void CellProgressBar::HandleProgressUpdate()
{
    const float perc = GetValuePerc();

    mBar->SetWidth(static_cast<int>(mBarW * perc / 100.f));
}

} // namespace game
