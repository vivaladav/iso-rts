#include "Widgets/ButtonQuickUnitSelection.h"

#include "GameUIData.h"
#include "Player.h"
#include "GameObjects/Unit.h"
#include "Screens/ScreenGame.h"
#include "Widgets/DigitsDisplay.h"

#include <core/event/KeyboardEvent.h>
#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/Text.h>
#include <graphic/Texture.h>
#include <graphic/TextureManager.h>

#include <cmath>

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
    // create shortcut label
    auto fm = lib::graphic::FontManager::Instance();
    auto fontShortcut = fm->GetFont("data/fonts/Lato-Regular.ttf", 12, lib::graphic::Font::NORMAL);
    mShortcut = new lib::graphic::Text(KEYS_STR[index], fontShortcut);
    RegisterRenderable(mShortcut);

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

    // button disabled by default as it has no unit assigned yet
    SetEnabled(false);
}

void ButtonQuickUnitSelection::ClearUnit()
{
    // already no unit
    if(nullptr == mUnit)
        return ;

    mUnit = nullptr;

    UnregisterRenderable(mImgUnit);
    delete mImgUnit;
    mImgUnit = nullptr;

    UnregisterRenderable(mIconEnergy);
    delete mIconEnergy;
    mIconEnergy = nullptr;

    delete mDisplayEnergy;
    mDisplayEnergy = nullptr;

    UnregisterRenderable(mIconHealth);
    delete mIconHealth;
    mIconHealth = nullptr;

    delete  mDisplayHealth;
    mDisplayHealth = nullptr;

    // disable button
    SetEnabled(false);
}

void ButtonQuickUnitSelection::SetUnit(Unit * unit)
{
    // already set same unit
    if(unit == mUnit)
        return ;

    auto * tm = lib::graphic::TextureManager::Instance();

    // set unit
    mUnit = unit;

    // update icon
    if(nullptr == mImgUnit)
    {
        mImgUnit = new lib::graphic::Image;
        RegisterRenderable(mImgUnit);
    }

    const Player * owner = unit->GetOwner();
    const unsigned int faction = owner->GetFaction();
    const UnitType type = unit->GetUnitType();

    const unsigned int texInd = IND_UQS_UNIT1_F1 + (NUM_UQS_UNIT_SPRITE_PER_FACTION * faction) + type;
    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileUnitQuickSel, texInd);

    mImgUnit->SetTexture(tex);

    // energy
    const int digits = 3;
    const int digitsFontSize = 16;
    const std::string suffix("%");

    if(nullptr == mIconEnergy)
    {
        tex = tm->GetSprite(SpriteFileUnitQuickSel, IND_UQS_ICON_ENERGY);
        mIconEnergy = new lib::graphic::Image(tex);
        RegisterRenderable(mIconEnergy);

        mDisplayEnergy = new DigitsDisplay(digits, digitsFontSize, suffix, this);
    }

    // health
    if(nullptr == mIconHealth)
    {
        tex = tm->GetSprite(SpriteFileUnitQuickSel, IND_UQS_ICON_HEALTH);
        mIconHealth = new lib::graphic::Image(tex);
        RegisterRenderable(mIconHealth);

        mDisplayHealth = new DigitsDisplay(digits, digitsFontSize, suffix, this);
        mDisplayHealth->SetSuffix(suffix);
    }

    // set initial values
    UpdateValues();

    // track values changing
    mUnit->SetOnValuesChanged([this] { UpdateValues(); });

    // make sure button is enabled
    SetEnabled(true);

    // position stuff
    HandlePositionChanged();
}

void ButtonQuickUnitSelection::OnStateChanged(lib::sgui::PushButton::VisualState state)
{
    lib::sgui::ImageButton::OnStateChanged(state);

    const unsigned int colorsShortcut[NUM_VISUAL_STATES] =
    {
        0xc7cdd1ff,
        0x5c6870ff,
        0xe3e6e8ff,
        0xabb4baff,
        0xe5e5b3ff
    };

    mShortcut->SetColor(colorsShortcut[state]);
}

void ButtonQuickUnitSelection::HandlePositionChanged()
{
    lib::sgui::ImageButton::HandlePositionChanged();

    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    // shortcut label
    const int marginShortcutH = 5;
    const int marginShortcutV = 3;
    const int shortcutX = x0 + marginShortcutH;
    const int shortcutY = y0 + marginShortcutV;
    mShortcut->SetPosition(shortcutX, shortcutY);

    // no unit -> no graphics to position
    if(nullptr == mUnit)
        return ;

    // unit image
    const int marginIconL = 25;
    const int imgX = x0 + marginIconL;
    const int imgY = y0 + (GetHeight() - mImgUnit->GetHeight()) * 0.5f;
    mImgUnit->SetPosition(imgX, imgY);

    // icons and displays
    const int iconsCenterY = 22;
    const int displayMarginB = 8;

    // icon energy
    const int iconEnCenterX = 94;
    const int iconEnX = x0 + iconEnCenterX - mIconEnergy->GetWidth() * 0.5f;
    const int iconEnY = y0 + iconsCenterY - mIconEnergy->GetHeight() * 0.5f;
    mIconEnergy->SetPosition(iconEnX, iconEnY);

    // digits energy
    const int digitsEnX = iconEnCenterX - mDisplayEnergy->GetWidth() * 0.5f;
    const int digitsEnY = GetHeight() - displayMarginB - mDisplayEnergy->GetHeight();
    mDisplayEnergy->SetPosition(digitsEnX, digitsEnY);

    // icon health
    const int iconHeCenterX = 152;
    const int iconHeX = x0 + iconHeCenterX - mIconHealth->GetWidth() * 0.5f;
    const int iconHeY = y0 + iconsCenterY - mIconHealth->GetHeight() * 0.5f;
    mIconHealth->SetPosition(iconHeX, iconHeY);

    // digits health
    const int digitsHeX = iconHeCenterX - mDisplayHealth->GetWidth() * 0.5f;
    const int digitsHeY = GetHeight() - displayMarginB - mDisplayHealth->GetHeight();
    mDisplayHealth->SetPosition(digitsHeX, digitsHeY);
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

void ButtonQuickUnitSelection::UpdateValues()
{
    // energy
    const int energy = std::roundf(mUnit->GetEnergy() * 100.f / mUnit->GetMaxEnergy());
    mDisplayEnergy->SetValue(energy);

    // health
    const int health = std::roundf(mUnit->GetHealth() * 100.f / mUnit->GetMaxHealth());
    mDisplayHealth->SetValue(health);
}

} // namespace game
