#include "Widgets/ButtonMainMenuWishlist.h"

#include "Widgets/GameUIData.h"

#include <sgl/graphic/Camera.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/graphic/Text.h>
#include <sgl/media/AudioManager.h>
#include <sgl/media/AudioPlayer.h>

namespace game
{

ButtonMainMenuWishlist::ButtonMainMenuWishlist(sgl::sgui::Widget * parent)
    : sgl::sgui::AbstractButton(parent)
    , mLabelsColor
    {
        0xd6ebf5ff,
        0xFFFFFFFF,
        0x9eecfaff,
        0x8fdfefff,
        0xFFFFFFFF
    }
    , mBody(new sgl::graphic::Image)
{
    using namespace sgl::graphic;

    auto tm = TextureManager::Instance();

    RegisterRenderable(mBody);

    // ICON
    Texture * tex = tm->GetSprite(SpriteFileMainMenuButtons, IND_MM_ICON_STEAM);
    mIcon = new Image(tex);
    RegisterRenderable(mIcon);

    // TEXT LABEL
    // TODO use setLabel after adding support for icon to PushButton
    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("Lato-Regular.ttf", 18, Font::NORMAL);
    mText = new Text("WISHLIST NOW", font, true);
    RegisterRenderable(mText);

    // init to normal state
    SetState(NORMAL);

    PositionElements();
}

void ButtonMainMenuWishlist::HandleMouseOver()
{
    sgl::sgui::AbstractButton::HandleMouseOver();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_over-02.ogg");
}

void ButtonMainMenuWishlist::HandleButtonDown()
{
    sgl::sgui::AbstractButton::HandleButtonDown();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_click-02.ogg");
}

void ButtonMainMenuWishlist::HandlePositionChanged()
{
     sgl::sgui::AbstractButton::HandlePositionChanged();

     PositionElements();
}

void ButtonMainMenuWishlist::OnStateChanged(sgl::sgui::AbstractButton::VisualState state)
{
    const unsigned int texIds[NUM_VISUAL_STATES] =
    {
        IND_MM_BTNW_NORMAL,
        IND_MM_BTNW_NORMAL,     // button can't be disabled
        IND_MM_BTNW_OVER,
        IND_MM_BTNW_PUSHED,
        IND_MM_BTNW_NORMAL,     // button can't be checked
    };

    auto tm = sgl::graphic::TextureManager::Instance();
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileMainMenuButtons, texIds[state]);
    mBody->SetTexture(tex);

    SetSize(mBody->GetWidth(), mBody->GetHeight());

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

    // position BG
    mBody->SetPosition(x, y);

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

