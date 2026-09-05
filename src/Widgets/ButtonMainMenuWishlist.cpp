#include "Widgets/ButtonMainMenuWishlist.h"

#include "Widgets/GameUIData.h"
#include "Widgets/WidgetsConstants.h"

#include <sgl/graphic/Camera.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/graphic/Text.h>
#include <sgl/media/AudioManager.h>
#include <sgl/media/AudioPlayer.h>
#include <sgl/utilities/StringManager.h>

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
    using namespace sgl;

    auto tm = graphic::TextureManager::Instance();

    RegisterRenderable(mBody);

    // ICON
    graphic::Texture * tex = tm->GetSprite(SpriteFileMainMenuUI, ID_MM_ICON_STEAM);
    mIcon = new graphic::Image(tex);
    RegisterRenderable(mIcon);

    // TEXT LABEL
    auto sm = utilities::StringManager::Instance();
    // TODO use setLabel after adding support for icon to PushButton
    auto fm = graphic::FontManager::Instance();
    auto font = fm->GetFont(WidgetsConstants::FontFileButton, 18, graphic::Font::NORMAL);
    mText = new graphic::Text(sm->GetCString("WL_NOW"), font, true);
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
        ID_MM_BTN_STEAM_NORMAL,
        ID_MM_BTN_STEAM_NORMAL,     // button can't be disabled
        ID_MM_BTN_STEAM_OVER,
        ID_MM_BTN_STEAM_PUSHED,
        ID_MM_BTN_STEAM_NORMAL,     // button can't be checked
    };

    auto tm = sgl::graphic::TextureManager::Instance();
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileMainMenuUI, texIds[state]);
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

