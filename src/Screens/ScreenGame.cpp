#include "Screens/ScreenGame.h"

#include "Game.h"
#include "Screens/ScreenIds.h"

#include <core/event/KeyboardEvent.h>
#include <core/event/MouseButtonEvent.h>
#include <graphic/Image.h>
#include <utilities/StateManager.h>

#include <iostream>

namespace game
{

ScreenGame::ScreenGame(Game * game)
    : Screen(ScreenId::GAME, game)
{

}

ScreenGame::~ScreenGame()
{

}

void ScreenGame::Update()
{
}

void ScreenGame::OnApplicationQuit() { mGame->Exit(); }

void ScreenGame::OnKeyUp(const lib::core::KeyboardEvent & event)
{
    using namespace lib::core;

    const int key = event.GetKey();

    if(key == KeyboardEvent::KEY_ESC)
        mGame->Exit();
    else if(key == KeyboardEvent::KEY_M)
        GetStateManager()->RequestNextActiveState(ScreenId::MAIN_MENU);
    else if(key == KeyboardEvent::KEY_T)
        GetStateManager()->RequestNextActiveState(ScreenId::TEST);
}

void ScreenGame::OnMouseButtonUp(const lib::core::MouseButtonEvent & event)
{
}

void ScreenGame::OnActive()
{
    Screen::OnActive();

    mGame->SetClearColor(0x33, 0x33, 0x33, 0xFF);
}

} // namespace game
