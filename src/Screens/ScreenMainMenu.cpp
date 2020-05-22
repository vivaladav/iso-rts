#include "Screens/ScreenMainMenu.h"

#include "Game.h"
#include "Screens/ScreenIds.h"

#include <core/event/KeyboardEvent.h>
#include <core/event/MouseButtonEvent.h>
#include <graphic/Image.h>

#include <iostream>

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

void ScreenMainMenu::OnApplicationQuit() { mGame->Exit(); }

void ScreenMainMenu::OnKeyUp(const lib::core::KeyboardEvent & event)
{
    if(event.GetKey() == lib::core::KeyboardEvent::KEY_ESC)
        mGame->Exit();
}

void ScreenMainMenu::OnMouseButtonUp(const lib::core::MouseButtonEvent & event)
{

}

} // namespace game
