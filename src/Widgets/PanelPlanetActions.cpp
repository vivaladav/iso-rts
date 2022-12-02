#include "Widgets/PanelPlanetActions.h"

#include "GameConstants.h"
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
    mButtons[EXPLORE] = new ButtonPlanetMap(this);
    mButtons[EXPLORE]->SetLabel("EXPLORE");

    mButtons[CONQUER] = new ButtonPlanetMap(this);
    mButtons[CONQUER]->SetLabel("CONQUER");

    mButtons[SEND_AI] = new ButtonPlanetMap(this);
    mButtons[SEND_AI]->SetLabel("SEND AI");

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

void PanelPlanetActions::AddOnButtonClickFunction(Button btn, const std::function<void()> & f)
{
    if(btn < NUM_BUTTONS)
        mButtons[btn]->AddOnClickFunction(f);
}

void PanelPlanetActions::UpdateButtons(TerritoryStatus status, bool isPlayerOccupier)
{
    const bool explored = TER_ST_FREE == status || TER_ST_OCCUPIED_UNEXPLORED == status;

    mButtons[EXPLORE]->SetEnabled(!explored);
    mButtons[CONQUER]->SetEnabled(!isPlayerOccupier);
    mButtons[SEND_AI]->SetEnabled(!isPlayerOccupier);
}

void PanelPlanetActions::HandlePositionChanged()
{
    UpdatePositions();
}

void PanelPlanetActions::HandleStateEnabled()
{
    mButtons[EXPLORE]->SetEnabled(true);
    mButtons[CONQUER]->SetEnabled(true);
    mButtons[SEND_AI]->SetEnabled(true);

    mTextExplore->SetColor(textColorEna);
    mTextConquer->SetColor(textColorEna);
    mTextSendAI->SetColor(textColorEna);
}

void PanelPlanetActions::HandleStateDisabled()
{
    mButtons[EXPLORE]->SetEnabled(false);
    mButtons[CONQUER]->SetEnabled(false);
    mButtons[SEND_AI]->SetEnabled(false);

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

    mButtons[EXPLORE]->SetPosition(x, y);

    y += buttonBlockH;

    mButtons[CONQUER]->SetPosition(x, y);

    y += buttonBlockH;

    mButtons[SEND_AI]->SetPosition(x, y);

    // TEXT
    x = x0 + marginL;
    y = y0 + mButtons[EXPLORE]->GetY() + mButtons[EXPLORE]->GetHeight() + marginTextT;

    mTextExplore->SetPosition(x, y);

    y = y0 + mButtons[CONQUER]->GetY() + mButtons[CONQUER]->GetHeight() + marginTextT;

    mTextConquer->SetPosition(x, y);

    y = y0 + mButtons[SEND_AI]->GetY() + mButtons[SEND_AI]->GetHeight() + marginTextT;

    mTextSendAI->SetPosition(x, y);
}

} // namespace game
