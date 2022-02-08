#include "Widgets/ButtonMainMenuWishlist.h"

#include "Widgets/GameUIData.h"

#include <sgl/graphic/Camera.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/graphic/Text.h>

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
    , mBody(new lib::graphic::Image)
{
    using namespace lib::graphic;

    auto tm = TextureManager::Instance();

    RegisterRenderable(mBody);

    // ICON
    Texture * tex = tm->GetSprite(SpriteFileMainMenu, IND_MM_ICON_STEAM);
    mIcon = new Image(tex);
    RegisterRenderable(mIcon);

    // TEXT LABEL
    // TODO use setLabel after adding support for icon to PushButton
    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/Lato-Regular.ttf", 18, Font::NORMAL);
    mText = new Text("WISHLIST NOW", font, true);
    RegisterRenderable(mText);

    // init to normal state
    SetState(NORMAL);

    PositionElements();
}

void ButtonMainMenuWishlist::HandlePositionChanged()
{
     lib::sgui::PushButton::HandlePositionChanged();

     PositionElements();
}

void ButtonMainMenuWishlist::OnStateChanged(lib::sgui::PushButton::VisualState state)
{
    const unsigned int texIds[NUM_VISUAL_STATES] =
    {
        IND_MM_BTNW_NORMAL,
        IND_MM_BTNW_NORMAL,     // button can't be disabled
        IND_MM_BTNW_OVER,
        IND_MM_BTNW_PUSHED,
        IND_MM_BTNW_NORMAL,     // button can't be checked
    };

    auto tm = lib::graphic::TextureManager::Instance();
    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileMainMenu, texIds[state]);
    mBody->SetTexture(tex);
    // reset BG to make changes visible
    SetCurrBg(mBody);

    mIcon->SetColor(mLabelsColor[state]);
    mText->SetColor(mLabelsColor[state]);
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

