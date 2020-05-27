#include "Screens/ScreenGame.h"

#include "Game.h"
#include "IsoMap.h"
#include "Screens/ScreenIds.h"

#include <core/event/KeyboardEvent.h>
#include <core/event/MouseButtonEvent.h>
#include <graphic/Image.h>
#include <graphic/Renderer.h>
#include <utilities/StateManager.h>

#include <iostream>

namespace game
{

ScreenGame::ScreenGame(Game * game)
    : Screen(ScreenId::GAME, game)
{
    mMap = new IsoMap(15, 15);
    mMap->AddTile("data/img/tile02.png");
    mMap->AddTile("data/img/tile03.png");
    mMap->AddTile("data/img/tile04.png");

    // center map on screen
    const int mapW = mMap->GetWidth();
    const int mapH = mMap->GetHeight();

    const int rendW = lib::graphic::Renderer::Instance()->GetWidth();
    const int rendH = lib::graphic::Renderer::Instance()->GetHeight();

    mMap->SetOrigin(rendW - (mapW * 0.5), (rendH - mapH) * 0.5);

    //mMap->SetOrigin(0, 0);
}

ScreenGame::~ScreenGame()
{
    delete mMap;
}

void ScreenGame::Update()
{
}

void ScreenGame::Render()
{
    mMap->Render();
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
    const Cell2D c = mMap->TileFromScreenPoint(event.GetX(), event.GetY());

    std::cout << "Point " << event.GetX() << "," << event.GetY() << " = "
              << "cell " << c.row << "," << c.col << std::endl;
}

void ScreenGame::OnActive()
{
    Screen::OnActive();

    mGame->SetClearColor(0x33, 0x33, 0x33, 0xFF);
}

} // namespace game
