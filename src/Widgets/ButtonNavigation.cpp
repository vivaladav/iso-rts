#include "ButtonNavigation.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>

namespace game
{

ButtonNavigation::ButtonNavigation(const char * text, lib::sgui::Widget * parent)
    : GameButton({
                    "data/img/buttons/navigation-01.png",
                    "data/img/buttons/navigation-02.png",
                    "data/img/buttons/navigation-03.png",
                    "data/img/buttons/navigation-04.png",
                    "data/img/buttons/navigation-05.png",
                 },
                 {
                    0xFFFFFFFF,
                    0xE5E5E5FF,
                    0xFFFFFFFF,
                    0xFFFFFFFF,
                    0xFFFFFFFF
                 },
                 parent)
{
    using namespace lib::graphic;

    // set label font
    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/OpenSans.ttf", 32);
    font->SetStyle(Font::BOLD);

    SetLabelFont(font);

    // init label
    SetLabel(text);
}

} // namespace game
