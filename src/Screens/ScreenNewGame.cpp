#include "Screens/ScreenNewGame.h"

#include "Game.h"
#include "Player.h"
#include "AI/PlayerAI.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"

#include <graphic/Renderer.h>
#include <sgui/PushButton.h>
#include <sgui/Stage.h>

namespace game
{

ScreenNewGame::ScreenNewGame(Game * game)
    : Screen(game)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    game->SetClearColor(0x11, 0x11, 0x11, 0xFF);

    Widget * panel = new Widget(0);

    int buttonY = 0;
    const int VMARGIN = 100;

    // -- BUTTON NEW GAME --
    ButtonMainMenu * button = new ButtonMainMenu("START", panel);

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

    buttonY += button->GetHeight() + VMARGIN;

    // -- BUTTON BACK --
    button = new ButtonMainMenu("BACK", panel);
    button->SetY(buttonY);

    button->SetOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::MAIN_MENU);
    });

    buttonY += button->GetHeight() + VMARGIN;

    // position buttons panel
    const int containerX = (Renderer::Instance()->GetWidth() - panel->GetWidth()) * 0.5f;
    const int containerY = 300;
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
