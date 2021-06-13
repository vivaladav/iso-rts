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
{
    using namespace lib::graphic;

    auto tm = TextureManager::Instance();

    // BACKGROUNDS
    Texture * tex;

    // normal
    tex = tm->GetSprite(SpriteFileMainMenu, IND_MM_BTNS_NORMAL);
    mBackgrounds[NORMAL] = new Image(tex);
    RegisterRenderable(mBackgrounds[NORMAL]);

    // mouse over
    tex = tm->GetSprite(SpriteFileMainMenu, IND_MM_BTNS_OVER);
    mBackgrounds[MOUSE_OVER] = new Image(tex);
    RegisterRenderable(mBackgrounds[MOUSE_OVER]);

    // pushed
    tex = tm->GetSprite(SpriteFileMainMenu, IND_MM_BTNS_PUSHED);
    mBackgrounds[PUSHED] = new Image(tex);
    RegisterRenderable(mBackgrounds[PUSHED]);

    // not handled cases
    mBackgrounds[DISABLED] = nullptr;
    mBackgrounds[CHECKED] = nullptr;

    // ICON
    tex = tm->GetSprite(SpriteFileMainMenu, icon);
    mIcon = new Image(tex);
    RegisterRenderable(mIcon);

    // init to normal state
    SetElements(NORMAL);

    PositionElements();
}

ButtonMainMenuSocial::~ButtonMainMenuSocial()
{
    for(unsigned int i = 0; i < NUM_VISUAL_STATES; ++i)
        delete mBackgrounds[i];

    delete mIcon;
}

void ButtonMainMenuSocial::HandlePositionChanged()
{
     lib::sgui::PushButton::HandlePositionChanged();

     PositionElements();
}

void ButtonMainMenuSocial::HandleMouseButtonDown(lib::core::MouseButtonEvent & event)
{
    PushButton::HandleMouseButtonDown(event);

    SetElements(PUSHED);
}

void ButtonMainMenuSocial::HandleMouseButtonUp(lib::core::MouseButtonEvent & event)
{
    PushButton::HandleMouseButtonUp(event);

    SetElements(NORMAL);
}

void ButtonMainMenuSocial::HandleMouseOver()
{
    SetElements(MOUSE_OVER);
}

void ButtonMainMenuSocial::HandleMouseOut()
{
    SetElements(NORMAL);
}

void ButtonMainMenuSocial::OnRender()
{
    lib::sgui::PushButton::OnRender();

    mIcon->Render();
}

void ButtonMainMenuSocial::SetElements(int index)
{
    if(mState == index)
        return ;

    mState = index;

    SetCurrBg(mBackgrounds[index]);

    mIcon->SetColor(mLabelsColor[index]);
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

