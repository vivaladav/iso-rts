#include "ButtonMainMenu.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>

namespace game
{

ButtonMainMenu::ButtonMainMenu(const char * text, lib::sgui::Widget * parent)
    : GameButton({
                    "data/img/buttons/main_menu-bg-01.png",
                    "data/img/buttons/main_menu-bg-02.png",
                    "data/img/buttons/main_menu-bg-03.png",
                    "data/img/buttons/main_menu-bg-04.png",
                    "data/img/buttons/main_menu-bg-05.png",
                 },
                 {
                    0x183929ff,
                    0x738c82ff,
                    0x183929ff,
                    0x183929ff,
                    0x183929ff
                 },
                 parent)
{
    using namespace lib::graphic;

    // set label font
    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/OpenSans.ttf", 32, Font::BOLD);

    SetLabelFont(font);

    // init label
    SetLabel(text);
}

} // namespace game
