#include "Widgets/ButtonMainMenu.h"

#include "Widgets/GameUIData.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>

namespace game
{

ButtonMainMenu::ButtonMainMenu(const char * text, lib::sgui::Widget * parent)
    : GameButton(parent)
{
    using namespace lib::graphic;

    SetData(SpriteFileMainMenu,
            {
                IND_MM_BTN1_NORMAL,
                IND_MM_BTN1_DISABLED,
                IND_MM_BTN1_OVER,
                IND_MM_BTN1_PUSHED,
                IND_MM_BTN1_CHECKED,
            },
            {
                0xd6ebf5ff,
                0x395260ff,
                0x99e5ffff,
                0x8cd8f3ff,
                0xf0f0a8ff
            },
            GameButton::NORMAL);

    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/Lato-Regular.ttf", 34, Font::NORMAL);

    SetLabelFont(font);

    SetLabel(text);
}

} // namespace game
