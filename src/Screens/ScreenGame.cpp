#include "Screens/ScreenGame.h"

#include "Cell2D.h"
#include "Game.h"
#include "GameConstants.h"
#include "GameMap.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "Player.h"
#include "Widgets/CellProgressBar.h"
#include "Widgets/PanelGameOver.h"
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
                                                // PLAYER 1 - LEVEL 1
                                                "data/img/unit1-p1l1.png",
                                                "data/img/unit2-p1l1.png",
                                                "data/img/unit3-p1l1.png",
                                                "data/img/unit4-p1l1.png",
                                                // PLAYER 1 - LEVEL 2
                                                "data/img/unit1-p1l2.png",
                                                "data/img/unit2-p1l2.png",
                                                "data/img/unit3-p1l2.png",
                                                "data/img/unit4-p1l2.png",
                                                // PLAYER 1 - LEVEL 3
                                                "data/img/unit1-p1l3.png",
                                                "data/img/unit2-p1l3.png",
                                                "data/img/unit3-p1l3.png",
                                                "data/img/unit4-p1l3.png",

                                                // PLAYER 2 - LEVEL 1
                                                "data/img/unit1-p2l1.png",
                                               };

    mIsoMap->CreateLayer(unitsImgs);

    // -- GAME MAP --
    mGameMap = new GameMap(game, this, mIsoMap);
    mGameMap->Load("data/maps/001.map");
    mGameMap->SetHomeCell();

    // testing
    Player * p1 = game->GetPlayer(1);
    mGameMap->AssignCell(Cell2D(1, 1), p1);
    mGameMap->AssignCell(Cell2D(13, 13), p1);
    mGameMap->AssignCell(Cell2D(13, 1), p1);
    mGameMap->AssignCell(Cell2D(1, 13), p1);

    mGameMap->AssignCell(Cell2D(1, 0), p1);
    mGameMap->CreateUnit(Cell2D(1, 0), p1);
    mGameMap->AssignCell(Cell2D(0, 13), p1);
    mGameMap->CreateUnit(Cell2D(0, 13), p1);
    mGameMap->AssignCell(Cell2D(13, 14), p1);
    mGameMap->CreateUnit(Cell2D(13, 14), p1);
    mGameMap->AssignCell(Cell2D(13, 0), p1);
    mGameMap->CreateUnit(Cell2D(13, 0), p1);

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

    // FORTIFY CELL
    panel->SetFunctionCellFortify([this, player]
    {
        const Cell2D cell = *(player->GetSelectedCell());

        // check if upgrade is possible
        if(!mGameMap->CanFortifyCell(cell, player))
            return ;

        // start upgrade
        mGameMap->StartFortifyCell(cell, player);

        // create and init progress bar
        CellProgressBar * pb = CreateProgressBar(cell, TIME_UPG_CELL, player->GetPlayerId());

        pb->SetFunctionOnCompleted([this, cell, pb]
        {
            mGameMap->FortifyCell(cell);

            mProgressBarsToDelete.push_back(pb->GetWidgetId());
        });

        // clear selection
        ClearSelection(player);
    });

    // UPGRADE CELL
    panel->SetFunctionCellUpgrade([this, player]
    {
        const Cell2D cell = *(player->GetSelectedCell());

        // check if upgrade is possible
        if(!mGameMap->CanUpgradeCell(cell, player))
            return ;

        // start upgrade
        mGameMap->StartUpgradeCell(cell, player);

        // create and init progress bar
        CellProgressBar * pb = CreateProgressBar(cell, TIME_UPG_CELL, player->GetPlayerId());

        pb->SetFunctionOnCompleted([this, cell, player, pb]
        {
            mGameMap->UpgradeCell(cell, player);

            mProgressBarsToDelete.push_back(pb->GetWidgetId());
        });

        // clear selection
        ClearSelection(player);
    });

    // CREATE NEW UNIT
    panel->SetFunctionNewUnit([this, player]
    {
        const Cell2D cell = *(player->GetSelectedCell());

        // check if create is possible
        if(!mGameMap->CanCreateUnit(cell, player))
            return ;

        // start create
        mGameMap->StartCreateUnit(cell, player);

        // create and init progress bar
        CellProgressBar * pb = CreateProgressBar(cell, TIME_NEW_UNIT, player->GetPlayerId());

        pb->SetFunctionOnCompleted([this, cell, player, pb]
        {
            mGameMap->CreateUnit(cell, player);

            mProgressBarsToDelete.push_back(pb->GetWidgetId());
        });

        // clear selection
        ClearSelection(player);
    });

    panel->SetFunctionUnitsUpgrade([this, panel, player]
    {
        const Cell2D cell = *(player->GetSelectedCell());

        // check if upgrade is possible
        if(!mGameMap->CanUpgradeUnit(cell, player))
            return ;

        // start upgrade
        mGameMap->StartUpgradeUnit(cell, player);

        // create and init progress bar
        CellProgressBar * pb = CreateProgressBar(cell, TIME_UPG_UNIT, player->GetPlayerId());

        pb->SetFunctionOnCompleted([this, cell, pb]
        {
            mGameMap->UpgradeUnit(cell);

            mProgressBarsToDelete.push_back(pb->GetWidgetId());
        });

        // clear selection
        player->ClearSelectedCell();
        panel->ClearSelectedCell();
        mIsoMap->SetLayerVisible(SELECTION, false);
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
    // -- UPDATE COINS --
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

    // -- PROGRESS BARS --
    UpdateProgressBars(delta);
}

void ScreenGame::Render()
{
    mIsoMap->Render();
}

void ScreenGame::GameOver()
{
    const int rendW = lib::graphic::Renderer::Instance()->GetWidth();
    const int rendH = lib::graphic::Renderer::Instance()->GetHeight();

    auto panel = new PanelGameOver(GetGame());

    const int x = (rendW - panel->GetWidth()) * 0.5f;
    const int y = rendH * 0.25f;

    panel->SetPosition(x, y);
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
        const int unitsToMove = panel->GetNumUnitsToMove();

        if(unitsToMove > 0)
        {
            mGameMap->MoveUnits(player->GetSelectedCell(), &c, unitsToMove, player);

            ClearSelection(player);
        }
        else
        {
            const int owner = mGameMap->GetCellOwner(c.row, c.col);
            const bool isLocalPlayer = owner == player->GetPlayerId();

            // own cell and not already changing
            if(isLocalPlayer && !mGameMap->IsCellChanging(c.row, c.col))
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
                ClearSelection(player);
        }
    }
    else
        ClearSelection(player);
}

CellProgressBar * ScreenGame::CreateProgressBar(const Cell2D & cell, float time, int playerId)
{
    const int barId = mProgressBars.size() + 1;
    auto pb = new CellProgressBar(playerId, 0.f, time);
    pb->SetValue(0.f);
    pb->SetWidgetId(barId);
    auto posCell = mIsoMap->GetCellPosition(cell.row, cell.col);
    const int pbX = posCell.x + (mIsoMap->GetTileWidth() - pb->GetWidth()) * 0.5f;
    const int pbY = posCell.y + (mIsoMap->GetTileHeight() * 0.75f - pb->GetHeight());
    pb->SetPosition(pbX, pbY);

    mProgressBars.emplace_back(pb);

    return pb;
}

void ScreenGame::UpdateProgressBars(float delta)
{
    for(auto pb : mProgressBars)
        pb->IncValue(delta);

    // delete progress bars when they finish
    while(!mProgressBarsToDelete.empty())
    {
        const int barId = mProgressBarsToDelete.back();
        mProgressBarsToDelete.pop_back();

        auto it = mProgressBars.begin();

        while(it != mProgressBars.end())
        {
            CellProgressBar * pb = *it;

            if(pb->GetWidgetId() == barId)
            {
                delete pb;
                it = mProgressBars.erase(it);
            }
            else
                ++it;
        }
    }
}

void ScreenGame::ClearSelection(Player * player)
{
    player->ClearSelectedCell();

    PanelPlayer * panel = mPanelsPlayer[player->GetPlayerId()];
    panel->ClearSelectedCell();

    mIsoMap->SetLayerVisible(SELECTION, false);
}

} // namespace game
