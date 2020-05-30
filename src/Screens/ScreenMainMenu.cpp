#include "Screens/ScreenMainMenu.h"

#include "Game.h"
#include "Screens/ScreenIds.h"

namespace game
{

ScreenMainMenu::ScreenMainMenu(Game * game)
    : Screen(ScreenId::MAIN_MENU, game)
{

}

ScreenMainMenu::~ScreenMainMenu()
{

}

void ScreenMainMenu::Update()
{
}

void ScreenMainMenu::Render()
{
}

void ScreenMainMenu::OnActive()
{
    Screen::OnActive();

    GetGame()->SetClearColor(0x11, 0x11, 0x11, 0xFF);
}

} // namespace game
