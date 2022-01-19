#include "CellProgressBar.h"

#include "GameConstants.h"
#include "GameData.h"

#include <graphic/Camera.h>
#include <graphic/Image.h>
#include <graphic/TextureManager.h>

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

    auto tm = TextureManager::Instance();

    // background
    Texture * tex = tm->GetSprite(SpriteFileCellBar, IND_CELL_BAR_BG);
    mBg = new Image(tex);
    RegisterRenderable(mBg);

    SetSize(mBg->GetWidth(), mBg->GetHeight());

    // bar
    const unsigned int texId = IND_CELL_BAR_F1 + faction;
    tex = tm->GetSprite(SpriteFileCellBar, texId);
    mBar = new Image(tex);

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
