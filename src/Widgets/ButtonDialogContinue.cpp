#include "ButtonDialogContinue.h"

#include "Widgets/GameUIData.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/media/AudioManager.h>
#include <sgl/media/AudioPlayer.h>

namespace game
{

ButtonDialogContinue::ButtonDialogContinue(sgl::sgui::Widget * parent)
    : GameButton(SpriteFileFactionSelection,
                {
                    IND_FSEL_CONTINUE_NORMAL,
                    IND_FSEL_CONTINUE_DISABLED,
                    IND_FSEL_CONTINUE_OVER,
                    IND_FSEL_CONTINUE_PUSHED,
                    IND_FSEL_CONTINUE_NORMAL
                 },
                 {
                    0xd6ebf5ff,
                    0x3d525cff,
                    0x99e5ffff,
                    0x8cd8f3ff,
                    0xFFFFFFFF
                 },
                 parent)
{
    using namespace sgl::graphic;

    // set label font
    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("Lato-Regular.ttf", 32, Font::NORMAL);

    SetLabelFont(font);

    // init label
    SetLabel("CONTINUE");
}

void ButtonDialogContinue::HandleMouseOver()
{
    sgl::sgui::AbstractButton::HandleMouseOver();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_over-01.ogg");
}

void ButtonDialogContinue::HandleButtonDown()
{
    sgl::sgui::AbstractButton::HandleButtonDown();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_click_long-01.ogg");
}

} // namespace game
