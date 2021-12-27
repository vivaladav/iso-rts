#include "Widgets/DialogNewUnit.h"

/*
#include "Player.h"
#include "GameObjects/Unit.h"
#include "GameObjects/UnitData.h"
#include "Widgets/ButtonCloseDialog.h"
#include "Widgets/GameUIData.h"
#include "Widgets/SlotDialogNewUnit.h"

#include <core/event/KeyboardEvent.h>
#include <graphic/Camera.h>
#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/TextureManager.h>
#include <sgui/Image.h>
#include <sgui/Label.h>

namespace game
{

DialogNewUnit::DialogNewUnit(Player * player)
    : mPlayer(player)
{
    using namespace lib::sgui;

    // BACKGROUND
    auto tm = lib::graphic::TextureManager::Instance();
    lib::graphic::Texture * tex = nullptr;

    // left bg
    tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_DIALOG_BG_LEFT);
    mBgLeft = new Image(tex, this);

    CreateHeadersPanel();

    // right bg
    tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_DIALOG_BG_RIGHT);
    mBgRight = new Image(tex, this);

    // CLOSE BUTTON
    const int buttonSide = 20;

    tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_ICON_CLOSE);
    mButtonClose = new ButtonCloseDialog(tex, buttonSide, buttonSide, this);
}

DialogNewUnit::~DialogNewUnit()
{
    delete mBgLeft;
    delete mBgRight;
}

void DialogNewUnit::AddUnitSlot(const UnitData & data)
{
    const PlayerFaction faction = mPlayer->GetFaction();
    auto slot = new SlotDialogNewUnit(faction, data, mSlots.size(), this);

    UpdateSlotButton(data, slot);

    mSlots.emplace_back(slot);

    RepositionElements();

    MoveChildToFront(mButtonClose);

    // handle build button clicked
    slot->SetOnBuildFunction([this, slot]
    {
        mTypeToBuild = slot->GetUnitType();

        mOnBuild();
    });
}

void DialogNewUnit::SetFunctionOnBuild(const std::function<void()> & f)
{
    mOnBuild = f;
}

void DialogNewUnit::SetFunctionOnClose(const std::function<void()> & f)
{
    mButtonClose->AddOnClickFunction(f);
}

void DialogNewUnit::UpdateSlots()
{
    const std::vector<UnitData> & unitsData = mPlayer->GetAvailableUnits();

    for(unsigned int i = 0; i < mSlots.size(); ++i)
        UpdateSlotButton(unitsData[i], mSlots[i]);
}

void DialogNewUnit::RepositionElements()
{
    // left BG
    mBgLeft->SetPosition(0, 0);

    int elemX = mBgLeft->GetWidth();

    // slots
    for(auto slot : mSlots)
    {
        slot->SetPosition(elemX, 0);

        elemX += slot->GetWidth();
    }

    // right BG
    mBgRight->SetPosition(elemX, 0);

    // close button
    elemX = GetWidth() - mButtonClose->GetWidth();
    mButtonClose->SetPosition(elemX, 0);
}

void DialogNewUnit::CreateHeadersPanel()
{
    using namespace lib::sgui;

    auto fm = lib::graphic::FontManager::Instance();
    auto tm = lib::graphic::TextureManager::Instance();

    // label UNITS
    auto font = fm->GetFont("data/fonts/Lato-Bold.ttf", 32, lib::graphic::Font::NORMAL);
    auto label = new Label("UNITS", font, mBgLeft);
    label->SetColor(0xabb4baff);
    label->SetPosition(50, 58);

    // panel BG
    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_PANEL_HEADERS);
    auto panel = new Image(tex, mBgLeft);
    panel->SetPosition(20, 130);

    // HEADERS
    auto fontHeaders = fm->GetFont("data/fonts/Lato-Bold.ttf", 16, lib::graphic::Font::NORMAL);

    const unsigned int numHeaders = 6;
    const unsigned int colorHeader = 0xf1f2f4ff;

    const char * strings[numHeaders] =
    {
        "ENERGY",
        "RESISTANCE",
        "ATTACK",
        "SPEED",
        "CONSTRUCTION",
        "CONQUEST"
    };

    const int headerX = 10;
    const int marginV = 8;
    int headerY = 10;

    for(unsigned int i = 0; i < numHeaders; ++i)
    {
        label = new Label(strings[i], fontHeaders, panel);
        label->SetColor(colorHeader);
        label->SetPosition(headerX, headerY);

        headerY += marginV + label->GetHeight();
    }
}

void DialogNewUnit::UpdateSlotButton(const game::UnitData & data, SlotDialogNewUnit * slot)
{
    const bool ok = data.costEnergy <= mPlayer->GetStat(Player::Stat::ENERGY).GetIntValue() &&
                    data.costMaterial <= mPlayer->GetStat(Player::Stat::MATERIAL).GetIntValue();

    slot->SetButtonEnabled(ok);
}

void DialogNewUnit::HandleKeyUp(lib::core::KeyboardEvent & event)
{
    // ESC -> close dialog
    if(event.GetKey() == lib::core::KeyboardEvent::KEY_ESC)
    {
        mButtonClose->Click();

        event.SetConsumed();
    }
}

} // namespace game

*/
