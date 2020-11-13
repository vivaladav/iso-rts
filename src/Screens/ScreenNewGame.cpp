#include "Screens/ScreenNewGame.h"

#include "Game.h"
#include "Player.h"
#include "AI/PlayerAI.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"
#include "Widgets/ButtonUnitsSelector.h"

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
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    game->SetClearColor(0xEE, 0xEE, 0xEE, 0xFF);

    const int marginL = 50;
    const int marginT = 50;
    const int marginTitleB = 50;
    const int marginWidgetsV = 100;
    const int marginWidgetsH = 100;
    int widgetY = marginT;

    // -- TITLE --
    FontManager * fm = FontManager::Instance();

    Font * fontTitle = fm->GetFont("data/fonts/OpenSans.ttf", 48);
    fontTitle->SetStyle(Font::BOLD);

    Label * title = new Label("SINGLE PLAYER GAME", fontTitle);
    title->SetColor(0x111111FF);
    title->SetPosition(marginL, widgetY);

    widgetY += title->GetHeight() + marginTitleB;

    // -- CPU PLAYERS --
    const unsigned int colorHeader = 0x333333FF;
    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 32);
    fontTitle->SetStyle(Font::BOLD);

    Label * header = new Label("CPU PLAYERS", fontHeader);
    header->SetColor(colorHeader);
    header->SetPosition(marginL, widgetY);

    // buttons
    auto * bg = new ButtonsGroup(ButtonsGroup::HORIZONTAL);
    bg->SetPosition(header->GetX() + header->GetWidth() + marginWidgetsH, widgetY);

    bg->AddButton(new ButtonUnitsSelector("1"));
    bg->AddButton(new ButtonUnitsSelector("2"));
    bg->AddButton(new ButtonUnitsSelector("3"));

    bg->SetButtonChecked(0, true);

    bg->SetFunctionOnToggle([this](unsigned int ind, bool checked)
    {
        std::cout << "ButtonsGroup button " << ind << " " << (checked ? " CHECKED" : "UNCHECKED") << std::endl;
    });

    widgetY += header->GetHeight() + marginWidgetsV;

    // -- DIFFICULTY --
    header = new Label("DIFFICULTY", fontHeader);
    header->SetColor(colorHeader);
    header->SetPosition(marginL, widgetY);

    widgetY += header->GetHeight() + marginWidgetsV;

    // -- MAP --
    header = new Label("MAP", fontHeader);
    header->SetColor(colorHeader);
    header->SetPosition(marginL, widgetY);

    widgetY += header->GetHeight() + marginWidgetsV;

    // -- NAVIGATION PANEL --
    const int marginButtonsH = 100;
    const int marginButtonsB = 100;

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

    button->SetOnClickFunction([game]
    {
        game->AddPlayer("PLAYER 1", 0);
        Player * p = game->GetPlayer(0);
        p->SetLocal(true);

        // AI player
        game->AddPlayer("PLAYER 2", 1);
        p = game->GetPlayer(1);
        auto * ai = new PlayerAI(p);
        p->SetAI(ai);

        // AI player
        game->AddPlayer("PLAYER 3", 2);
        p = game->GetPlayer(2);
        ai = new PlayerAI(p);
        p->SetAI(ai);

        // AI player
        game->AddPlayer("PLAYER 4", 3);
        p = game->GetPlayer(3);
        ai = new PlayerAI(p);
        p->SetAI(ai);

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
