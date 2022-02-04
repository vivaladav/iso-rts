#include "ButtonQuickUnitSelection.h"

#include "GameUIData.h"
#include "Player.h"
#include "GameObjects/Unit.h"
#include "Screens/ScreenGame.h"

#include <core/event/KeyboardEvent.h>
#include <graphic/Image.h>
#include <graphic/Texture.h>
#include <graphic/TextureManager.h>

namespace game
{

const std::array<unsigned int, lib::sgui::PushButton::NUM_VISUAL_STATES> TEX_IDS =
{
    SpriteIdUnitQuickSel::IND_UQS_BG_NORMAL,
    SpriteIdUnitQuickSel::IND_UQS_BG_DISABLED,
    SpriteIdUnitQuickSel::IND_UQS_BG_MOUSE_OVER,
    SpriteIdUnitQuickSel::IND_UQS_BG_PUSHED,
    SpriteIdUnitQuickSel::IND_UQS_BG_CHECKED
};

const int MAX_BUTTONS = 10;

const int KEYS[MAX_BUTTONS] =
{
    lib::core::KeyboardEvent::KEY_1,
    lib::core::KeyboardEvent::KEY_2,
    lib::core::KeyboardEvent::KEY_3,
    lib::core::KeyboardEvent::KEY_4,
    lib::core::KeyboardEvent::KEY_5,
    lib::core::KeyboardEvent::KEY_6,
    lib::core::KeyboardEvent::KEY_7,
    lib::core::KeyboardEvent::KEY_8,
    lib::core::KeyboardEvent::KEY_9,
    lib::core::KeyboardEvent::KEY_0
};

const char * KEYS_STR[MAX_BUTTONS] =
{
    "1", "2", "3", "4", "5",
    "6", "7", "8", "9", "0"
};

ButtonQuickUnitSelection::ButtonQuickUnitSelection(int index, ScreenGame * sg, lib::sgui::Widget * parent)
    : lib::sgui::ImageButton(TEX_IDS, SpriteFileUnitQuickSel, parent)
    , mScreenGame(sg)
    , mShortcutKey(KEYS[index])
{
    // button disabled by default as it has no unit assigned yet
    SetEnabled(false);

    // handle button checked
    AddOnToggleFunction([this](bool checked)
    {
        if(!checked)
            return;

        // nothing else to do if unit already selected
        if(mUnit->IsSelected())
            return;

        Player * p = mUnit->GetOwner();

        mScreenGame->ClearSelection(p);
        mScreenGame->SelectObject(mUnit, p);
        mScreenGame->CenterCameraOverObject(mUnit);
    });
}

void ButtonQuickUnitSelection::ClearUnit()
{
    // already no unit
    if(!IsEnabled())
        return ;

    UnregisterRenderable(mIcon);
    delete mIcon;
    mIcon = nullptr;

    // disable button
    SetEnabled(false);
}

void ButtonQuickUnitSelection::SetUnit(Unit * unit)
{
    // already set
    if(unit == mUnit)
        return ;

    auto * tm = lib::graphic::TextureManager::Instance();

    // set unit
    mUnit = unit;

    // update icon
    if(nullptr == mIcon)
    {
        mIcon = new lib::graphic::Image;
        RegisterRenderable(mIcon);
    }

    const Player * owner = unit->GetOwner();
    const unsigned int faction = owner->GetFaction();
    const UnitType type = unit->GetUnitType();

    const unsigned int texInd = IND_UQS_UNIT1_F1 + (NUM_UQS_UNIT_SPRITE_PER_FACTION * faction) + type;
    lib::graphic::Texture * tex =tm->GetSprite(SpriteFileUnitQuickSel, texInd);

    mIcon->SetTexture(tex);

    // make sure button is enabled
    SetEnabled(true);

    // position stuff
    HandlePositionChanged();
}

void ButtonQuickUnitSelection::HandlePositionChanged()
{
    lib::sgui::ImageButton::HandlePositionChanged();

    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    // icon
    if(mIcon != nullptr)
    {
        const int marginIconL = 25;
        const int iconX = x0 + marginIconL;
        const int iconY = y0 + (GetHeight() - mIcon->GetHeight()) * 0.5f;
        mIcon->SetPosition(iconX, iconY);
    }
}

void ButtonQuickUnitSelection::HandleKeyDown(lib::core::KeyboardEvent & event)
{
    if(event.GetKey() == mShortcutKey)
    {
        HandleButtonDown();

        event.SetConsumed();
    }
}

void ButtonQuickUnitSelection::HandleKeyUp(lib::core::KeyboardEvent & event)
{
    if(event.GetKey() == mShortcutKey)
    {
        HandleButtonUp();

        event.SetConsumed();
    }
}
} // namespace game
