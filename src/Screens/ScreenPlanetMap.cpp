#include "Screens/ScreenPlanetMap.h"

#include "Game.h"
#include "States/StatesIds.h"
#include "Widgets/GameButton.h"
#include "Widgets/GameUIData.h"
#include "Widgets/PanelPlanetResources.h"
#include "Widgets/PlanetMap.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/Image.h>
#include <sgl/sgui/Label.h>
#include <sgl/sgui/Stage.h>

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

// ===== SCREEN PLANET MAP =====
ScreenPlanetMap::ScreenPlanetMap(Game * game)
    : Screen(game)
{
    using namespace sgl;

    auto tm = graphic::TextureManager::Instance();
    auto fm = graphic::FontManager::Instance();
    graphic::Texture * tex = nullptr;
    graphic::Font * fnt = nullptr;

    const char * fileFont = "data/fonts/Lato-Regular.ttf";

    const unsigned int colorHeader = 0xe9f7fbcc;
    const int sizeHeader = 26;

    // BACKGROUND
    tex = tm->GetTexture("data/img/space_bg.jpg");
    mBg = new graphic::Image(tex);

    // PANEL PLANET NAME
    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_PANEL_NAME);
    auto panelName = new sgui::Image(tex);

    fnt = fm->GetFont(fileFont, sizeHeader, graphic::Font::NORMAL);
    mLabelName = new sgui::Label(fnt, panelName);
    mLabelName->SetColor(colorHeader);

    // PANEL DATE
    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_PANEL_DATE);
    auto panelDate = new sgui::Image(tex);
    panelDate->SetX(mBg->GetWidth() - panelDate->GetWidth());

    fnt = fm->GetFont(fileFont, sizeHeader, graphic::Font::NORMAL);
    mLabelDate = new sgui::Label(fnt, panelDate);
    mLabelDate->SetColor(colorHeader);

    // PANEL RESOURCES
    const int panelResY = 100;
    mPanelResources = new PanelPlanetResources;
    mPanelResources->SetY(panelResY);
    mPanelResources->SetEnabled(false);

    // PANEL LEAVE
    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_PANEL_LEAVE);
    auto panelLeave = new sgui::Image(tex);
    panelLeave->SetPosition(mBg->GetWidth() - panelLeave->GetWidth(),
                            mBg->GetHeight() - panelLeave->GetHeight());

    const int labelHeaderLeaveX = 35;
    const int labelHeaderLeaveY = 15;
    fnt = fm->GetFont(fileFont, sizeHeader, graphic::Font::NORMAL);
    auto labelHeaderLeave = new sgui::Label("LEAVE THE PLANET", fnt, panelLeave);
    labelHeaderLeave->SetColor(colorHeader);
    labelHeaderLeave->SetPosition(labelHeaderLeaveX, labelHeaderLeaveY);

    auto buttonLeave = new ButtonPlanetMap(panelLeave);
    buttonLeave->SetLabel("LEAVE");
    const int buttonLeaveY = labelHeaderLeaveY + labelHeaderLeave-> GetHeight() + 25;
    buttonLeave->SetPosition((panelLeave->GetWidth() - buttonLeave->GetWidth()) * 0.5f,
                              buttonLeaveY);

    buttonLeave->AddOnClickFunction([this]
    {
        // TODO change to galaxy screen
        GetGame()->RequestNextActiveState(StateId::MAIN_MENU);
    });

    // PLANET
    mPlanet = new PlanetMap;

    const int planetX = (mBg->GetWidth() - mPlanet->GetWidth()) * 0.5f;
    const int planetY = (mBg->GetHeight() - mPlanet->GetHeight()) * 0.5f;
    mPlanet->SetPosition(planetX, planetY);

    mPlanet->SetFunctionOnToggle([this](unsigned int ind, bool enabled)
    {
        if(!enabled)
            return;

        mPanelResources->SetEnabled(true);

        // TEST - REMOVE LATER
        mPanelResources->SetResourceValue(0, 1);
        mPanelResources->SetResourceValue(1, 2);
        mPanelResources->SetResourceValue(2, 3);
        mPanelResources->SetResourceValue(3, 4);
    });

    // TEST - REMOVE LATER
    SetPlanetName("TEST PLANET V");
    SetDate("001 - 2200");
}

ScreenPlanetMap::~ScreenPlanetMap()
{
    sgl::sgui::Stage::Instance()->ClearWidgets();

    delete mBg;
}

void ScreenPlanetMap::Update(float update)
{
}

void ScreenPlanetMap::Render()
{
    mBg->Render();
}

void ScreenPlanetMap::SetPlanetName(const char * name)
{
    mLabelName->SetText(name);

    sgl::sgui::Widget * parent = mLabelName->GetParent();
    const int x = (parent->GetWidth() - mLabelName->GetWidth()) * 0.5f;
    const int y = (parent->GetHeight() - mLabelName->GetHeight()) * 0.5f;
    mLabelName->SetPosition(x, y);
}

void ScreenPlanetMap::SetDate(const char * date)
{
    mLabelDate->SetText(date);

    sgl::sgui::Widget * parent = mLabelDate->GetParent();
    const int x = (parent->GetWidth() - mLabelDate->GetWidth()) * 0.5f;
    const int y = (parent->GetHeight() - mLabelDate->GetHeight()) * 0.5f;
    mLabelDate->SetPosition(x, y);
}

} // namespace game
