#include "ButtonMainMenu.h"

namespace game
{

ButtonMainMenu::ButtonMainMenu(const char * text, lib::sgui::Widget * parent)
    : GameButton(text,
                 {
                    "data/img/buttons/main_menu-bg-01.png",
                    "data/img/buttons/main_menu-bg-02.png",
                    "data/img/buttons/main_menu-bg-03.png",
                    "data/img/buttons/main_menu-bg-04.png"
                 },
                 {
                    0x183929ff,
                    0x738c82ff,
                    0x183929ff,
                    0x183929ff
                 },
                 parent)
{

}

} // namespace game
