#include "Screens/ScreenGame.h"

#include "Game.h"
#include "IsoMap.h"
#include "Player.h"

#include <core/event/MouseButtonEvent.h>
#include <graphic/Font.h>
#include <graphic/Image.h>
#include <graphic/Renderer.h>
#include <sgui/Label.h>
#include <sgui/PushButton.h>

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
        lib::sgui::Widget * panel = CreatePlayerUI(GetGame()->GetPlayer(i));

        const int x = (i % 2) ? (rendW - panel->GetWidth() - 10) : 10;
        const int y = (i < 2) ? 10 : (rendH - panel->GetHeight() - 10);

        panel->SetPosition(x, y);
    }

    // -- GAMEPLAY TESTING --
    srand(time(nullptr));

    SetHomeCell();
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

void ScreenGame::OnMouseButtonUp(const lib::core::MouseButtonEvent & event)
{
    const Cell2D c = mMap->CellFromScreenPoint(event.GetX(), event.GetY());

    int cellType = mMap->GetCellType(c.row, c.col);

    std::cout << "Point " << event.GetX() << "," << event.GetY() << " = "
              << "cell " << c.row << "," << c.col << " - type: " << cellType << std::endl;
}

lib::sgui::Widget *ScreenGame::CreatePlayerUI(Player * p)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    int wid = 0;

    Widget * panel = new Widget(wid++);

    Font fontName("data/fonts/OpenSans.ttf", 48);
    fontName.SetStyle(Font::BOLD);

    Font fontHeader("data/fonts/OpenSans.ttf", 28);

    Font fontData("data/fonts/OpenSans.ttf", 40);

    Label * labelName = new Label(p->GetName().c_str(), wid++, &fontName, panel);
    labelName->SetColor(33, 33, 33, 255);

    const int marginX0 = 80;
    const int marginX1 = 50;

    const int Y0 = 10;

    Label * labelHeader1 = new Label("COINS", wid++, &fontHeader, panel);
    labelHeader1->SetPosition(labelName->GetX() + labelName->GetWidth() + marginX0, Y0);
    labelHeader1->SetColor(33, 33, 33, 255);

    Label * labelData1 = new Label("000", wid++, &fontData, panel);
    labelData1->SetPosition(labelHeader1->GetX() + (labelHeader1->GetWidth() - labelData1->GetWidth()) * 0.5f,
                            labelHeader1->GetY() + labelHeader1->GetHeight());
    labelData1->SetColor(33, 33, 33, 255);

    Label * labelHeader2 = new Label("CELLS", wid++, &fontHeader, panel);
    labelHeader2->SetPosition(labelHeader1->GetX() + labelHeader1->GetWidth() + marginX1, Y0);
    labelHeader2->SetColor(33, 33, 33, 255);

    Label * labelData2 = new Label("000", wid++, &fontData, panel);
    labelData2->SetPosition(labelHeader2->GetX() + (labelHeader2->GetWidth() - labelData2->GetWidth()) * 0.5f,
                            labelHeader2->GetY() + labelHeader2->GetHeight());
    labelData2->SetColor(33, 33, 33, 255);

    Label * labelHeader3 = new Label("UNITS", wid++, &fontHeader, panel);
    labelHeader3->SetPosition(labelHeader2->GetX() + labelHeader2->GetWidth() + marginX1, Y0);
    labelHeader3->SetColor(33, 33, 33, 255);

    Label * labelData3 = new Label("000", wid++, &fontData, panel);
    labelData3->SetPosition(labelHeader3->GetX() + (labelHeader3->GetWidth() - labelData3->GetWidth()) * 0.5f,
                            labelHeader3->GetY() + labelHeader3->GetHeight());
    labelData3->SetColor(33, 33, 33, 255);

    return panel;
}

void ScreenGame::SetHomeCell()
{
    const int NUM_CORNERS = 4;
    Cell2D corners[NUM_CORNERS] = { {0, 0}, {0, 14}, {14, 0}, {14, 14} };

    const int pick = rand() % NUM_CORNERS;

    mMap->SetCellType(corners[pick].row, corners[pick].col, 1);
}

} // namespace game
