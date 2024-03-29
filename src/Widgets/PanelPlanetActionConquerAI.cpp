#include "Widgets/PanelPlanetActionConquerAI.h"

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

namespace
{
    constexpr unsigned int textColor = 0x80a2b3ff;
    constexpr int textSize = 18;
    const char * fileFont = "Lato-Regular.ttf";
}

namespace game
{

PanelPlanetActionConquerAI::PanelPlanetActionConquerAI(Player * player, int money, int energy, int material, int diamonds)
    : sgl::sgui::Widget(nullptr)
    , mPlayer(player)
    , mCostMoney(money)
    , mCostenergy(energy)
    , mCostmaterial(material)
    , mCostDiamonds(diamonds)
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
    mTitle = new graphic::Text("SEND AI", fnt);
    mTitle->SetColor(colorTitle);
    RegisterRenderable(mTitle);

    // CONTENT
    CreateContentStart(money, energy, material, diamonds);
    CreateContentFailure();
    CreateContentSuccess();

    mContentFailure->SetVisible(false);
    mContentSuccess->SetVisible(false);

    // BUTTONS
    mButtonOk = new ButtonPlanetMap(this);
    mButtonOk->SetLabel("PROCEED");

    mButtonCancel = new SecondaryButtonPlanetMap(this);
    mButtonCancel->SetLabel("CANCEL");

    // position elements
    UpdatePositions();
}

void PanelPlanetActionConquerAI::ShowAction()
{
    // show content
    mContentStart->SetVisible(true);
    mContentFailure->SetVisible(false);
    mContentSuccess->SetVisible(false);

    // update costs
    const bool enoughMoney = mPlayer->HasEnough(Player::Stat::MONEY, mCostMoney);
    const bool enoughEnergy = mPlayer->HasEnough(Player::Stat::ENERGY, mCostenergy);
    const bool enoughMaterial = mPlayer->HasEnough(Player::Stat::MATERIAL, mCostmaterial);
    const bool enoughDiamonds = mPlayer->HasEnough(Player::Stat::DIAMONDS, mCostDiamonds);
    const bool canConquer = enoughMoney && enoughEnergy && enoughMaterial && enoughDiamonds;

    mLabelMoney->SetColor(enoughMoney ? WidgetsConstants::colorPlanetMapEnoughRes :
                                        WidgetsConstants::colorPlanetMapLackRes);
    mLabelEnergy->SetColor(enoughEnergy ? WidgetsConstants::colorPlanetMapEnoughRes :
                                          WidgetsConstants::colorPlanetMapLackRes);
    mLabelMaterial->SetColor(enoughMaterial ? WidgetsConstants::colorPlanetMapEnoughRes :
                                              WidgetsConstants::colorPlanetMapLackRes);
    mLabelDiamonds->SetColor(enoughDiamonds ? WidgetsConstants::colorPlanetMapEnoughRes :
                                              WidgetsConstants::colorPlanetMapLackRes);

    // update buttons
    mButtonOk->SetVisible(true);
    mButtonOk->SetEnabled(canConquer);

    mButtonCancel->SetLabel("CANCEL");
}

void PanelPlanetActionConquerAI::ShowResult(bool success)
{
    mContentStart->SetVisible(false);

    mContentFailure->SetVisible(!success);
    mContentSuccess->SetVisible(success);

    mButtonOk->SetVisible(false);

    mButtonCancel->SetLabel("CLOSE");
}

void PanelPlanetActionConquerAI::AddOnButtonOkClickFunction(const std::function<void()> & f)
{
    mButtonOk->AddOnClickFunction(f);
}

void PanelPlanetActionConquerAI::AddOnButtonCancelClickFunction(const std::function<void()> & f)
{
    mButtonCancel->AddOnClickFunction(f);
}

void PanelPlanetActionConquerAI::CreateContentStart(int money, int energy, int material, int diamonds)
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
    const char * txt = "Send an AI general to conquer the territory.\n\n"
                       "This will cost you:";
    auto text = new sgui::TextArea(contW, contH, txt, fnt, false, mContentStart);
    text->SetColor(textColor);

    // COSTS
    auto contCosts = new sgui::Widget(mContentStart);

    const int valueBlockW = 110;
    const int spacingValue = 5;

    int x = 0;
    int y = 0;

    // money
    graphic::Texture * tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_ICON_MONEY);
    auto icon = new sgui::Image(tex, contCosts);

    mLabelMoney = new sgui::Label(std::to_string(money).c_str(), fnt, contCosts);

    x = icon->GetWidth() + spacingValue;
    y = (icon->GetHeight() - mLabelMoney->GetHeight()) * 0.5f;
    mLabelMoney->SetPosition(x, y);

    // energy
    x = valueBlockW;

    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_ICON_ENERGY);
    icon = new sgui::Image(tex, contCosts);
    icon->SetX(x);

    mLabelEnergy = new sgui::Label(std::to_string(energy).c_str(), fnt, contCosts);

    x += icon->GetWidth() + spacingValue;
    y = (icon->GetHeight() - mLabelEnergy->GetHeight()) * 0.5f;
    mLabelEnergy->SetPosition(x, y);

    // material
    x = 2 * valueBlockW;

    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_ICON_MATERIAL);
    icon = new sgui::Image(tex, contCosts);
    icon->SetX(x);

    mLabelMaterial = new sgui::Label(std::to_string(material).c_str(), fnt, contCosts);

    x += icon->GetWidth() + spacingValue;
    y = (icon->GetHeight() - mLabelMaterial->GetHeight()) * 0.5f;
    mLabelMaterial->SetPosition(x, y);

    // diamonds
    x = 3 * valueBlockW;

    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_ICON_DIAMONDS);
    icon = new sgui::Image(tex, contCosts);
    icon->SetX(x);

    mLabelDiamonds = new sgui::Label(std::to_string(diamonds).c_str(), fnt, contCosts);

    x += icon->GetWidth() + spacingValue;
    y = (icon->GetHeight() - mLabelDiamonds->GetHeight()) * 0.5f;
    mLabelDiamonds->SetPosition(x, y);

    // position row
    x = (w - contCosts->GetWidth()) * 0.5f - marginL;
    y = text->GetHeight();
    contCosts->SetPosition(x, y);
}

void PanelPlanetActionConquerAI::CreateContentFailure()
{
    using namespace sgl;

    mContentFailure = new sgui::Widget(this);

    auto fm = graphic::FontManager::Instance();

    const int w = GetWidth();

    // DESCRIPTION
    graphic::Font * fnt = fm->GetFont(fileFont, textSize, graphic::Font::NORMAL);

    const int marginL = 20;
    const int marginR = 20;
    const int contW = w - marginL - marginR;
    const int contH = 100;
    const char * txt = "Sorry Commander,\n"
                       "but the conquest failed.\n\n"
                       "Your enemy has prevailed.";
    auto text = new sgui::TextArea(contW, contH, txt, fnt, false, mContentFailure);
    text->SetColor(textColor);
}

void PanelPlanetActionConquerAI::CreateContentSuccess()
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
    const int contH = 120;
    const char * txt = "Good news Commander,\n"
                       "the conquest was successful!\n\n"
                       "Your resources have been replenished.";
    auto text = new sgui::TextArea(contW, contH, txt, fnt, false, mContentSuccess);
    text->SetColor(textColor);
}

void PanelPlanetActionConquerAI::HandlePositionChanged()
{
    UpdatePositions();
}

void PanelPlanetActionConquerAI::UpdatePositions()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();
    const int w = GetWidth();
    const int h = GetHeight();

    const int marginL = 20;
    const int marginT = 15;
    const int marginTextT = 5;

    int x;
    int y;

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
    mContentFailure->SetPosition(x, y);
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
