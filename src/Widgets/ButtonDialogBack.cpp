#include "ButtonDialogBack.h"

#include "Widgets/GameUIData.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>

namespace game
{

ButtonDialogBack::ButtonDialogBack(lib::sgui::Widget * parent)
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
    using namespace lib::graphic;

    // set label font
    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/Lato-Regular.ttf", 28, Font::NORMAL);

    SetLabelFont(font);

    // init label
    SetLabel("BACK");
}

} // namespace game
