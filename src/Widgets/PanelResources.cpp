#include "PanelResources.h"

#include "Player.h"
#include "Widgets/ResourceDisplay.h"

#include <graphic/Image.h>
#include <graphic/Texture.h>
#include <graphic/TextureManager.h>

#include <sgui/Image.h>

namespace game
{

PanelResources::PanelResources(Player * player)
    : mBg(new lib::graphic::Image)
{
    SetBg();

    // TODO change this to have dynamic slots added/removed at runtime
    const int slotW = 150;
    const int slots = 4;
    const int slotsW = slotW * slots;
    const int x0 = (GetWidth() - slotsW) * 0.5f;

    int slotX = x0;

    auto rd = new ResourceDisplay("data/img/UI/icon_energy.png", this);
    rd->SetValue(100);
    rd->SetPosition(slotX + (slotW - rd->GetWidth()) * 0.5f, (GetHeight() - rd->GetHeight()) * 0.5f);

    slotX += slotW;

    rd = new ResourceDisplay("data/img/UI/icon_material1.png", this);
    rd->SetValue(325);
    rd->SetPosition(slotX + (slotW - rd->GetWidth()) * 0.5f, (GetHeight() - rd->GetHeight()) * 0.5f);

    slotX += slotW;

    rd = new ResourceDisplay("data/img/UI/icon_diamond.png", this);
    rd->SetValue(654);
    rd->SetPosition(slotX + (slotW - rd->GetWidth()) * 0.5f, (GetHeight() - rd->GetHeight()) * 0.5f);

    slotX += slotW;

    rd = new ResourceDisplay("data/img/UI/icon_blob.png", this);
    rd->SetValue(950);
    rd->SetPosition(slotX + (slotW - rd->GetWidth()) * 0.5f, (GetHeight() - rd->GetHeight()) * 0.5f);
}

PanelResources::~PanelResources()
{
    delete mBg;
}

void PanelResources::OnRender()
{
    mBg->Render();
}

void PanelResources::SetBg()
{
    // NOTE in the future this will chose the right bg based on the number of slots
    auto * tm = lib::graphic::TextureManager::Instance();

    lib::graphic::Texture * tex = tm->GetTexture("data/img/UI/game_top_bar.png");

    mBg->SetTexture(tex);

    SetSize(tex->GetWidth(), tex->GetHeight());
}

void PanelResources::HandlePositionChanged()
{
    mBg->SetPosition(GetScreenX(), GetScreenY());
}

} // namespace game
