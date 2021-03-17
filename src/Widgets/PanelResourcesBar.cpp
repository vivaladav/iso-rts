#include "PanelResourcesBar.h"

#include "Player.h"
#include "Widgets/DigitsDisplay.h"
#include "Widgets/ProgressBarResources.h"

#include <graphic/Image.h>
#include <graphic/Texture.h>
#include <graphic/TextureManager.h>

namespace game
{

PanelResourcesBar::PanelResourcesBar(Player * player)
    : mBg(new lib::graphic::Image)
{
    SetBg();

    auto dd = new DigitsDisplay(5, this);
    dd->SetValue(125);
    dd->SetPosition(50, 10);

    auto bar1 = new ProgressBarResources(0.f, 1000.f, this);
    bar1->SetValue(300.f);
    bar1->SetPosition(dd->GetX() + (dd->GetWidth() - bar1->GetWidth()) * 0.5f, dd->GetY() + dd->GetHeight());

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
