#include "Screens/ScreenMainMenu.h"

#include "Game.h"
#include "States/StatesIds.h"

#include <graphic/Renderer.h>
#include <sgui/PushButton.h>
#include <sgui/Stage.h>

namespace game
{

ScreenMainMenu::ScreenMainMenu(Game * game)
    : Screen(game)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    game->SetClearColor(0x11, 0x11, 0x11, 0xFF);

    int buttonId = 10;

    Widget * container = new Widget(0);

    int buttonY = 0;
    const int VMARGIN = 100;

    // -- BUTTON NEW GAME --
    mButtonNew = new PushButton(buttonId++, container);
    mButtonNew->SetBackground("data/img/buttons/new_game-01.png");

    mButtonNew->SetOnClickFunction([this]
    {
        GetGame()->RequestNextActiveState(StateId::GAME);
    });

    buttonY += mButtonNew->GetHeight() + VMARGIN;

    // -- BUTTON EXIT --
    mButtonExit = new PushButton(buttonId++, container);
    mButtonExit->SetBackground("data/img/buttons/exit-01.png");

    mButtonExit->SetOnClickFunction([this] { GetGame()->Exit(); });

    mButtonExit->SetY(buttonY);

    // position buttons panel
    const int containerX = (Renderer::Instance()->GetWidth() - mButtonNew->GetWidth()) * 0.5f;
    const int containerY = 300;
    container->SetPosition(containerX, containerY);
}

ScreenMainMenu::~ScreenMainMenu()
{
    lib::sgui::Stage::Instance()->ClearWidgets();
}

void ScreenMainMenu::Update()
{
}

void ScreenMainMenu::Render()
{
}

} // namespace game
