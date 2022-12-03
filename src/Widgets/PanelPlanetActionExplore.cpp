#include "Widgets/PanelPlanetActionExplore.h"

#include "GameConstants.h"
#include "Player.h"
#include "Widgets/ButtonPlanetMap.h"
#include "Widgets/GameUIData.h"
#include "Widgets/WidgetsConstants.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/Image.h>
#include <sgl/sgui/Label.h>
#include <sgl/sgui/TextArea.h>

#include <iostream>

namespace
{
    constexpr unsigned int textColor = 0x80a2b3ff;
    constexpr int textSize = 18;
    const char * fileFont = "data/fonts/Lato-Regular.ttf";
}

namespace game
{

PanelPlanetActionExplore::PanelPlanetActionExplore(Player * player, int money, int energy, int material)
    : sgl::sgui::Widget(nullptr)
    , mPlayer(player)
    , mCostMoney(money)
    , mCostenergy(energy)
    , mCostmaterial(material)
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
    const unsigned int colorTitle = 0xe9f7fbcc;

    graphic::Font * fnt = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapTitle,
                                      graphic::Font::NORMAL);
    mTitle = new graphic::Text("EXPLORE", fnt);
    mTitle->SetColor(colorTitle);
    RegisterRenderable(mTitle);

    // CONTENT
    CreateContentStart(money, energy, material);
    CreateContentSuccess();

    mContentSuccess->SetVisible(false);

    // BUTTONS
    mButtonOk = new ButtonPlanetMap(this);
    mButtonOk->SetLabel("EXPLORE");

    mButtonCancel = new SecondaryButtonPlanetMap(this);
    mButtonCancel->SetLabel("CANCEL");

    // position elements
    UpdatePositions();
}

void PanelPlanetActionExplore::UpdateExplorationStatus(TerritoryStatus status)
{
    const bool unexplored = status == TER_ST_UNEXPLORED || status == TER_ST_OCCUPIED_UNEXPLORED;

    mButtonOk->SetVisible(unexplored);

    mContentStart->SetVisible(unexplored);


    if(unexplored)
    {
        const int money = mPlayer->GetStat(Player::Stat::MONEY).GetIntValue();
        const int energy = mPlayer->GetStat(Player::Stat::ENERGY).GetIntValue();
        const int material = mPlayer->GetStat(Player::Stat::MATERIAL).GetIntValue();
        const bool enoughMoney = money >= mCostMoney;
        const bool enoughEnergy = energy >= mCostenergy;
        const bool enoughMaterial = material >= mCostmaterial;
        const bool canExplore = enoughMoney && enoughEnergy && enoughMaterial;

        mButtonOk->SetEnabled(canExplore);

        constexpr unsigned int enoughResColor = 0x85cc85ff;
        constexpr unsigned int lackResColor = 0xcc8b85ff;

        mLabelMoney->SetColor(enoughMoney ? enoughResColor : lackResColor);
        mLabelEnergy->SetColor(enoughEnergy ? enoughResColor : lackResColor);
        mLabelMaterial->SetColor(enoughMaterial ? enoughResColor : lackResColor);
    }

    // handle specific cases
    if(TER_ST_FREE == status || TER_ST_OCCUPIED == status)
    {
        mContentSuccess->SetVisible(true);

        mButtonCancel->SetLabel("CLOSE");
    }
    else
    {
        mContentSuccess->SetVisible(false);

        mButtonCancel->SetLabel("CANCEL");
    }
}

void PanelPlanetActionExplore::AddOnButtonOkClickFunction(const std::function<void()> & f)
{
    mButtonOk->AddOnClickFunction(f);
}

void PanelPlanetActionExplore::AddOnButtonCancelClickFunction(const std::function<void()> & f)
{
    mButtonCancel->AddOnClickFunction(f);
}

void PanelPlanetActionExplore::CreateContentStart(int money, int energy, int material)
{
    using namespace sgl;

    mContentStart = new sgui::Widget(this);

    auto fm = graphic::FontManager::Instance();
    auto tm = graphic::TextureManager::Instance();

    const int w = GetWidth();

    // DESCRIPTION
    graphic::Font * fnt = fm->GetFont(fileFont, textSize, graphic::Font::NORMAL);

    const int marginL = 20;
    const int marginR = 20;
    const int contW = w - marginL - marginR;
    const int contH = 80;
    const char * txt = "Send a squad of scouts to explore the territory.\n\nThis will cost you:";
    auto text = new sgui::TextArea(contW, contH, txt, fnt, mContentStart);
    text->SetColor(textColor);

    // COSTS
    auto contCosts = new sgui::Widget(mContentStart);

    const unsigned int valueColor = 0xb8bec7ff;
    const int valueBlockW = 130;
    const int spacingValue = 5;

    int x = 0;
    int y = 0;

    graphic::Texture * tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_ICON_MONEY);
    auto icon = new sgui::Image(tex, contCosts);

    mLabelMoney = new sgui::Label(std::to_string(money).c_str(), fnt, contCosts);
    mLabelMoney->SetColor(valueColor);

    x = icon->GetWidth() + spacingValue;
    y = (icon->GetHeight() - mLabelMoney->GetHeight()) * 0.5f;
    mLabelMoney->SetPosition(x, y);

    x = valueBlockW;

    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_ICON_ENERGY);
    icon = new sgui::Image(tex, contCosts);
    icon->SetX(x);

    mLabelEnergy = new sgui::Label(std::to_string(energy).c_str(), fnt, contCosts);
    mLabelEnergy->SetColor(valueColor);

    x += icon->GetWidth() + spacingValue;
    y = (icon->GetHeight() - mLabelEnergy->GetHeight()) * 0.5f;
    mLabelEnergy->SetPosition(x, y);

    x = valueBlockW * 2;

    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_ICON_MATERIAL);
    icon = new sgui::Image(tex, contCosts);
    icon->SetX(x);

    mLabelMaterial = new sgui::Label(std::to_string(material).c_str(), fnt, contCosts);
    mLabelMaterial->SetColor(valueColor);

    x += icon->GetWidth() + spacingValue;
    y = (icon->GetHeight() - mLabelMaterial->GetHeight()) * 0.5f;
    mLabelMaterial->SetPosition(x, y);

    x = (w - contCosts->GetWidth()) * 0.5f - marginL;
    y = text->GetHeight();
    contCosts->SetPosition(x, y);
}

void PanelPlanetActionExplore::CreateContentSuccess()
{
    using namespace sgl;

    mContentSuccess = new sgui::Widget(this);

    auto fm = graphic::FontManager::Instance();

    const int w = GetWidth();

    // DESCRIPTION
    graphic::Font * fnt = fm->GetFont(fileFont, textSize, graphic::Font::NORMAL);

    const int marginL = 20;
    const int marginR = 20;
    const int contW = w - marginL - marginR;
    const int contH = 100;
    const char * txt = "Exploration was successful!\n\nCheck out the other panels for the results.";
    auto text = new sgui::TextArea(contW, contH, txt, fnt, mContentSuccess);
    text->SetColor(textColor);
}

void PanelPlanetActionExplore::HandlePositionChanged()
{
    UpdatePositions();
}

void PanelPlanetActionExplore::UpdatePositions()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();
    const int w = GetWidth();
    const int h = GetHeight();

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

    // CONTENT
    x = marginL;
    y = marginT + mTitle->GetHeight() + marginTextT;

    mContentStart->SetPosition(x, y);

    mContentSuccess->SetPosition(x, y);

    // BUTTONS
    const int marginButtonsBottom = 50;
    const int spacingButtons = 20;

    x = (w - mButtonCancel->GetWidth()) * 0.5f;
    y = h - marginButtonsBottom - mButtonCancel->GetHeight();

    mButtonCancel->SetPosition(x, y);

    y -= spacingButtons + mButtonOk->GetHeight();

    mButtonOk->SetPosition(x, y);
}

} // namespace game
