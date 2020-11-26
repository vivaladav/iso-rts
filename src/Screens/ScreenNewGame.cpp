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
#include <string>

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
    const std::vector<std::string> & mapFiles = game->GetMapFiles();

    const int marginMapH = 174; // change later when adding navigation button
    const int marginMapV = 60; // change later when adding navigation button
    const int mapX0 = 125;      // change later when adding navigation button
    int mapX = mapX0;

    const int rows = 2;
    const int cols = 3;
    const int lastCol = cols - 1;
    const int mapsPerScreen = rows * cols;
    const unsigned int end = mStartMap + mapsPerScreen;
    const unsigned int mapEnd = mapFiles.size() < end ? mapFiles.size() : end;

    for(unsigned int m = mStartMap; m < mapEnd; ++m)
    {
        MapPreview * preview = new MapPreview(mapFiles[m].c_str());
        preview->SetPosition(mapX, widgetY);
        preview->SetOnClickFunction([this, game, m]
        {
            if(mMapPreviews[m] != mMapPreviews[mMapSelInd])
            {
                mMapPreviews[m]->SetSelected(true);

                if(mMapSelInd != -1)
                    mMapPreviews[mMapSelInd]->SetSelected(false);

                mMapSelInd = m;
                game->SetCurrentMap(m);

                mButtonStart->SetEnabled(true);
            }
        });

        const int c = m % cols;

        if(c < lastCol)
            mapX += preview->GetWidth() + marginMapH;
        else
        {
            mapX = mapX0;
            widgetY += preview->GetHeight() + marginMapV;
        }

        mMapPreviews.emplace_back(preview);
    }

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
    mButtonStart = new ButtonMainMenu("START", panel);
    mButtonStart->SetEnabled(false);
    mButtonStart->SetX(buttonBackW + marginButtonsH);

    mButtonStart->SetOnClickFunction([this, game]
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
