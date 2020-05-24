#include "Screens/ScreenMainMenu.h"

#include "Game.h"
#include "Screens/ScreenIds.h"

#include <core/event/KeyboardEvent.h>
#include <core/event/MouseButtonEvent.h>
#include <graphic/Image.h>
#include <graphic/Window.h>
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

void ScreenMainMenu::Render()
{
}

void ScreenMainMenu::OnApplicationQuit() { mGame->Exit(); }

void ScreenMainMenu::OnKeyUp(const lib::core::KeyboardEvent & event)
{
    using namespace lib::core;
    using namespace lib::graphic;

    const int key = event.GetKey();

    if(key == KeyboardEvent::KEY_ESC)
        mGame->Exit();
    else if(key == KeyboardEvent::KEY_G)
        GetStateManager()->RequestNextActiveState(ScreenId::GAME);
    else if(key == KeyboardEvent::KEY_T)
        GetStateManager()->RequestNextActiveState(ScreenId::TEST);
    else if(key == KeyboardEvent::KEY_F)
        Window::Instance()->SwitchFullscreen();
    else if(key == KeyboardEvent::KEY_1)
        Window::Instance()->SetSize(960, 540);
    else if(key == KeyboardEvent::KEY_2)
        Window::Instance()->SetSize(1280, 720);
    else if(key == KeyboardEvent::KEY_3)
        Window::Instance()->SetSize(1600, 900);
    else if(key == KeyboardEvent::KEY_4)
        Window::Instance()->SetSize(1920, 1080);
}

void ScreenMainMenu::OnMouseButtonUp(const lib::core::MouseButtonEvent & event)
{
}

void ScreenMainMenu::OnActive()
{
    Screen::OnActive();

    mGame->SetClearColor(0x11, 0x11, 0x11, 0xFF);
}

} // namespace game
