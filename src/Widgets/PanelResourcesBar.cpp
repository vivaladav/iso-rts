#include "PanelResourcesBar.h"

#include "Player.h"
#include "Widgets/ResourceDisplay.h"

#include <graphic/Image.h>
#include <graphic/Texture.h>
#include <graphic/TextureManager.h>

#include <sgui/Image.h>

namespace game
{

PanelResourcesBar::PanelResourcesBar(Player * player)
    : mBg(new lib::graphic::Image)
{
    SetBg();

    const int x0 = 15;

    auto rd = new ResourceDisplay("data/img/UI/icon_energy.png", this);
    rd->SetValue(100);
    rd->SetPosition(x0 + 50, (GetHeight() - rd->GetHeight()) * 0.5f);

    rd = new ResourceDisplay("data/img/UI/icon_material1.png", this);
    rd->SetValue(325);
    rd->SetPosition(x0 + 200, (GetHeight() - rd->GetHeight()) * 0.5f);

    rd = new ResourceDisplay("data/img/UI/icon_diamond.png", this);
    rd->SetValue(654);
    rd->SetPosition(x0 + 350, (GetHeight() - rd->GetHeight()) * 0.5f);

    rd = new ResourceDisplay("data/img/UI/icon_blob.png", this);
    rd->SetValue(950);
    rd->SetPosition(x0 + 500, (GetHeight() - rd->GetHeight()) * 0.5f);
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
