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
#include <string>
#include <vector>

namespace game
{

ScreenGame::ScreenGame(Game * game)
    : Screen(ScreenId::GAME, game)
{
    const int SIDE = 15;
    const int TILE_W = 128;

    const std::vector<std::string> tileFiles = {
                                                "data/img/tile01.png",
                                                "data/img/tile02.png",
                                                "data/img/tile03.png",
                                                "data/img/tile04.png"
                                               };

    mMap = new IsoMap(SIDE, SIDE, TILE_W);
    mMap->SetTiles(tileFiles);
    mMap->Load("data/maps/001.map");

    // center map on screen
    const int mapW = mMap->GetWidth();
    const int mapH = mMap->GetHeight();

    const int rendW = lib::graphic::Renderer::Instance()->GetWidth();
    const int rendH = lib::graphic::Renderer::Instance()->GetHeight();

    mMap->SetOrigin(rendW - (mapW * 0.5), (rendH - mapH) * 0.5);
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

void ScreenGame::OnApplicationQuit() { GetGame()->Exit(); }

void ScreenGame::OnKeyUp(const lib::core::KeyboardEvent & event)
{
    using namespace lib::core;

    const int key = event.GetKey();

    if(key == KeyboardEvent::KEY_ESC)
        GetGame()->Exit();
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

    GetGame()->SetClearColor(0xAA, 0xAA, 0xAA, 0xFF);
}

} // namespace game
