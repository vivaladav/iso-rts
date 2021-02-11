#include "Screens/ScreenMainMenu.h"

#include "Game.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"

#include <graphic/Image.h>
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

    game->SetClearColor(0x12, 0x12, 0x12, 0xFF);

    Widget * panel = new Widget;

    int buttonY = 0;
    const int VMARGIN = 100;

    const int screenW = Renderer::Instance()->GetWidth();

    // -- TITLE --
    mLogo = new lib::graphic::Image("data/img/logo.png");

    const int logoX = (screenW - mLogo->GetWidth()) * 0.5f;
    const int logoY = 0;
    mLogo->SetPosition(logoX, logoY);

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
    const int containerX = (screenW - panel->GetWidth()) * 0.5f;
    const int containerY = mLogo->GetY() + mLogo->GetHeight() + VMARGIN;
    panel->SetPosition(containerX, containerY);
}

ScreenMainMenu::~ScreenMainMenu()
{
    lib::sgui::Stage::Instance()->ClearWidgets();

    delete mLogo;
}

void ScreenMainMenu::Update(float update)
{
}

void ScreenMainMenu::Render()
{
    mLogo->Render();
}

} // namespace game
