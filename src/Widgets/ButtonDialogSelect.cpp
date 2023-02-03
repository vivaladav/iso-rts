#include "ButtonDialogSelect.h"

#include "Widgets/GameUIData.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/media/AudioManager.h>
#include <sgl/media/AudioPlayer.h>

namespace game
{

ButtonDialogSelect::ButtonDialogSelect(sgl::sgui::Widget * parent)
    : GameButton(SpriteFileFactionSelection,
                {
                    IND_FSEL_SEL_NORMAL,
                    IND_FSEL_SEL_NORMAL,
                    IND_FSEL_SEL_OVER,
                    IND_FSEL_SEL_PUSHED,
                    IND_FSEL_SEL_CHECKED
                 },
                 {
                    0xd6ebf5ff,
                    0xFFFFFFFF,
                    0x99e5ffff,
                    0x8cd8f3ff,
                    0xf0f0a8ff
                 },
                 parent)
{
    using namespace sgl::graphic;

    SetCheckable(true);

    // set label font
    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("Lato-Regular.ttf", 28, Font::NORMAL);

    SetLabelFont(font);

    // init label
    SetLabel("SELECT");
}

void ButtonDialogSelect::HandleMouseOver()
{
    sgl::sgui::AbstractButton::HandleMouseOver();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_over-01.ogg");
}

void ButtonDialogSelect::HandleButtonDown()
{
    sgl::sgui::AbstractButton::HandleButtonDown();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_click-01.ogg");
}

} // namespace game
