#include "PanelResources.h"

#include "Player.h"
#include "Widgets/ResourceDisplay.h"
#include "Widgets/SimpleResourceDisplay.h"
#include "Widgets/GameUIData.h"

#include <graphic/Camera.h>
#include <graphic/Image.h>
#include <graphic/Texture.h>
#include <graphic/TextureManager.h>

#include <sgui/Image.h>

namespace game
{

PanelResources::PanelResources(Player * player)
    : mBg(new lib::graphic::Image)
{
    RegisterRenderable(mBg);

    SetBg();

    // TODO change this to have dynamic slots added/removed at runtime
    const int slotW = 150;
    const int slots = 5;
    const int slotsW = slotW * slots;
    const int x0 = (GetWidth() - slotsW) * 0.5f;

    int slotX = x0;

    auto * tm = lib::graphic::TextureManager::Instance();
    lib::graphic::Texture * tex = nullptr;

    const int numDigitsMoney = 8;
    const int numDigits = 5;

    // MONEY
    StatValue & money = player->GetStat(Player::Stat::MONEY);
    tex = tm->GetSprite(SpriteFileResourcesBar, IND_RESBAR_MONEY);
    auto srd = new SimpleResourceDisplay(tex, numDigitsMoney, this);
    srd->SetValue(money.GetIntValue());
    srd->SetPosition(slotX + (slotW - srd->GetWidth()) * 0.5f, (GetHeight() - srd->GetHeight()) * 0.5f);

    money.AddOnValueChanged([srd](const StatValue * val) { srd->SetValue(val->GetIntValue()); });

    slotX += slotW;

    // ENERGY
    StatValue & energy = player->GetStat(Player::Stat::ENERGY);
    tex = tm->GetSprite(SpriteFileResourcesBar, IND_RESBAR_ENERGY);
    auto rd = new ResourceDisplay(tex, numDigits, this);
    rd->SetValueMinMax(energy.GetIntMin(), energy.GetIntMax());
    rd->SetValue(energy.GetIntValue());
    rd->SetPosition(slotX + (slotW - rd->GetWidth()) * 0.5f, (GetHeight() - rd->GetHeight()) * 0.5f);

    energy.AddOnValueChanged([rd](const StatValue * val) { rd->SetValue(val->GetIntValue()); });

    slotX += slotW;

    // MATERIAL
    StatValue & material = player->GetStat(Player::Stat::MATERIAL);
    tex = tm->GetSprite(SpriteFileResourcesBar, IND_RESBAR_MATERIAL);
    rd = new ResourceDisplay(tex, numDigits, this);
    rd->SetValueMinMax(material.GetIntMin(), material.GetIntMax());
    rd->SetValue(material.GetIntValue());
    rd->SetPosition(slotX + (slotW - rd->GetWidth()) * 0.5f, (GetHeight() - rd->GetHeight()) * 0.5f);

    material.AddOnValueChanged([rd](const StatValue * val) { rd->SetValue(val->GetIntValue()); });

    slotX += slotW;

    // DIAMONDS
    StatValue & diamonds = player->GetStat(Player::Stat::DIAMONDS);
    tex = tm->GetSprite(SpriteFileResourcesBar, IND_RESBAR_DIAMOND);
    rd = new ResourceDisplay(tex, numDigits, this);
    rd->SetValueMinMax(diamonds.GetIntMin(), diamonds.GetIntMax());
    rd->SetValue(diamonds.GetIntValue());
    rd->SetPosition(slotX + (slotW - rd->GetWidth()) * 0.5f, (GetHeight() - rd->GetHeight()) * 0.5f);

    diamonds.AddOnValueChanged([rd](const StatValue * val) { rd->SetValue(val->GetIntValue()); });

    slotX += slotW;

    // BLOB
    StatValue & blobs = player->GetStat(Player::Stat::BLOBS);
    tex = tm->GetSprite(SpriteFileResourcesBar, IND_RESBAR_BLOB);
    rd = new ResourceDisplay(tex, numDigits, this);
    rd->SetValueMinMax(blobs.GetIntMin(), blobs.GetIntMax());
    rd->SetValue(blobs.GetIntValue());
    rd->SetPosition(slotX + (slotW - rd->GetWidth()) * 0.5f, (GetHeight() - rd->GetHeight()) * 0.5f);

    blobs.AddOnValueChanged([rd](const StatValue * val) { rd->SetValue(val->GetIntValue()); });
}

PanelResources::~PanelResources()
{
    delete mBg;
}

void PanelResources::SetBg()
{
    // NOTE in the future this will chose the right bg based on the number of slots
    auto * tm = lib::graphic::TextureManager::Instance();

    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileResourcesBar, IND_RESBAR_BG);

    mBg->SetTexture(tex);

    SetSize(tex->GetWidth(), tex->GetHeight());
}

void PanelResources::HandlePositionChanged()
{
    mBg->SetPosition(GetScreenX(), GetScreenY());
}

} // namespace game
