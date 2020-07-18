#include "ButtonPanelPlayer.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>

namespace game
{

ButtonPanelPlayer::ButtonPanelPlayer(lib::sgui::Widget * parent)
    : GameButton(parent)
{
    using namespace lib::graphic;

    SetData({
                "data/img/buttons/player_ui-01.png",
                "data/img/buttons/player_ui-02.png",
                "data/img/buttons/player_ui-03.png",
                "data/img/buttons/player_ui-04.png",
                "data/img/buttons/player_ui-05.png"
            },
            {
                0xFFFFFFFF,
                0xb2b2b2ff,
                0xFFFFFFFF,
                0xFFFFFFFF,
                0xFFFFFFFF
            },
            GameButton::NORMAL);

    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/OpenSans.ttf", 18);
    font->SetStyle(Font::BOLD);

    SetLabelFont(font);
}

ButtonPanelPlayer::ButtonPanelPlayer(const char * text, lib::sgui::Widget * parent)
    : ButtonPanelPlayer(parent)
{
    SetLabel(text);
}

} // namespace game
