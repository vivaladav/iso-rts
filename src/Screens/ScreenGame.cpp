#include "Screens/ScreenGame.h"

#include "Cell2D.h"
#include "Game.h"
#include "GameConstants.h"
#include "GameMap.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "Player.h"
#include "Widgets/PanelPlayer.h"

#include <core/event/MouseButtonEvent.h>
#include <graphic/Renderer.h>
#include <sgui/Stage.h>

#include <cstdlib>
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
                                                // scene
                                                "data/img/tile00.png",
                                                "data/img/tile01.png",
                                                // player 1
                                                "data/img/tile-p1l1.png",
                                                "data/img/tile-p1l2.png",
                                                "data/img/tile-p1l3.png",
                                                "data/img/tile-p1l4.png",
                                                // player 2
                                                "data/img/tile-p2l1.png",
                                                "data/img/tile-p2l2.png",
                                                "data/img/tile-p2l3.png",
                                                "data/img/tile-p2l4.png",
                                                // player 3
                                                "data/img/tile-p3l1.png",
                                                "data/img/tile-p3l2.png",
                                                "data/img/tile-p3l3.png",
                                                "data/img/tile-p3l4.png",
                                                // player 4
                                                "data/img/tile-p1l1.png",
                                                "data/img/tile-p1l2.png",
                                                "data/img/tile-p1l3.png",
                                                "data/img/tile-p1l4.png"
                                               };

    mIsoMap = new IsoMap(SIDE, SIDE, TILE_W);
    mIsoMap->SetTiles(tileFiles);

    // center map on screen
    const int mapW = mIsoMap->GetWidth();
    const int mapH = mIsoMap->GetHeight();

    const int rendW = lib::graphic::Renderer::Instance()->GetWidth();
    const int rendH = lib::graphic::Renderer::Instance()->GetHeight();

    mIsoMap->SetOrigin(rendW - (mapW * 0.5), (rendH - mapH) * 0.5);

    // -- LAYERS --
    // FORTIFICATIONS
    const std::vector<std::string> fortImgs = {
                                                "data/img/fort01.png",
                                                "data/img/fort02.png",
                                                "data/img/fort03.png"
                                              };

    mIsoMap->CreateLayer(fortImgs);

    // SELECTION
    mPrevSel = new Cell2D(0, 0);

    const std::vector<std::string> selImgs = { "data/img/selection.png" };

    IsoLayer * layer = mIsoMap->CreateLayer(selImgs);
    layer->AddObject(mPrevSel->row, mPrevSel->col, 0, NO_ALIGNMENT);

    mIsoMap->SetLayerVisible(SELECTION, false);

    // UNITS
    const std::vector<std::string> unitsImgs = {
                                                // PLAYER - LEVEL 1
                                                "data/img/unit1-p1l1.png",
                                                "data/img/unit2-p1l1.png",
                                                "data/img/unit3-p1l1.png",
                                                "data/img/unit4-p1l1.png",
                                                // PLAYER - LEVEL 2
                                                "data/img/unit1-p1l2.png",
                                                "data/img/unit2-p1l2.png",
                                                "data/img/unit3-p1l2.png",
                                                "data/img/unit4-p1l2.png",
                                                // PLAYER - LEVEL 3
                                                "data/img/unit1-p1l3.png",
                                                "data/img/unit2-p1l3.png",
                                                "data/img/unit3-p1l3.png",
                                                "data/img/unit4-p1l3.png",
                                               };

    mIsoMap->CreateLayer(unitsImgs);

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

    panel->SetFunctionCellFortify([gameMap, panel, player]
    {
        std::cout << "CELL FORTIFY" << std::endl;

        const Cell2D * cell = player->GetSelectedCell();

        gameMap->FortifyCell(cell, player);

        panel->UpdateButtonCellFortify(gameMap->GetCell(cell->row, cell->col).fortLevel);
    });

    panel->SetFunctionCellUpgrade([gameMap, panel, player]
    {
        std::cout << "CELL UPGRADE" << std::endl;

        const Cell2D * cell = player->GetSelectedCell();

        gameMap->UpgradeCell(cell, player);

        panel->UpdateButtonCellUpgrade(gameMap->GetCell(cell->row, cell->col).level);
    });

    panel->SetFunctionNewUnit([gameMap, panel, player]
    {
        std::cout << "NEW UNIT" << std::endl;

        const Cell2D * cell = player->GetSelectedCell();

        gameMap->NewUnit(cell, player);

        const GameMapCell & gameCell = gameMap->GetCell(cell->row, cell->col);

        panel->UpdateButtonNewUnit(gameCell.units, gameCell.unitsLevel);
    });
}

ScreenGame::~ScreenGame()
{
    delete mPrevSel;

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
    PanelPlayer * panel = mPanelsPlayer[0];

    if(insideMap)
    {
        const int owner = mGameMap->GetCellOwner(c.row, c.col);

        const bool isLocalPlayer = owner == player->GetPlayerId();

        const int unitsToMove = panel->GetNumUnitsToMove();

        if(unitsToMove > 0)
        {
            mGameMap->MoveUnits(player->GetSelectedCell(), &c, unitsToMove, player);

            panel->ClearNumUnitsToMove();
            panel->ClearSelectedCell();
            player->ClearSelectedCell();
            mIsoMap->SetLayerVisible(SELECTION, false);
        }
        else
        {
            if(isLocalPlayer)
            {
                player->SetSelectedCell(c);
                panel->SetSelectedCell(mGameMap->GetCell(c.row, c.col));

                IsoLayer * layerSel = mIsoMap->GetLayer(SELECTION);
                layerSel->MoveObject(mPrevSel->row, mPrevSel->col, c.row, c.col, NO_ALIGNMENT);
                mIsoMap->SetLayerVisible(SELECTION, true);

                // store selection cell
                *mPrevSel = c;
            }
            else
            {
                player->ClearSelectedCell();
                panel->ClearSelectedCell();
                mIsoMap->SetLayerVisible(SELECTION, false);
            }
        }
    }
    else
    {
        player->ClearSelectedCell();
        panel->ClearNumUnitsToMove();
        panel->ClearSelectedCell();
        mIsoMap->SetLayerVisible(SELECTION, false);
    }
}

} // namespace game
