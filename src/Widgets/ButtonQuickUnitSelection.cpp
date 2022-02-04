#include "ButtonQuickUnitSelection.h"

#include "GameUIData.h"
#include "Player.h"
#include "GameObjects/Unit.h"
#include "Screens/ScreenGame.h"

#include <graphic/Image.h>
#include <graphic/Texture.h>
#include <graphic/TextureManager.h>

namespace game
{

ButtonQuickUnitSelection::ButtonQuickUnitSelection(ScreenGame * sg, lib::sgui::Widget * parent)
    : lib::sgui::ImageButton({ SpriteIdUnitQuickSel::IND_UQS_BG_NORMAL,
                               SpriteIdUnitQuickSel::IND_UQS_BG_DISABLED,
                               SpriteIdUnitQuickSel::IND_UQS_BG_MOUSE_OVER,
                               SpriteIdUnitQuickSel::IND_UQS_BG_PUSHED,
                               SpriteIdUnitQuickSel::IND_UQS_BG_CHECKED },
                             SpriteFileUnitQuickSel, parent)
    , mScreenGame(sg)
{
    SetEnabled(false);


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

} // namespace game
