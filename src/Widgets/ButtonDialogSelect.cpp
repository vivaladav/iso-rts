#include "ButtonDialogSelect.h"

#include "Widgets/GameUIData.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>

namespace game
{

ButtonDialogSelect::ButtonDialogSelect(lib::sgui::Widget * parent)
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
    using namespace lib::graphic;

    // set label font
    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/Lato-Regular.ttf", 28, Font::NORMAL);

    SetLabelFont(font);

    // init label
    SetLabel("SELECT");
}

} // namespace game
