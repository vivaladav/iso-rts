#include "PanelResourcesBar.h"

#include "Player.h"

#include <graphic/Image.h>
#include <graphic/Texture.h>
#include <graphic/TextureManager.h>

namespace game
{


PanelResourcesBar::PanelResourcesBar(Player * player)
    : mBg(new lib::graphic::Image)
{
    SetBg();
}

PanelResourcesBar::~PanelResourcesBar()
{
    delete mBg;
}

void PanelResourcesBar::OnRender()
{
    mBg->Render();
}

void PanelResourcesBar::SetBg()
{
    // NOTE in the future this will chose the right bg based on the number of slots
    auto * tm = lib::graphic::TextureManager::Instance();

    lib::graphic::Texture * tex = tm->GetTexture("data/img/UI/game_top_bar.png");

    mBg->SetTexture(tex);

    SetSize(tex->GetWidth(), tex->GetHeight());
}

void PanelResourcesBar::HandlePositionChanged()
{
    mBg->SetPosition(GetScreenX(), GetScreenY());
}

} // namespace game
