#include "Screens/ScreenMainMenu.h"

#include "Game.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"

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

    Widget * panel = new Widget();

    int buttonY = 0;
    const int VMARGIN = 100;

    // -- BUTTON NEW GAME --
    ButtonMainMenu * button = new ButtonMainMenu("NEW GAME", panel);

    button->SetOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::NEW_GAME);
    });

    buttonY += button->GetHeight() + VMARGIN;

    // -- BUTTON EXIT --
    button = new ButtonMainMenu("EXIT", panel);

    button->SetOnClickFunction([game] { game->Exit(); });

    button->SetY(buttonY);

    // position buttons panel
    const int containerX = (Renderer::Instance()->GetWidth() - panel->GetWidth()) * 0.5f;
    const int containerY = 300;
    panel->SetPosition(containerX, containerY);
}

ScreenMainMenu::~ScreenMainMenu()
{
    lib::sgui::Stage::Instance()->ClearWidgets();
}

void ScreenMainMenu::Update(float update)
{
}

void ScreenMainMenu::Render()
{
}

} // namespace game
