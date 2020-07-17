#include "ButtonPanelPlayer.h"

namespace game
{

ButtonPanelPlayer::ButtonPanelPlayer(lib::sgui::Widget * parent)
    : GameButton(parent)
{
    SetData({
                "data/img/buttons/player_ui-01.png",
                "data/img/buttons/player_ui-02.png",
                "data/img/buttons/player_ui-03.png",
                "data/img/buttons/player_ui-04.png"
            },
            {
                0xFFFFFFFF,
                0xb2b2b2ff,
                0xFFFFFFFF,
                0xFFFFFFFF,
            },
            GameButton::NORMAL);
}

} // namespace game
