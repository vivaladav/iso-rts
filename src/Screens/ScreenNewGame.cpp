#include "Screens/ScreenNewGame.h"

#include "Game.h"
#include "Player.h"
#include "AI/PlayerAI.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"
#include "Widgets/ButtonUnitsSelector.h"
#include "Widgets/MapPreview.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Renderer.h>
#include <sgui/ButtonsGroup.h>
#include <sgui/Label.h>
#include <sgui/PushButton.h>
#include <sgui/Stage.h>

#include <iostream>

namespace game
{

ScreenNewGame::ScreenNewGame(Game * game)
    : Screen(game)
    , mDiff(Difficulty::EASY)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    game->SetClearColor(0xE5, 0xE5, 0xE5, 0xFF);

    const int marginL = 25;
    const int marginT = 10;
    const int marginTitleB = 50;
    const int marginHeaderB = 20;
    const int marginWidgetsH = 100;
    const int marginWidgetsV = 50;
    int widgetY = marginT;

    // -- TITLE --
    FontManager * fm = FontManager::Instance();

    Font * fontTitle = fm->GetFont("data/fonts/OpenSans.ttf", 48);
    fontTitle->SetStyle(Font::BOLD);

    auto * title = new Label("SINGLE PLAYER GAME", fontTitle);
    title->SetColor(0x111111FF);
    title->SetPosition(marginL, widgetY);

    widgetY += title->GetHeight() + marginTitleB;

    // -- CPU PLAYERS --
    const unsigned int colorHeader = 0x333333FF;
    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 32);
    fontTitle->SetStyle(Font::BOLD);

    Label * headerPlayers = new Label("CPU PLAYERS", fontHeader);
    headerPlayers->SetColor(colorHeader);
    headerPlayers->SetPosition(marginL, widgetY);

    widgetY += headerPlayers->GetHeight() + marginHeaderB;

    // buttons
    auto * bgPlayers = new ButtonsGroup(ButtonsGroup::HORIZONTAL);
    bgPlayers->SetPosition(headerPlayers->GetX(), widgetY);

    bgPlayers->AddButton(new ButtonUnitsSelector("1"));
    bgPlayers->AddButton(new ButtonUnitsSelector("2"));
    bgPlayers->AddButton(new ButtonUnitsSelector("3"));

    bgPlayers->SetButtonChecked(0, true);

    bgPlayers->SetFunctionOnToggle([this](unsigned int ind, bool checked)
    {
        if(checked)
            mCpuPlayers = ind + 1;
    });

    // -- DIFFICULTY --
    auto * headerDiff = new Label("DIFFICULTY", fontHeader);
    headerDiff->SetColor(colorHeader);
    headerDiff->SetPosition(headerPlayers->GetX() + headerPlayers->GetWidth() + marginWidgetsH, headerPlayers->GetY());

    // buttons
    auto * bgDiff = new ButtonsGroup(ButtonsGroup::HORIZONTAL);
    bgDiff->SetPosition(headerDiff->GetX(), widgetY);

    bgDiff->AddButton(new ButtonUnitsSelector("1"));
    bgDiff->AddButton(new ButtonUnitsSelector("2"));
    bgDiff->AddButton(new ButtonUnitsSelector("3"));

    bgDiff->SetButtonChecked(0, true);

    bgDiff->SetFunctionOnToggle([this](unsigned int ind, bool checked)
    {
        if(checked)
            mDiff = static_cast<Difficulty>(ind);
    });

    widgetY += bgDiff->GetHeight() + marginWidgetsV;

    // -- MAP --
    auto * headerMap = new Label("MAP", fontHeader);
    headerMap->SetColor(colorHeader);
    headerMap->SetPosition(marginL, widgetY);

    widgetY += headerMap->GetHeight() + marginHeaderB;

    // MAP PREVIEW
    const int marginMapH = 174; // change later when adding navigation button
    const int marginMapV = 60; // change later when adding navigation button
    const int mapX0 = 125;      // change later when adding navigation button
    int mapX = mapX0;

    MapPreview * preview = new MapPreview("data/maps/5x5-01.map");
    preview->SetPosition(mapX, widgetY);
    mMapPreviews.emplace_back(preview);

    mapX += preview->GetWidth() + marginMapH;

    preview = new MapPreview("data/maps/6x6-01.map");
    preview->SetPosition(mapX, widgetY);
    mMapPreviews.emplace_back(preview);

    mapX += preview->GetWidth() + marginMapH;

    preview = new MapPreview("data/maps/7x7-01.map");
    preview->SetPosition(mapX, widgetY);
    mMapPreviews.emplace_back(preview);

    mapX = mapX0;
    widgetY += preview->GetHeight() + marginMapV;

    preview = new MapPreview("data/maps/10x10-01.map");
    preview->SetPosition(mapX, widgetY);
    mMapPreviews.emplace_back(preview);

    mapX += preview->GetWidth() + marginMapH;

    preview = new MapPreview("data/maps/001.map");
    preview->SetPosition(mapX, widgetY);
    mMapPreviews.emplace_back(preview);

    // -- NAVIGATION PANEL --
    const int marginButtonsH = 100;
    const int marginButtonsB = 25;

    Widget * panel = new Widget;

    // BUTTON BACK
    ButtonMainMenu * button = new ButtonMainMenu("BACK", panel);

    button->SetOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::MAIN_MENU);
    });

    const int buttonBackW = button->GetWidth();

    // -- BUTTON NEW GAME --
    button = new ButtonMainMenu("START", panel);
    button->SetX(buttonBackW + marginButtonsH);

    button->SetOnClickFunction([this, game]
    {
        // create human player
        game->AddPlayer("PLAYER 1", 0);
        Player * p = game->GetPlayer(0);
        p->SetLocal(true);

        // create AI players
        const char * strPlayers[] =
        {
            "PLAYER 2",
            "PLAYER 3",
            "PLAYER 4"
        };

        for(int i = 0; i < mCpuPlayers; ++i)
        {
            const int playerId = i + 1;

            game->AddPlayer(strPlayers[i], playerId);
            p = game->GetPlayer(playerId);
            auto * ai = new PlayerAI(p);
            p->SetAI(ai);
        }

        // set game difficulty
        game->SetDifficulty(mDiff);

        // move to game
        game->RequestNextActiveState(StateId::GAME);
    });

    // position buttons panel
    const int containerX = (Renderer::Instance()->GetWidth() - panel->GetWidth()) * 0.5f;
    const int containerY = Renderer::Instance()->GetHeight() - panel->GetHeight() - marginButtonsB;
    panel->SetPosition(containerX, containerY);
}

ScreenNewGame::~ScreenNewGame()
{
    lib::sgui::Stage::Instance()->ClearWidgets();
}

void ScreenNewGame::Update(float update)
{
}

void ScreenNewGame::Render()
{
}

} // namespace game
