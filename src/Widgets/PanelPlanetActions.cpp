#include "Widgets/PanelPlanetActions.h"

#include "Widgets/GameButton.h"
#include "Widgets/GameUIData.h"
#include "Widgets/WidgetsConstants.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>

namespace game
{

// ===== BUTTON PLANET MAP PANELS =====
class ButtonPlanetMap : public GameButton
{
public:
    ButtonPlanetMap(sgl::sgui::Widget * parent)
        : GameButton(SpriteFilePlanetMap,
                     { IND_PM_BTN_NORMAL, IND_PM_BTN_DISABLED,
                       IND_PM_BTN_OVER, IND_PM_BTN_PRESSED,
                       IND_PM_BTN_NORMAL },
                     { 0xd7eaf4ff, 0x506c7cff, 0xebf4f9ff,
                       0xc3dfeeff, 0xd7eaf4ff },  parent)
    {
        const char * fileFont = "data/fonts/Lato-Regular.ttf";
        const int size = 20;

        auto fm = sgl::graphic::FontManager::Instance();
        sgl::graphic::Font * fnt = fm->GetFont(fileFont, size, sgl::graphic::Font::NORMAL);
        SetLabelFont(fnt);
    }
};

// ===== PANEL PLANET ACTIONS =====
PanelPlanetActions::PanelPlanetActions()
    : sgl::sgui::Widget(nullptr)
{
    using namespace sgl;

    auto tm = graphic::TextureManager::Instance();
    auto fm = graphic::FontManager::Instance();

    // BACKGROUND
    graphic::Texture * tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_PANEL_ACTIONS);
    mBg = new graphic::Image(tex);
    RegisterRenderable(mBg);

    SetSize(tex->GetWidth(), tex->GetHeight());

    // TITLE
    const char * fileFont = "data/fonts/Lato-Regular.ttf";
    const unsigned int colorTitle = 0xe9f7fbcc;

    graphic::Font * fnt = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapTitle,
                                      graphic::Font::NORMAL);
    mTitle = new graphic::Text("ACTIONS", fnt);
    mTitle->SetColor(colorTitle);
    RegisterRenderable(mTitle);

    // BUTTONS
    mButtonExplore = new ButtonPlanetMap(this);
    mButtonExplore->SetLabel("EXPLORE");

    // position elements
    UpdatePositions();
}

void PanelPlanetActions::HandlePositionChanged()
{
    UpdatePositions();
}

void PanelPlanetActions::UpdatePositions()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    const int marginL = 20;
    const int marginR = 30;
    const int marginT = 15;

    int x;
    int y;
    int dataX;

    // BACKGROUND
    mBg->SetPosition(x0, y0);

    // TITLE
    x = x0 + marginL;
    y = y0 + marginT;

    mTitle->SetPosition(x, y);

    // BUTTONS
    const int marginButtons = 30;

    x = marginL;
    y = marginT + mTitle->GetHeight() + marginButtons;

    mButtonExplore->SetPosition(x, y);
}

} // namespace game
