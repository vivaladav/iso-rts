#include "Widgets/ButtonMainMenuWishlist.h"

#include "GameData.h"

#include <graphic/Camera.h>
#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/TextureManager.h>
#include <graphic/Text.h>

namespace game
{

ButtonMainMenuWishlist::ButtonMainMenuWishlist(lib::sgui::Widget * parent)
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
    tex = tm->GetSprite(SpriteFileMainMenu, IND_MM_BTNW_NORMAL);
    mBackgrounds[NORMAL] = new Image(tex);
    RegisterRenderable(mBackgrounds[NORMAL]);

    // mouse over
    tex = tm->GetSprite(SpriteFileMainMenu, IND_MM_BTNW_OVER);
    mBackgrounds[MOUSE_OVER] = new Image(tex);
    RegisterRenderable(mBackgrounds[MOUSE_OVER]);

    // pushed
    tex = tm->GetSprite(SpriteFileMainMenu, IND_MM_BTNW_PUSHED);
    mBackgrounds[PUSHED] = new Image(tex);
    RegisterRenderable(mBackgrounds[PUSHED]);

    // not handled cases
    mBackgrounds[DISABLED] = nullptr;
    mBackgrounds[CHECKED] = nullptr;

    // ICON
    tex = tm->GetSprite(SpriteFileMainMenu, IND_MM_ICON_STEAM);
    mIcon = new Image(tex);
    RegisterRenderable(mIcon);

    // TEXT LABEL
    // TODO use setLabel after adding support for icon to PushButton
    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/Lato-Regular.ttf", 18, Font::NORMAL);
    mText = new Text("WISHLIST NOW", font, true);
    RegisterRenderable(mText);

    // init to normal state
    SetElements(NORMAL);

    PositionElements();
}

ButtonMainMenuWishlist::~ButtonMainMenuWishlist()
{
    for(unsigned int i = 0; i < NUM_VISUAL_STATES; ++i)
        delete mBackgrounds[i];
}

void ButtonMainMenuWishlist::HandlePositionChanged()
{
     lib::sgui::PushButton::HandlePositionChanged();

     PositionElements();
}

void ButtonMainMenuWishlist::HandleMouseButtonDown(lib::core::MouseButtonEvent & event)
{
    PushButton::HandleMouseButtonDown(event);

    SetElements(PUSHED);
}

void ButtonMainMenuWishlist::HandleMouseButtonUp(lib::core::MouseButtonEvent & event)
{
    PushButton::HandleMouseButtonUp(event);

    SetElements(NORMAL);
}

void ButtonMainMenuWishlist::HandleMouseOver()
{
    SetElements(MOUSE_OVER);
}

void ButtonMainMenuWishlist::HandleMouseOut()
{
    SetElements(NORMAL);
}

void ButtonMainMenuWishlist::OnRender()
{
    lib::sgui::PushButton::OnRender();

    mIcon->Render();
    mText->Render();
}

void ButtonMainMenuWishlist::SetElements(int index)
{
    if(mState == index)
        return ;

    mState = index;

    SetCurrBg(mBackgrounds[index]);

    mIcon->SetColor(mLabelsColor[index]);
    mText->SetColor(mLabelsColor[index]);
}

void ButtonMainMenuWishlist::PositionElements()
{
    const int x = GetScreenX();
    const int y = GetScreenY();
    const int w = GetWidth();
    const int h = GetHeight();

    const int marginH = 10;
    const int totW = mIcon->GetWidth() + marginH + mText->GetWidth();
    const int x0 = x + (w - totW) * 0.5f;

    // icon
    const int iconX = x0;
    const int iconY = y + (h - mIcon->GetHeight()) * 0.5f;
    mIcon->SetPosition(iconX, iconY);

    // text
    const int textX = iconX + mIcon->GetWidth() + marginH;
    const int textY = y + (h - mText->GetHeight()) * 0.5f;
    mText->SetPosition(textX, textY);
}

} // namespace game

