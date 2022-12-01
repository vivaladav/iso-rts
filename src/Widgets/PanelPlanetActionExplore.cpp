#include "Widgets/PanelPlanetActionExplore.h"

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

PanelPlanetActionExplore::PanelPlanetActionExplore(int money, int energy, int material)
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
    mTitle = new graphic::Text("EXPLORE", fnt);
    mTitle->SetColor(colorTitle);
    RegisterRenderable(mTitle);

    // BUTTONS
    mButtonOk = new ButtonPlanetMap(this);
    mButtonOk->SetLabel("EXPLORE");

    mButtonCancel = new SecondaryButtonPlanetMap(this);
    mButtonCancel->SetLabel("CANCEL");

    // TEXT
    constexpr unsigned int textColor = 0x80a2b3ff;
    const int textSize = 18;

    fnt = fm->GetFont(fileFont, textSize, graphic::Font::NORMAL);

    mTextDesc = new graphic::Text("Send a squad of scouts to explore the territory.", fnt);
    mTextDesc->SetColor(textColor);
    RegisterRenderable(mTextDesc);

    mTextCost  = new graphic::Text("This will cost:", fnt);
    mTextCost->SetColor(textColor);
    RegisterRenderable(mTextCost);

    // ICONS
    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_MONEY);
    mIconMoney = new graphic::Image(tex);
    RegisterRenderable(mIconMoney);

    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_ICON_ENERGY);
    mIconEnergy = new graphic::Image(tex);
    RegisterRenderable(mIconEnergy);

    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_ICON_MATERIAL);
    mIconMaterial = new graphic::Image(tex);
    RegisterRenderable(mIconMaterial);

    // VALUES
    constexpr unsigned int valueColor = 0xb8bec7ff;

    mTextMoney = new graphic::Text(std::to_string(money).c_str(), fnt);
    mTextMoney->SetColor(valueColor);
    RegisterRenderable(mTextMoney);

    mTextEnergy = new graphic::Text(std::to_string(energy).c_str(), fnt);
    mTextEnergy->SetColor(valueColor);
    RegisterRenderable(mTextEnergy);

    mTextMaterial = new graphic::Text(std::to_string(material).c_str(), fnt);
    mTextMaterial->SetColor(valueColor);
    RegisterRenderable(mTextMaterial);

    // position elements
    UpdatePositions();
}

void PanelPlanetActionExplore::AddOnButtonOkClickFunction(const std::function<void()> & f)
{
    mButtonOk->AddOnClickFunction(f);
}

void PanelPlanetActionExplore::AddOnButtonCancelClickFunction(const std::function<void()> & f)
{
    mButtonCancel->AddOnClickFunction(f);
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

    // TEXT
    const int spacingText = 15;
    x = x0 + marginL;
    y = mTitle->GetY() + mTitle->GetHeight() + marginTextT;

    mTextDesc->SetPosition(x, y);

    y += mTextDesc->GetHeight() + spacingText;

    mTextCost->SetPosition(x, y);

    // VALUES
    const int valueBlockW = 130;
    const int spacingValue = 5;
    const int valuesW = (valueBlockW * 2) + mIconMaterial->GetWidth() +
                        spacingValue + mTextMaterial->GetWidth();

    x = x0 + (w - valuesW) * 0.5f;
    y += mTextCost->GetHeight() + spacingText;

    mIconMoney->SetPosition(x, y);

    x += valueBlockW;

    mIconEnergy->SetPosition(x, y);

    x += valueBlockW;

    mIconMaterial->SetPosition(x, y);

    x = mIconMoney->GetX() + mIconMoney->GetWidth() + spacingValue;
    y = mIconMoney->GetY() + (mIconMoney->GetHeight() - mTextMoney->GetHeight()) * 0.5f;

    mTextMoney->SetPosition(x, y);

    x = mIconEnergy->GetX() + mIconEnergy->GetWidth() + spacingValue;
    y = mIconEnergy->GetY() + (mIconEnergy->GetHeight() - mTextEnergy->GetHeight()) * 0.5f;

    mTextEnergy->SetPosition(x, y);

    x = mIconMaterial->GetX() + mIconMaterial->GetWidth() + spacingValue;
    y = mIconMaterial->GetY() + (mIconMaterial->GetHeight() - mTextMaterial->GetHeight()) * 0.5f;

    mTextMaterial->SetPosition(x, y);

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
