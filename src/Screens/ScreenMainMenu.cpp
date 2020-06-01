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
    mButtonNew = new lib::sgui::PushButton(0);
    mButtonNew->SetBackground("data/img/buttons/new_game-01.png");

    const int buttonX = (lib::graphic::Renderer::Instance()->GetWidth() - mButtonNew->GetWidth()) * 0.5f;
    const int buttonY = 200;

    mButtonNew->SetPosition(buttonX, buttonY);

    mButtonNew->SetOnClickFunction([this] { GetStateManager()->RequestNextActiveState(ScreenId::GAME); });

    GetGame()->AddEventListener(mButtonNew);
}

ScreenMainMenu::~ScreenMainMenu()
{
    delete mButtonNew;
}

void ScreenMainMenu::Update()
{
}

void ScreenMainMenu::Render()
{
    mButtonNew->Render();
}

void ScreenMainMenu::OnActive()
{
    Screen::OnActive();

    GetGame()->SetClearColor(0x11, 0x11, 0x11, 0xFF);
}

} // namespace game
