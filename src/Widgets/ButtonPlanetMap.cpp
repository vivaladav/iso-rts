#include "ButtonPlanetMap.h"

#include "Widgets/GameUIData.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>

namespace game
{

ButtonPlanetMap::ButtonPlanetMap(sgl::sgui::Widget * parent) :
    GameButton(SpriteFilePlanetMap,
        { IND_PM_BTN_NORMAL, IND_PM_BTN_DISABLED, IND_PM_BTN_OVER,
          IND_PM_BTN_PRESSED, IND_PM_BTN_NORMAL },
        { 0xd7eaf4ff, 0x506c7cff, 0xebf4f9ff, 0xc3dfeeff, 0xd7eaf4ff },
        parent)
{
    const char * fileFont = "data/fonts/Lato-Regular.ttf";
    const int size = 20;

    auto fm = sgl::graphic::FontManager::Instance();
    sgl::graphic::Font * fnt = fm->GetFont(fileFont, size, sgl::graphic::Font::NORMAL);
    SetLabelFont(fnt);
}

SecondaryButtonPlanetMap::SecondaryButtonPlanetMap(sgl::sgui::Widget * parent) :
    GameButton(SpriteFilePlanetMap,
        { IND_PM_BTN2_NORMAL, IND_PM_BTN2_DISABLED, IND_PM_BTN2_OVER,
          IND_PM_BTN2_PRESSED, IND_PM_BTN2_NORMAL },
        { 0xf4e5d7ff, 0x7c6650ff, 0xf9f2ebff, 0xeed9c3ff, 0xf4e5d7ff },
        parent)
{
    const char * fileFont = "data/fonts/Lato-Regular.ttf";
    const int size = 20;

    auto fm = sgl::graphic::FontManager::Instance();
    sgl::graphic::Font * fnt = fm->GetFont(fileFont, size, sgl::graphic::Font::NORMAL);
    SetLabelFont(fnt);
}

} // namespace game
