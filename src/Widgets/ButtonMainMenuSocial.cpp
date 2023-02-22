#include "Widgets/ButtonMainMenuSocial.h"

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

ButtonMainMenuSocial::ButtonMainMenuSocial(unsigned int icon, sgl::sgui::Widget * parent)
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
    Texture * tex = tm->GetSprite(SpriteFileMainMenuButtons, icon);
    mIcon = new Image(tex);
    RegisterRenderable(mIcon);

    // init to normal state
    SetState(NORMAL);

    PositionElements();
}

void ButtonMainMenuSocial::HandleMouseOver()
{
    sgl::sgui::AbstractButton::HandleMouseOver();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_over-02.ogg");
}

void ButtonMainMenuSocial::HandleButtonDown()
{
    sgl::sgui::AbstractButton::HandleButtonDown();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_click-02.ogg");
}

void ButtonMainMenuSocial::HandlePositionChanged()
{
     sgl::sgui::AbstractButton::HandlePositionChanged();

     PositionElements();
}

void ButtonMainMenuSocial::OnStateChanged(VisualState state)
{
    const unsigned int texIds[NUM_VISUAL_STATES] =
    {
        IND_MM_BTNS_NORMAL,
        IND_MM_BTNS_NORMAL,     // button can't be disabled
        IND_MM_BTNS_OVER,
        IND_MM_BTNS_PUSHED,
        IND_MM_BTNS_NORMAL,     // button can't be checked
    };

    auto tm = sgl::graphic::TextureManager::Instance();
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileMainMenuButtons, texIds[state]);
    mBody->SetTexture(tex);

    SetSize(mBody->GetWidth(), mBody->GetHeight());

    mIcon->SetColor(mLabelsColor[state]);
}

void ButtonMainMenuSocial::PositionElements()
{
    const int x = GetScreenX();
    const int y = GetScreenY();
    const int w = GetWidth();
    const int h = GetHeight();

    // position BG
    mBody->SetPosition(x, y);

    // center icon
    const int iconX = x + (w - mIcon->GetWidth()) * 0.5f;
    const int iconY = y + (h - mIcon->GetHeight()) * 0.5f;
    mIcon->SetPosition(iconX, iconY);
}

} // namespace game

