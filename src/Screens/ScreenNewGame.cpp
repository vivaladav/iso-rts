#include "Screens/ScreenNewGame.h"

#include "Game.h"
#include "Player.h"
#include "AI/PlayerAI.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"
#include "Widgets/ButtonNavigation.h"
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

// margins
const int marginHeaderB = 20;
const int marginMapH = 174;
const int marginMapV = 60;

// maps
const int mapX0 = 125;
const int rows = 2;
const int cols = 3;
const int mapsPerScreen = rows * cols;

ScreenNewGame::ScreenNewGame(Game * game)
    : Screen(game)
    , mGame(game)
    , mDiff(Difficulty::EASY)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    game->SetClearColor(0xE5, 0xE5, 0xE5, 0xFF);

    const int marginL = 25;
    const int marginT = 10;
    const int marginTitleB = 50;
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
    mLabelMaps = new Label("MAP", fontHeader);
    mLabelMaps->SetColor(colorHeader);
    mLabelMaps->SetPosition(marginL, widgetY);

    // MAP PREVIEW
    const unsigned int numMaps = mGame->GetMapFiles().size();
    mMapPreviews.assign(numMaps, nullptr);

    ShowMapPreviews(mStartMap);

    // MAP NAVIGATION BUTTONS
    if(numMaps > mapsPerScreen)
    {
        const int rendW = lib::graphic::Renderer::Instance()->GetWidth();
        const int yc = mMapPreviews[0]->GetY() + mMapPreviews[0]->GetHeight() + marginMapV * 0.5f;

        mButtonNavL = new ButtonNavigation("<");
        mButtonNavL->SetOnClickFunction([this]
        {
            mStartMap -= mapsPerScreen;
            UpdateNavButtonsState();
            ShowMapPreviews(mStartMap);
        });
        mButtonNavL->SetPosition(marginL, yc - mButtonNavL->GetHeight() * 0.5f);

        mButtonNavR = new ButtonNavigation(">");
        mButtonNavR->SetOnClickFunction([this]
        {
            mStartMap += mapsPerScreen;
            UpdateNavButtonsState();
            ShowMapPreviews(mStartMap);
        });
        mButtonNavR->SetPosition(rendW - marginL - mButtonNavR->GetWidth(),
                                 yc - mButtonNavR->GetHeight() * 0.5f);

        UpdateNavButtonsState();
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

MapPreview * ScreenNewGame::GetMapPreview(unsigned int index)
{
    const std::vector<std::string> & mapFiles = mGame->GetMapFiles();

    if(index >= mapFiles.size())
        return nullptr;

    if(nullptr == mMapPreviews[index])
        mMapPreviews[index] = new MapPreview(mapFiles[index].c_str());

    return mMapPreviews[index];
}

void ScreenNewGame::ShowMapPreviews(unsigned int mapStart)
{
    // hide maps between 0 and start, if already created
    for(unsigned int m = 0; m < mapStart; ++m)
    {
        MapPreview * mp = mMapPreviews[m];

        if(mp)
            mp->SetVisible(false);
    }

    // show maps between start and start + mps
    const std::vector<std::string> & mapFiles = mGame->GetMapFiles();
    const unsigned int numMaps = mapFiles.size();

    int mapX = mapX0;
    int mapY = mLabelMaps->GetY() + mLabelMaps->GetHeight() + marginHeaderB;

    const int lastCol = cols - 1;
    const unsigned int end = mapStart + mapsPerScreen;
    const unsigned int mapEnd = numMaps < end ? numMaps : end;

    for(unsigned int m = mapStart; m < mapEnd; ++m)
    {
        MapPreview * preview = mMapPreviews[m];

        // create preview widget if not created yet
        if(nullptr == preview)
        {
            preview = new MapPreview(mapFiles[m].c_str());
            mMapPreviews[m] = preview;

            preview->SetPosition(mapX, mapY);
            preview->SetOnClickFunction([this, m]
            {
                if(mMapPreviews[m] != mMapPreviews[mMapSelInd])
                {
                    mMapPreviews[m]->SetSelected(true);

                    if(mMapSelInd != -1)
                        mMapPreviews[mMapSelInd]->SetSelected(false);

                    mMapSelInd = m;
                    mGame->SetCurrentMap(m);

                    mButtonStart->SetEnabled(true);
                }
            });
        }

        preview->SetVisible(true);

        const int c = m % cols;

        if(c < lastCol)
            mapX += preview->GetWidth() + marginMapH;
        else
        {
            mapX = mapX0;
            mapY += preview->GetHeight() + marginMapV;
        }
    }

    // hide maps between start + mps and end, if already created
    for(unsigned int m = mapEnd; m < numMaps; ++m)
    {
        MapPreview * mp = mMapPreviews[m];

        if(mp)
            mp->SetVisible(false);
    }
}

void ScreenNewGame::UpdateNavButtonsState()
{
    const std::vector<std::string> & mapFiles = mGame->GetMapFiles();
    const unsigned int numMaps = mapFiles.size();

    // disable LEFT if start is < mapsPerScreen
    mButtonNavL->SetEnabled(mStartMap >= mapsPerScreen);
    // disable RIGHT if start + maps per screen > total maps
    mButtonNavR->SetEnabled(mStartMap + mapsPerScreen < numMaps);
}

} // namespace game