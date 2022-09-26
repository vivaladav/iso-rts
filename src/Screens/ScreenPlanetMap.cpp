#include "Screens/ScreenPlanetMap.h"

#include "Game.h"
#include "GameConstants.h"
#include "MapsRegistry.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonPlanetMap.h"
#include "Widgets/GameButton.h"
#include "Widgets/GameUIData.h"
#include "Widgets/PanelPlanetActions.h"
#include "Widgets/PanelPlanetInfo.h"
#include "Widgets/PanelPlanetResources.h"
#include "Widgets/PlanetMap.h"
#include "Widgets/WidgetsConstants.h"

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

    // BACKGROUND
    tex = tm->GetTexture("data/img/space_bg.jpg");
    mBg = new graphic::Image(tex);

    // PANEL PLANET NAME
    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_PANEL_NAME);
    auto panelName = new sgui::Image(tex);

    fnt = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapTitle, graphic::Font::NORMAL);
    mLabelName = new sgui::Label(fnt, panelName);
    mLabelName->SetColor(colorHeader);

    // PANEL DATE
    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_PANEL_DATE);
    auto panelDate = new sgui::Image(tex);
    panelDate->SetX(mBg->GetWidth() - panelDate->GetWidth());

    fnt = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapTitle, graphic::Font::NORMAL);
    mLabelDate = new sgui::Label(fnt, panelDate);
    mLabelDate->SetColor(colorHeader);

    // PANEL RESOURCES
    const int panelResY = 100;
    mPanelResources = new PanelPlanetResources;
    mPanelResources->SetY(panelResY);
    mPanelResources->SetEnabled(false);

    // PANEL ACTIONS
    mPanelActions = new PanelPlanetActions;
    const int panActionsY = (mBg->GetHeight() - mPanelActions->GetHeight()) * 0.5f;
    mPanelActions->SetY(panActionsY);
    mPanelActions->SetEnabled(false);

    // PANEL INFO
    mPanelInfo = new PanelPlanetInfo;
    const int panelInfoY = mBg->GetHeight() - mPanelInfo->GetHeight();
    mPanelInfo->SetY(panelInfoY);
    mPanelInfo->SetEnabled(false);

    // PANEL LEAVE
    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_PANEL_LEAVE);
    auto panelLeave = new sgui::Image(tex);
    panelLeave->SetPosition(mBg->GetWidth() - panelLeave->GetWidth(),
                            mBg->GetHeight() - panelLeave->GetHeight());

    const int labelHeaderLeaveX = 35;
    const int labelHeaderLeaveY = 15;
    fnt = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapTitle, graphic::Font::NORMAL);
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

    const int planetId = game->GetCurrentPlanet();
    auto mapReg = game->GetMapsRegistry();

    const int numMaps = mapReg->GetNumMaps(planetId);

    for(int i = 0; i < numMaps; ++i)
    {
        const TerritoryStatus ts = mapReg->GetMapStatus(planetId, i);

        mPlanet->SetButtonState(i, NO_FACTION, (i%2 == 0));

        if(ts == TER_ST_UNAVAILABLE)
            mPlanet->SetButtonEnabled(i, false);
        else
        {
            const PlayerFaction occupier = mapReg->GetMapOccupier(planetId, i);
            const bool explored = ts != TER_ST_UNEXPLORED;

            mPlanet->SetButtonState(i, occupier, explored);
        }
    }

    mPlanet->SetFunctionOnToggle([this](unsigned int ind, bool enabled)
    {
        if(!enabled)
            return;

        // enable panels
        mPanelResources->SetEnabled(true);
        mPanelActions->SetEnabled(true);
        mPanelInfo->SetEnabled(true);

        auto game = GetGame();
        const int planetId = game->GetCurrentPlanet();
        auto mapReg = game->GetMapsRegistry();

        mPanelResources->SetResourceValue(RES_ENERGY, mapReg->GetMapEnergy(planetId, ind));
        mPanelResources->SetResourceValue(RES_MATERIAL1, mapReg->GetMapMaterial(planetId, ind));
        mPanelResources->SetResourceValue(RES_BLOBS, mapReg->GetMapBlobs(planetId, ind));
        mPanelResources->SetResourceValue(RES_DIAMONDS, mapReg->GetMapDiamonds(planetId, ind));

        mPanelInfo->SetTerritoryValue(mapReg->GetMapValue(planetId, ind));
        mPanelInfo->SetTerritorySize(mapReg->GetMapSize(planetId, ind));
        mPanelInfo->SetTerritoryStatus(mapReg->GetMapStatus(planetId, ind));
        mPanelInfo->SetTerritoryOccupier(mapReg->GetMapOccupier(planetId, ind));
    });

    SetPlanetName(PLANETS_NAME[planetId]);

    // TEST - REMOVE LATER
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
