#include "CellProgressBar.h"

#include <graphic/Image.h>

namespace game
{

CellProgressBar::CellProgressBar(lib::sgui::Widget * parent)
    : lib::sgui::ProgressBar(0, 100, parent)
{
    using namespace  lib::graphic;

    mBg = new Image("data/img/cell_bar_bg.png");

    SetSize(mBg->GetWidth(), mBg->GetHeight());

    mBar = new Image("data/img/cell_bar.png");
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
