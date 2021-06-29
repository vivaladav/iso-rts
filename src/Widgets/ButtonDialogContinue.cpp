#include "ButtonDialogContinue.h"

#include "Widgets/GameUIData.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>

namespace game
{

ButtonDialogContinue::ButtonDialogContinue(lib::sgui::Widget * parent)
    : GameButton(SpriteFileFactionSelection,
                {
                    IND_FSEL_CONTINUE_NORMAL,
                    IND_FSEL_CONTINUE_DISABLED,
                    IND_FSEL_CONTINUE_OVER,
                    IND_FSEL_CONTINUE_PUSHED,
                    IND_FSEL_CONTINUE_NORMAL
                 },
                 {
                    0xd6ebf5ff,
                    0x3d525cff,
                    0x99e5ffff,
                    0x8cd8f3ff,
                    0xFFFFFFFF
                 },
                 parent)
{
    using namespace lib::graphic;

    // set label font
    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/Lato-Regular.ttf", 32, Font::NORMAL);

    SetLabelFont(font);

    // init label
    SetLabel("CONTINUE");
}

} // namespace game
