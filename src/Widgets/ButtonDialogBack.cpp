#include "ButtonDialogBack.h"

#include "Widgets/GameUIData.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/media/AudioManager.h>
#include <sgl/media/AudioPlayer.h>

namespace game
{

ButtonDialogBack::ButtonDialogBack(sgl::sgui::Widget * parent)
    : GameButton(SpriteFileFactionSelection,
                {
                    IND_FSEL_BACK_NORMAL,
                    IND_FSEL_BACK_NORMAL,
                    IND_FSEL_BACK_OVER,
                    IND_FSEL_BACK_PUSHED,
                    IND_FSEL_BACK_NORMAL
                 },
                 {
                    0xd6ebf5ff,
                    0xFFFFFFFF,
                    0xecf5f9ff,
                    0xd7eaf4ff,
                    0xFFFFFFFF
                 },
                 parent)
{
    using namespace sgl::graphic;

    // set label font
    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("Lato-Regular.ttf", 28, Font::NORMAL);

    SetLabelFont(font);

    // init label
    SetLabel("BACK");
}

void ButtonDialogBack::HandleMouseOver()
{
    sgl::sgui::AbstractButton::HandleMouseOver();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_over-02.ogg");
}

void ButtonDialogBack::HandleButtonDown()
{
    sgl::sgui::AbstractButton::HandleButtonDown();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_click-02.ogg");
}

} // namespace game
