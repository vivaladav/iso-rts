#include "CellProgressBar.h"

#include <graphic/Image.h>

namespace game
{

CellProgressBar::CellProgressBar(int player, lib::sgui::Widget * parent)
    : lib::sgui::ProgressBar(0, 100, parent)
{
    using namespace  lib::graphic;

    mBg = new Image("data/img/cell_bar_bg.png");

    SetSize(mBg->GetWidth(), mBg->GetHeight());

    if(0 == player)
        mBar = new Image("data/img/cell_bar-p1.png");
    else if(1 == player)
        mBar = new Image("data/img/cell_bar-p2.png");
    else if(2 == player)
        mBar = new Image("data/img/cell_bar-p3.png");
    else if(3 == player)
        mBar = new Image("data/img/cell_bar-p4.png");

    mBarW = mBar->GetWidth();
    mBarH = mBar->GetHeight();
}

void CellProgressBar::HandlePositionChanged()
{
    const int x = GetScreenX();
    const int y = GetScreenY();
    const int border = 2;

    mBg->SetPosition(x, y);
    mBar->SetPosition(x + border, y + border);
}

void CellProgressBar::HandleProgressUpdate()
{
    const float perc = GetValuePerc();

    mBar->SetWidth(static_cast<int>(mBarW * perc / 100.f));
}

void CellProgressBar::OnRender()
{
    mBg->Render();
    mBar->Render();}

} // namespace game
