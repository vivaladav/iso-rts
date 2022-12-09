#include "Screens/ScreenPlanetMap.h"

#include "Game.h"
#include "GameConstants.h"
#include "MapsRegistry.h"
#include "Player.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonPlanetMap.h"
#include "Widgets/GameUIData.h"
#include "Widgets/PanelResources.h"
#include "Widgets/PanelPlanetActionConquer.h"
#include "Widgets/PanelPlanetActionConquerAI.h"
#include "Widgets/PanelPlanetActionExplore.h"
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
#include <sgl/utilities/LoadedDie.h>

namespace
{
    constexpr int costExploreMoney = 100;
    constexpr int costExploreEnergy = 50;
    constexpr int costExploreMaterial = 30;

    constexpr int costConquestMoney = 500;
    constexpr int costConquestEnergy = 400;
    constexpr int costConquestMaterial = 50;
    constexpr int costConquestDiamonds = 10;
}

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

    const int rendW = sgl::graphic::Renderer::Instance()->GetWidth();

    Player * player = game->GetLocalPlayer();

    // BACKGROUND
    tex = tm->GetTexture("data/img/space_bg.jpg");
    mBg = new graphic::Image(tex);

    // PANEL PLAYER RESOURCES TOP BAR
    mPanelPlayerRes = new PanelResources(player);
    mPanelPlayerRes->SetX((rendW - mPanelPlayerRes->GetWidth()) * 0.5f);

    // PANEL PLANET NAME
    tex = tm->GetSprite(SpriteFilePlanetMap2, IND_PM_PANEL_NAME);
    auto panelName = new sgui::Image(tex);

    fnt = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapTitle, graphic::Font::NORMAL);
    mLabelName = new sgui::Label(fnt, panelName);
    mLabelName->SetColor(colorHeader);

    // PANEL DATE
    tex = tm->GetSprite(SpriteFilePlanetMap2, IND_PM_PANEL_DATE);
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

    mPanelActions->AddOnButtonClickFunction(PanelPlanetActions::EXPLORE, [this]
    {
        mPanelActions->SetVisible(false);
        mPanelExplore->SetVisible(true);

        auto game = GetGame();
        auto mapReg = game->GetMapsRegistry();
        const int planetId = game->GetCurrentPlanet();
        const int territory = mPlanet->GetSelectedTerritoryId();
        const TerritoryStatus status = mapReg->GetMapStatus(planetId, territory);

        mPanelExplore->UpdateExplorationStatus(status);
    });

    mPanelActions->AddOnButtonClickFunction(PanelPlanetActions::CONQUER, [this]
    {
        mPanelActions->SetVisible(false);
        mPanelConquer->SetVisible(true);
    });

    mPanelActions->AddOnButtonClickFunction(PanelPlanetActions::SEND_AI, [this]
    {
        mPanelActions->SetVisible(false);
        mPanelConquerAI->SetVisible(true);

        auto game = GetGame();
        auto mapReg = game->GetMapsRegistry();
        const int planetId = game->GetCurrentPlanet();
        const int territory = mPlanet->GetSelectedTerritoryId();
        const TerritoryStatus status = mapReg->GetMapStatus(planetId, territory);
        const PlayerFaction occupier = mapReg->GetMapOccupier(planetId, territory);
        const bool playerOccupier = game->GetLocalPlayerFaction() == occupier;

        mPanelConquerAI->UpdateConquestStatus(status, playerOccupier);
    });

    // PANEL ACTION EXPLORE
    mPanelExplore = new PanelPlanetActionExplore(player, costExploreMoney,
                                                 costExploreEnergy, costExploreMaterial);
    mPanelExplore->SetY(panActionsY);
    mPanelExplore->SetVisible(false);

    mPanelExplore->AddOnButtonOkClickFunction([this]
    {
        auto game = GetGame();
        Player * player = game->GetLocalPlayer();

        player->SumResource(Player::Stat::MONEY, -costExploreMoney);
        player->SumResource(Player::Stat::ENERGY, -costExploreEnergy);
        player->SumResource(Player::Stat::MATERIAL, -costExploreMaterial);

        // handle the result
        const int planetId = game->GetCurrentPlanet();
        const int territory = mPlanet->GetSelectedTerritoryId();
        auto mapReg = game->GetMapsRegistry();

        const PlayerFaction occupier = mapReg->GetMapOccupier(planetId, territory);

        if(occupier != NO_FACTION)
        {
            const float probFail = 90.f;
            const float probSuccess = 100.f - probFail;

            sgl::utilities::LoadedDie die({probFail, probSuccess});

            const bool res = die.GetNextValue();

            if(res)
            {
                mapReg->SetMapStatus(planetId, territory, TER_ST_OCCUPIED);

                ShowInfo(territory);
            }
            else
            {
                mapReg->SetMapStatus(planetId, territory, TER_ST_OCCUPIED_UNEXPLORED);

                // PANEL INFO
                const int size = 0;
                const int value = 0;

                mPanelInfo->SetEnabled(true);
                mPanelInfo->SetData(size, TER_ST_OCCUPIED_UNEXPLORED, occupier, value);
            }

            mPanelExplore->ShowResult(res);
        }
        else
        {
            mapReg->SetMapStatus(planetId, territory, TER_ST_FREE);

            mPanelExplore->ShowResult(true);

            ShowInfo(territory);
        }

        // update panel actions
        const TerritoryStatus status = mapReg->GetMapStatus(planetId, territory);
        const bool playerOccupier = game->GetLocalPlayerFaction() == occupier;

        mPanelActions->UpdateButtons(status, playerOccupier);

        mPlanet->SetButtonState(territory, occupier, status);
    });

    mPanelExplore->AddOnButtonCancelClickFunction([this]
    {
        mPanelActions->SetVisible(true);
        mPanelExplore->SetVisible(false);
    });

    // PANEL ACTION CONQUER
    mPanelConquer = new PanelPlanetActionConquer;
    mPanelConquer->SetY(panActionsY);
    mPanelConquer->SetVisible(false);

    mPanelConquer->AddOnButtonOkClickFunction([this]
    {
        Game * game = GetGame();

        const int territory = mPlanet->GetSelectedTerritoryId();
        game->SetCurrentTerritory(territory);
        game->RequestNextActiveState(StateId::GAME);
    });

    mPanelConquer->AddOnButtonCancelClickFunction([this]
    {
        mPanelActions->SetVisible(true);
        mPanelConquer->SetVisible(false);
    });

    // PANEL ACTION CONQUER AI
    mPanelConquerAI = new PanelPlanetActionConquerAI(player, costConquestMoney, costConquestEnergy,
                                                     costConquestMaterial, costConquestDiamonds);
    mPanelConquerAI->SetY(panActionsY);
    mPanelConquerAI->SetVisible(false);

    mPanelConquerAI->AddOnButtonOkClickFunction([this]
    {
        Game * game = GetGame();
        Player * player = game->GetLocalPlayer();

        // pay costs
        player->SumResource(Player::Stat::MONEY, -costConquestMoney);
        player->SumResource(Player::Stat::ENERGY, -costConquestEnergy);
        player->SumResource(Player::Stat::MATERIAL, -costConquestMaterial);
        player->SumResource(Player::Stat::DIAMONDS, -costConquestDiamonds);

        // attempt the conquest
        const int territory = mPlanet->GetSelectedTerritoryId();
        const bool res = TryToConquerTerritory(territory);

        // conquest successful
        if(res)
        {
            const int planetId = game->GetCurrentPlanet();
            const PlayerFaction faction = game->GetLocalPlayerFaction();
            const TerritoryStatus status = TER_ST_OCCUPIED;

            MapsRegistry * mapReg = game->GetMapsRegistry();

            // update data
            mapReg->SetMapStatus(planetId, territory, status);
            mapReg->SetMapOccupier(planetId, territory, faction);

            ExpandTerritoryReach(territory);

            // update resources
            const int multMoney = 500;
            const int multRes1 = 100;
            const int multRes2 = 10;

            player->SumResource(Player::Stat::MONEY, multMoney * mapReg->GetMapValue(planetId, territory));
            player->SumResource(Player::Stat::ENERGY, multRes1 * mapReg->GetMapEnergy(planetId, territory));
            player->SumResource(Player::Stat::MATERIAL, multRes1 * mapReg->GetMapMaterial(planetId, territory));
            player->SumResource(Player::Stat::BLOBS, multRes2 * mapReg->GetMapBlobs(planetId, territory));
            player->SumResource(Player::Stat::DIAMONDS, multRes2 * mapReg->GetMapDiamonds(planetId, territory));

            // update UI
            mPlanet->SetButtonState(territory, faction, status);
            mPanelActions->UpdateButtons(status, true);

            ShowInfo(territory);
        }

        mPanelConquerAI->ShowResult(res);
    });

    mPanelConquerAI->AddOnButtonCancelClickFunction([this]
    {
        mPanelActions->SetVisible(true);
        mPanelConquerAI->SetVisible(false);
    });

    // PANEL INFO
    mPanelInfo = new PanelPlanetInfo;
    const int panelInfoY = mBg->GetHeight() - mPanelInfo->GetHeight();
    mPanelInfo->SetY(panelInfoY);
    mPanelInfo->SetEnabled(false);

    // PANEL LEAVE
    tex = tm->GetSprite(SpriteFilePlanetMap2, IND_PM_PANEL_LEAVE);
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
        const PlayerFaction occupier = mapReg->GetMapOccupier(planetId, i);
        const TerritoryStatus ts = mapReg->GetMapStatus(planetId, i);

        mPlanet->SetButtonState(i, occupier, ts);
    }

    mPlanet->SetFunctionOnToggle([this](unsigned int ind, bool enabled)
    {
        if(!enabled)
            return;

        // enable panels
        mPanelActions->SetEnabled(true);
        mPanelInfo->SetEnabled(true);

        // make sure panel actions is visible
        mPanelActions->SetVisible(true);
        mPanelExplore->SetVisible(false);
        mPanelConquer->SetVisible(false);
        mPanelConquerAI->SetVisible(false);

        auto game = GetGame();
        const int planetId = game->GetCurrentPlanet();
        auto mapReg = game->GetMapsRegistry();

        const TerritoryStatus status = mapReg->GetMapStatus(planetId, ind);
        const PlayerFaction occupier = mapReg->GetMapOccupier(planetId, ind);
        const bool playerOccupier = game->GetLocalPlayerFaction() == occupier;
        mPanelActions->UpdateButtons(status, playerOccupier);

        if(status == TER_ST_FREE || status == TER_ST_OCCUPIED)
            ShowInfo(ind);
        else if(status == TER_ST_OCCUPIED_UNEXPLORED)
        {
            mPanelInfo->SetEnabled(true);
            mPanelInfo->SetData(0, status, occupier, 0);
        }
        else
        {
            mPanelResources->SetEnabled(false);

            mPanelInfo->SetData(0, status, NO_FACTION, 0);
        }

        mPanelExplore->UpdateExplorationStatus(status);
        mPanelConquerAI->UpdateConquestStatus(status, playerOccupier);
    });

    SetPlanetName(PLANETS_NAME[planetId]);

    // TEST - REMOVE LATER
    SetDate("001 - 2200");
}

ScreenPlanetMap::~ScreenPlanetMap()
{
    sgl::sgui::Stage::Instance()->ClearWidgets();

    delete mBg;

    auto stage = sgl::sgui::Stage::Instance();

    stage->ClearWidgets();
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

void ScreenPlanetMap::ShowInfo(int territory)
{
    auto game = GetGame();
    const int planetId = game->GetCurrentPlanet();
    auto mapReg = game->GetMapsRegistry();

    // PANEL RESOURCES
    mPanelResources->SetEnabled(true);
    mPanelResources->SetResourceValue(RES_ENERGY, mapReg->GetMapEnergy(planetId, territory));
    mPanelResources->SetResourceValue(RES_MATERIAL1, mapReg->GetMapMaterial(planetId, territory));
    mPanelResources->SetResourceValue(RES_BLOBS, mapReg->GetMapBlobs(planetId, territory));
    mPanelResources->SetResourceValue(RES_DIAMONDS, mapReg->GetMapDiamonds(planetId, territory));

    // PANEL INFO
    const int size = mapReg->GetMapSize(planetId, territory);
    const int value = mapReg->GetMapValue(planetId, territory);
    const PlayerFaction occupier = mapReg->GetMapOccupier(planetId, territory);
    const TerritoryStatus status = mapReg->GetMapStatus(planetId, territory);

    mPanelInfo->SetEnabled(true);
    mPanelInfo->SetData(size, status, occupier, value);
}

bool ScreenPlanetMap::TryToConquerTerritory(int index)
{
    auto game = GetGame();
    auto mapReg = game->GetMapsRegistry();

    const int planetId = game->GetCurrentPlanet();
    const TerritoryStatus status = mapReg->GetMapStatus(planetId, index);

    float probSuccess = 60.f;

    // bonus explored and free
    if(status == TER_ST_FREE)
    {
        const float bonusExploredAndFree = 30.f;
        probSuccess += bonusExploredAndFree;
    }
    // bonus explored
    else if(status == TER_ST_OCCUPIED)
    {
        const float bonusExplored = 15.f;
        probSuccess += bonusExplored;
    }

    const float probFail = 100.f - probSuccess;

    sgl::utilities::LoadedDie die({ probFail, probSuccess });
    return die.GetNextValue();;
}

void ScreenPlanetMap::ExpandTerritoryReach(int index)
{
    switch (index)
    {
        case 0:
            TryToMakeTerrytoryUnexplored(2);
        break;

        case 1:
            TryToMakeTerrytoryUnexplored(3);
        break;

        case 2:
            TryToMakeTerrytoryUnexplored(0);
            TryToMakeTerrytoryUnexplored(3);
            TryToMakeTerrytoryUnexplored(5);
        break;

        case 3:
            TryToMakeTerrytoryUnexplored(1);
            TryToMakeTerrytoryUnexplored(2);
            TryToMakeTerrytoryUnexplored(6);
        break;

        case 4:
            TryToMakeTerrytoryUnexplored(5);
        break;

        case 5:
            TryToMakeTerrytoryUnexplored(2);
            TryToMakeTerrytoryUnexplored(4);
            TryToMakeTerrytoryUnexplored(8);
        break;

        case 6:
            TryToMakeTerrytoryUnexplored(3);
            TryToMakeTerrytoryUnexplored(7);
            TryToMakeTerrytoryUnexplored(9);
        break;

        case 7:
            TryToMakeTerrytoryUnexplored(6);
        break;

        case 8:
            TryToMakeTerrytoryUnexplored(5);
            TryToMakeTerrytoryUnexplored(9);
            TryToMakeTerrytoryUnexplored(10);
        break;

        case 9:
            TryToMakeTerrytoryUnexplored(6);
            TryToMakeTerrytoryUnexplored(8);
            TryToMakeTerrytoryUnexplored(11);
        break;

        case 10:
            TryToMakeTerrytoryUnexplored(8);
        break;

        case 11:
            TryToMakeTerrytoryUnexplored(9);
        break;

        default:
        break;
    }
}

void ScreenPlanetMap::TryToMakeTerrytoryUnexplored(int index)
{
    auto game = GetGame();
    const int planetId = game->GetCurrentPlanet();
    auto mapReg = game->GetMapsRegistry();

    if(mapReg->GetMapStatus(planetId, index) == TER_ST_UNREACHABLE)
    {
        mapReg->SetMapStatus(planetId, index, TER_ST_UNEXPLORED);

        const PlayerFaction faction = mapReg->GetMapOccupier(planetId, index);
        mPlanet->SetButtonState(index, faction, TER_ST_UNEXPLORED);
    }
}

} // namespace game
