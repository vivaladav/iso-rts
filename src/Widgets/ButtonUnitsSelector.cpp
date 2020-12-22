#include "ButtonUnitsSelector.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>

namespace game
{

ButtonUnitsSelector::ButtonUnitsSelector(const char * text)
    : GameButton({
                    "data/img/buttons/multi_sel-01.png",
                    "data/img/buttons/multi_sel-02.png",
                    "data/img/buttons/multi_sel-03.png",
                    "data/img/buttons/multi_sel-04.png",
                    "data/img/buttons/multi_sel-05.png",
                 },
                 {
                     0xFFFFFFFF,
                     0xb2b2b2ff,
                     0xFFFFFFFF,
                     0xFFFFFFFF,
                     0xFFFFFFFF
                 },
                 nullptr)
{
    using namespace lib::graphic;

    // set label font
    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/OpenSans.ttf", 24, Font::BOLD);

    SetLabelFont(font);

    // init label
    SetLabel(text);
}

} // namespace game
