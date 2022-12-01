#include "Widgets/PanelPlanetActions.h"

#include "Widgets/ButtonPlanetMap.h"
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

constexpr unsigned int textColorEna = 0x80a2b3ff;
constexpr unsigned int textColorDis = 0x4c6f80ff;

PanelPlanetActions::PanelPlanetActions()
    : sgl::sgui::Widget(nullptr)
{
    using namespace sgl;

    auto tm = graphic::TextureManager::Instance();
    auto fm = graphic::FontManager::Instance();

    // BACKGROUND
    graphic::Texture * tex = tm->GetSprite(SpriteFilePlanetMap2, IND_PM_PANEL_ACTIONS);
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

    mButtonConquer = new ButtonPlanetMap(this);
    mButtonConquer->SetLabel("CONQUER");

    mButtonSendAI = new ButtonPlanetMap(this);
    mButtonSendAI->SetLabel("SEND AI");

    // TEXT

    const int textSize = 18;
    fnt = fm->GetFont(fileFont, textSize, graphic::Font::NORMAL);

    mTextExplore = new graphic::Text("Send scouts to explore the territory.", fnt);

    RegisterRenderable(mTextExplore);

    mTextConquer = new graphic::Text("Play to conquer the territory.", fnt);

    RegisterRenderable(mTextConquer);

    mTextSendAI = new graphic::Text("An AI general handles the conquest for you.", fnt);

    RegisterRenderable(mTextSendAI);

    // position elements
    UpdatePositions();
}

void PanelPlanetActions::HandlePositionChanged()
{
    UpdatePositions();
}

void PanelPlanetActions::HandleStateEnabled()
{
    mButtonExplore->SetEnabled(true);
    mButtonConquer->SetEnabled(true);
    mButtonSendAI->SetEnabled(true);

    mTextExplore->SetColor(textColorEna);
    mTextConquer->SetColor(textColorEna);
    mTextSendAI->SetColor(textColorEna);
}

void PanelPlanetActions::HandleStateDisabled()
{
    mButtonExplore->SetEnabled(false);
    mButtonConquer->SetEnabled(false);
    mButtonSendAI->SetEnabled(false);

    mTextExplore->SetColor(textColorDis);
    mTextConquer->SetColor(textColorDis);
    mTextSendAI->SetColor(textColorDis);
}

void PanelPlanetActions::UpdatePositions()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    const int marginL = 20;
    const int marginR = 30;
    const int marginT = 15;
    const int marginTextT = 5;

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
    const int buttonBlockH = 110;

    x = marginL;
    y = marginT + mTitle->GetHeight() + marginButtons;

    mButtonExplore->SetPosition(x, y);

    y += buttonBlockH;

    mButtonConquer->SetPosition(x, y);

    y += buttonBlockH;

    mButtonSendAI->SetPosition(x, y);

    // TEXT
    x = x0 + marginL;
    y = y0 + mButtonExplore->GetY() + mButtonExplore->GetHeight() + marginTextT;

    mTextExplore->SetPosition(x, y);

    y = y0 + mButtonConquer->GetY() + mButtonConquer->GetHeight() + marginTextT;

    mTextConquer->SetPosition(x, y);

    y = y0 + mButtonSendAI->GetY() + mButtonSendAI->GetHeight() + marginTextT;

    mTextSendAI->SetPosition(x, y);
}

} // namespace game
