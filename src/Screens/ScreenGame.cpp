#include "Screens/ScreenGame.h"

#include "Cell2D.h"
#include "Game.h"
#include "GameConstants.h"
#include "GameMap.h"
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

    // TODO replace with C++11 random generation
    srand(time(nullptr));

    // -- ISOMETRIC MAP --
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

    mIsoMap = new IsoMap(SIDE, SIDE, TILE_W);
    mIsoMap->SetTiles(tileFiles);

    // center map on screen
    const int mapW = mIsoMap->GetWidth();
    const int mapH = mIsoMap->GetHeight();

    const int rendW = lib::graphic::Renderer::Instance()->GetWidth();
    const int rendH = lib::graphic::Renderer::Instance()->GetHeight();

    mIsoMap->SetOrigin(rendW - (mapW * 0.5), (rendH - mapH) * 0.5);

    // -- GAME MAP --
    mGameMap = new GameMap(mIsoMap, SIDE, SIDE);
    mGameMap->Load("data/maps/001.map");
    mGameMap->SetHomeCell(game);

    // -- PLAYERS --
    for(int i = 0; i < GetGame()->GetNumPlayers(); ++i)
    {
        Player * p = game->GetPlayer(i);

        // add start money
        p->SumMoney(START_MONEY);
    }

    // -- UI --
    for(int i = 0; i < GetGame()->GetNumPlayers(); ++i)
    {
        Player * player = GetGame()->GetPlayer(i);
        PanelPlayer * panel = new PanelPlayer(player);

        const int x = (i % 2) ? (rendW - panel->GetWidth() - 10) : 10;
        const int y = (i < 2) ? 10 : (rendH - panel->GetHeight() - 10);

        panel->SetPosition(x, y);

        panel->SetPanelCellVisible(false);
        panel->SetPanelUnitsVisible(false);

        // setup data update functions
        player->SetOnMoneyChanged([panel](int money)
        {
            panel->UpdateCoins(money);
        });

        player->SetOnNumCellsChanged([panel](int cells)
        {
            panel->UpdateCells(cells);
        });

        player->SetOnNumUnitsChanged([panel](int units)
        {
            panel->UpdateUnits(units);
        });

        mPanelsPlayer[i] = panel;
    }

    // UI actions
    Player * player = game->GetPlayer(0);
    PanelPlayer * panel = mPanelsPlayer[0];
    GameMap * gameMap = mGameMap;

    panel->SetFunctionCellFortify([] { std::cout << "CELL FORTIFY" << std::endl; });

    panel->SetFunctionCellUpgrade([gameMap, player]
    {
        std::cout << "CELL UPGRADE" << std::endl;

        gameMap->UpgradeCell(player->GetSelectedCell(), player);
    });

    panel->SetFunctionNewUnit([] { std::cout << "NEW UNIT" << std::endl; });
}

ScreenGame::~ScreenGame()
{
    delete mIsoMap;
    delete mGameMap;

    GetGame()->ClearPlayers();

    lib::sgui::Stage::Instance()->ClearWidgets();
}

void ScreenGame::Update(float delta)
{
    mTimerCoins -= delta;

    if(mTimerCoins < 0.f)
    {
        Game * game = GetGame();

        for(int i = 0; i < game->GetNumPlayers(); ++i)
        {
            Player * p = game->GetPlayer(i);

            const int coins = p->GetTotalCellsLevel() * COINS_PER_CELL;
            p->SumMoney(coins);
        }

        mTimerCoins = TIME_COINS_GEN;
    }
}

void ScreenGame::Render()
{
    mIsoMap->Render();
}

void ScreenGame::OnMouseButtonUp(lib::core::MouseButtonEvent & event)
{
    const Cell2D c = mIsoMap->CellFromScreenPoint(event.GetX(), event.GetY());

    const int cellType = mIsoMap->GetCellType(c.row, c.col);

    const bool insideMap = mIsoMap->IsCellInside(c);

    std::cout << "Point " << event.GetX() << "," << event.GetY() << " = "
              << "cell " << c.row << "," << c.col << " - type: " << cellType
              << " - INSIDE: "<< (insideMap ? "YES" : "NO") << std::endl;

    Player * player = GetGame()->GetPlayer(0);

    if(insideMap)
    {
        const int owner = mGameMap->GetCellOwner(c.row, c.col);



        const bool isLocalPlayer = owner == player->GetPlayerId();

        if(isLocalPlayer)
            player->SetSelectedCell(c);
        else
            player->ClearSelectedCell();

        mPanelsPlayer[0]->SetPanelCellVisible(isLocalPlayer);
    }
    else
    {
        mPanelsPlayer[0]->SetPanelCellVisible(false);

        player->ClearSelectedCell();
    }
}

} // namespace game
