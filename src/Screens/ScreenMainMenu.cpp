#include "Screens/ScreenMainMenu.h"

#include "Game.h"
#include "Screens/ScreenIds.h"

#include <graphic/Renderer.h>
#include <sgui/PushButton.h>
#include <utilities/StateManager.h>

namespace game
{

ScreenMainMenu::ScreenMainMenu(Game * game)
    : Screen(ScreenId::MAIN_MENU, game)
{
    const int VMARGIN = 100;

    // -- BUTTON NEW GAME --
    mButtonNew = new lib::sgui::PushButton(0);
    mButtonNew->SetBackground("data/img/buttons/new_game-01.png");

    const int buttonX = (lib::graphic::Renderer::Instance()->GetWidth() - mButtonNew->GetWidth()) * 0.5f;
    int buttonY = 300;

    mButtonNew->SetPosition(buttonX, buttonY);

    mButtonNew->SetOnClickFunction([this] { GetStateManager()->RequestNextActiveState(ScreenId::GAME); });

    GetGame()->AddEventListener(mButtonNew);

    buttonY += mButtonNew->GetHeight() + VMARGIN;

    // -- BUTTON EXIT --
    mButtonExit = new lib::sgui::PushButton(0);
    mButtonExit->SetBackground("data/img/buttons/exit-01.png");

    mButtonExit->SetPosition(buttonX, buttonY);

    mButtonExit->SetOnClickFunction([this] { GetGame()->Exit(); });

    GetGame()->AddEventListener(mButtonExit);
}

ScreenMainMenu::~ScreenMainMenu()
{
    delete mButtonNew;
    delete mButtonExit;
}

void ScreenMainMenu::Update()
{
}

void ScreenMainMenu::Render()
{
    mButtonNew->Render();
    mButtonExit->Render();
}

void ScreenMainMenu::OnActive()
{
    Screen::OnActive();

    GetGame()->SetClearColor(0x11, 0x11, 0x11, 0xFF);
}

} // namespace game
