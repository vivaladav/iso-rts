#include "Widgets/ButtonMainMenuSocial.h"

#include "Widgets/GameUIData.h"

#include <graphic/Camera.h>
#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/TextureManager.h>
#include <graphic/Text.h>

namespace game
{

ButtonMainMenuSocial::ButtonMainMenuSocial(unsigned int icon, lib::sgui::Widget * parent)
    : lib::sgui::PushButton(parent)
    , mLabelsColor
    {
        0xd6ebf5ff,
        0xFFFFFFFF,
        0x9eecfaff,
        0x8fdfefff,
        0xFFFFFFFF
    }
    , mBody(new lib::graphic::Image)
{
    using namespace lib::graphic;

    auto tm = TextureManager::Instance();

    RegisterRenderable(mBody);

    // ICON
    Texture * tex = tm->GetSprite(SpriteFileMainMenu, icon);
    mIcon = new Image(tex);
    RegisterRenderable(mIcon);

    // init to normal state
    SetState(NORMAL);

    PositionElements();
}

void ButtonMainMenuSocial::HandlePositionChanged()
{
     lib::sgui::PushButton::HandlePositionChanged();

     PositionElements();
}

void ButtonMainMenuSocial::OnStateChanged(lib::sgui::PushButton::VisualState state)
{
    const unsigned int texIds[NUM_VISUAL_STATES] =
    {
        IND_MM_BTNS_NORMAL,
        IND_MM_BTNS_NORMAL,     // button can't be disabled
        IND_MM_BTNS_OVER,
        IND_MM_BTNS_PUSHED,
        IND_MM_BTNS_NORMAL,     // button can't be checked
    };

    auto tm = lib::graphic::TextureManager::Instance();
    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileMainMenu, texIds[state]);
    mBody->SetTexture(tex);
    // reset BG to make changes visible
    SetCurrBg(mBody);

    mIcon->SetColor(mLabelsColor[state]);
}

void ButtonMainMenuSocial::PositionElements()
{
    const int x = GetScreenX();
    const int y = GetScreenY();
    const int w = GetWidth();
    const int h = GetHeight();

    // center icon
    const int iconX = x + (w - mIcon->GetWidth()) * 0.5f;
    const int iconY = y + (h - mIcon->GetHeight()) * 0.5f;
    mIcon->SetPosition(iconX, iconY);
}

} // namespace game

