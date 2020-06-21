#include "Screens/ScreenGame.h"

#include "Game.h"
#include "IsoMap.h"
#include "Player.h"
#include "Widgets/PanelPlayer.h"

#include <core/event/MouseButtonEvent.h>
#include <graphic/Renderer.h>
#include <sgui/Stage.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

namespace game
{

ScreenGame::ScreenGame(Game * game)
    : Screen(game)
{
    game->SetClearColor(0xE5, 0xE5, 0xE5, 0xFF);

    const int SIDE = 15;
    const int TILE_W = 128;

    const std::vector<std::string> tileFiles = {
                                                "data/img/tile00.png",
                                                "data/img/tile01.png",
                                                "data/img/tile02.png",
                                                "data/img/tile03.png",
                                                "data/img/tile04.png",
                                                "data/img/tile05.png"
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

    // -- UI --
    for(int i = 0; i < GetGame()->GetNumPlayers(); ++i)
    {
        PanelPlayer * panel = new PanelPlayer(GetGame()->GetPlayer(i));

        const int x = (i % 2) ? (rendW - panel->GetWidth() - 10) : 10;
        const int y = (i < 2) ? 10 : (rendH - panel->GetHeight() - 10);

        panel->SetPosition(x, y);

        panel->SetPanelCellVisible(false);
        panel->SetPanelUnitsVisible(false);
    }

    // -- GAMEPLAY TESTING --
    srand(time(nullptr));

    SetHomeCell();
}

ScreenGame::~ScreenGame()
{
    delete mMap;

    lib::sgui::Stage::Instance()->ClearWidgets();
}

void ScreenGame::Update()
{
}

void ScreenGame::Render()
{
    mMap->Render();
}

void ScreenGame::OnMouseButtonUp(const lib::core::MouseButtonEvent & event)
{
    const Cell2D c = mMap->CellFromScreenPoint(event.GetX(), event.GetY());

    int cellType = mMap->GetCellType(c.row, c.col);

    std::cout << "Point " << event.GetX() << "," << event.GetY() << " = "
              << "cell " << c.row << "," << c.col << " - type: " << cellType << std::endl;
}

void ScreenGame::SetHomeCell()
{
    const int NUM_CORNERS = 4;
    Cell2D corners[NUM_CORNERS] = { {0, 0}, {0, 14}, {14, 0}, {14, 14} };

    const int pick = rand() % NUM_CORNERS;

    mMap->SetCellType(corners[pick].row, corners[pick].col, 1);
}

} // namespace game
