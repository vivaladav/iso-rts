#include "Screens/ScreenMainMenu.h"

#include "Game.h"
#include "Screens/ScreenIds.h"

#include <core/event/KeyboardEvent.h>
#include <core/event/MouseButtonEvent.h>
#include <graphic/Image.h>
#include <utilities/StateManager.h>

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
    using namespace lib::core;

    const int key = event.GetKey();

    if(key == KeyboardEvent::KEY_ESC)
        mGame->Exit();
    else if(key == KeyboardEvent::KEY_T)
        GetStateManager()->RequestNextActiveState(ScreenId::TEST);
}

void ScreenMainMenu::OnMouseButtonUp(const lib::core::MouseButtonEvent & event)
{
}

} // namespace game
