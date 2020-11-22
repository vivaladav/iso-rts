#include "Screens/ScreenGame.h"

#include "Game.h"
#include "GameConstants.h"
#include "GameMap.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "Player.h"
#include "AI/PlayerAI.h"
#include "Widgets/CellProgressBar.h"
#include "Widgets/PanelGameOver.h"
#include "Widgets/PanelGameWon.h"
#include "Widgets/PanelPlayer.h"

#include <core/event/MouseButtonEvent.h>
#include <graphic/Renderer.h>
#include <sgui/Stage.h>

#include <algorithm>
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

    // -- ISOMETRIC MAP --
    const int SIDE = 15;
    const int TILE_W = 128;

    const std::vector<std::string> tileFiles =
    {
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
        "data/img/tile-p4l1.png",
        "data/img/tile-p4l2.png",
        "data/img/tile-p4l3.png",
        "data/img/tile-p4l4.png"
    };

    mIsoMap = new IsoMap(SIDE, SIDE, TILE_W);
    mIsoMap->SetTiles(tileFiles);

    // center map on screen
    const int mapH = mIsoMap->GetHeight();

    const int rendW = lib::graphic::Renderer::Instance()->GetWidth();
    const int rendH = lib::graphic::Renderer::Instance()->GetHeight();

    mIsoMap->SetOrigin(rendW * 0.5, (rendH - mapH) * 0.5);

    // -- LAYERS --
    // FORTIFICATIONS
    const std::vector<std::string> fortImgs =
    {
        "data/img/fort01.png",
        "data/img/fort02.png",
        "data/img/fort03.png"
    };

    mIsoMap->CreateLayer(fortImgs);

    // SELECTION
    const std::vector<std::string> selImgs = { "data/img/selection.png" };

    IsoLayer * layer = mIsoMap->CreateLayer(selImgs);
    layer->AddObject(mPrevSel.row, mPrevSel.col, 0, NO_ALIGNMENT);

    mIsoMap->SetLayerVisible(SELECTION, false);

    // UNITS
    const std::vector<std::string> unitsImgs =
    {
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
        "data/img/unit2-p2l1.png",
        "data/img/unit3-p2l1.png",
        "data/img/unit4-p2l1.png",
        // PLAYER 2 - LEVEL 2
        "data/img/unit1-p2l2.png",
        "data/img/unit2-p2l2.png",
        "data/img/unit3-p2l2.png",
        "data/img/unit4-p2l2.png",
        // PLAYER 2 - LEVEL 3
        "data/img/unit1-p2l3.png",
        "data/img/unit2-p2l3.png",
        "data/img/unit3-p2l3.png",
        "data/img/unit4-p2l3.png",

        // PLAYER 3 - LEVEL 1
        "data/img/unit1-p3l1.png",
        "data/img/unit2-p3l1.png",
        "data/img/unit3-p3l1.png",
        "data/img/unit4-p3l1.png",
        // PLAYER 3 - LEVEL 2
        "data/img/unit1-p3l2.png",
        "data/img/unit2-p3l2.png",
        "data/img/unit3-p3l2.png",
        "data/img/unit4-p3l2.png",
        // PLAYER 3 - LEVEL 3
        "data/img/unit1-p3l3.png",
        "data/img/unit2-p3l3.png",
        "data/img/unit3-p3l3.png",
        "data/img/unit4-p3l3.png",

        // PLAYER 4 - LEVEL 1
        "data/img/unit1-p4l1.png",
        "data/img/unit2-p4l1.png",
        "data/img/unit3-p4l1.png",
        "data/img/unit4-p4l1.png",
        // PLAYER 4 - LEVEL 2
        "data/img/unit1-p4l2.png",
        "data/img/unit2-p4l2.png",
        "data/img/unit3-p4l2.png",
        "data/img/unit4-p4l2.png",
        // PLAYER 4 - LEVEL 3
        "data/img/unit1-p4l3.png",
        "data/img/unit2-p4l3.png",
        "data/img/unit3-p4l3.png",
        "data/img/unit4-p4l3.png"
    };

    mIsoMap->CreateLayer(unitsImgs);

    // -- GAME MAP --
    mGameMap = new GameMap(game, this, mIsoMap);
    mGameMap->Load("data/maps/001.map");
    mGameMap->SetHomeCell();

    // -- PLAYERS --
    for(int i = 0; i < GetGame()->GetNumPlayers(); ++i)
    {
        Player * p = game->GetPlayer(i);

        // add start money
        p->SumMoney(START_MONEY);

        if(p->IsAI())
        {
            p->GetAI()->SetGameMap(mGameMap);
            mAiPlayers.push_back(p);
        }
    }

    // -- UI --
    const PanelPlayer::PanelPosition panelPos[] =
    {
        PanelPlayer::PPOS_TL,
        PanelPlayer::PPOS_TR,
        PanelPlayer::PPOS_BL,
        PanelPlayer::PPOS_BR,
    };

    for(int i = 0; i < GetGame()->GetNumPlayers(); ++i)
    {
        Player * player = GetGame()->GetPlayer(i);
        PanelPlayer * panel = new PanelPlayer(player, panelPos[i]);

        const int x = (i % 2) ? (rendW - panel->GetWidth()) : 0;
        const int y = (i < 2) ? 0 : (rendH - panel->GetHeight());

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
        SetupCellFortify(*(player->GetSelectedCell()), player);

        // clear selection
        ClearSelection(player);
    });

    // UPGRADE CELL
    panel->SetFunctionCellUpgrade([this, player]
    {
        SetupCellUpgrade(*(player->GetSelectedCell()), player);

        // clear selection
        ClearSelection(player);
    });

    // CREATE NEW UNIT
    panel->SetFunctionNewUnit([this, player]
    {
        SetupNewUnit(*(player->GetSelectedCell()), player);

        // clear selection
        ClearSelection(player);
    });

    // UNIT DESTROY
    panel->SetFunctionUnitsDestroy([this, player]
    {
       SetupUnitDestroy(*(player->GetSelectedCell()), player);

       ClearSelection(player);
    });

    // UNIT UPGRADE
    panel->SetFunctionUnitsUpgrade([this, player]
    {
        SetupUnitUpgrade(*(player->GetSelectedCell()), player);

        // clear selection
        ClearSelection(player);
    });
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
    // -- UPDATE COINS --
    mTimerCoins -= delta;

    if(mTimerCoins < 0.f)
    {
        Game * game = GetGame();

        for(int i = 0; i < game->GetNumPlayers(); ++i)
        {
            Player * p = game->GetPlayer(i);

            const int coins = p->GetTotalCellsLevel() * COINS_PER_CELL - p->GetTotalUnitsLevel() * COST_PER_UNIT;
            p->SumMoney(coins);
        }

        mTimerCoins = TIME_COINS_GEN;
    }

    // -- PROGRESS BARS --
    UpdateProgressBars(delta);

    // -- AI --
    if(!mAiPlayers.empty())
        UpdateAI(delta);
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

void ScreenGame::GameWon()
{
    const int rendW = lib::graphic::Renderer::Instance()->GetWidth();
    const int rendH = lib::graphic::Renderer::Instance()->GetHeight();

    auto panel = new PanelGameWon(GetGame());

    const int x = (rendW - panel->GetWidth()) * 0.5f;
    const int y = rendH * 0.25f;

    panel->SetPosition(x, y);
}

void ScreenGame::CancelProgressBar(const Cell2D & cell)
{
    const int cellInd = CellToIndex(cell);

    // delete progress bar
    auto it = mProgressBars.find(cellInd);

    if(it != mProgressBars.end())
    {
        delete it->second;
        mProgressBars.erase(it);
    }

    // remove index from bars to delete (unlikely, but just in case)
    auto it2 = std::find(mProgressBarsToDelete.begin(), mProgressBarsToDelete.end(), cellInd);

    if(it2 != mProgressBarsToDelete.end())
        mProgressBarsToDelete.erase(it2);
}

void ScreenGame::OnMouseButtonUp(lib::core::MouseButtonEvent & event)
{
    const Cell2D c = mIsoMap->CellFromScreenPoint(event.GetX(), event.GetY());

    const bool insideMap = mIsoMap->IsCellInside(c);

    Player * player = GetGame()->GetPlayer(0);
    PanelPlayer * panel = mPanelsPlayer[0];

    if(insideMap)
    {
        const Cell2D * selCell = player->GetSelectedCell();

        if(selCell)
        {
            const int unitsToMove = mGameMap->GetCell(selCell->row, selCell->col).units;

            if(unitsToMove > 0)
                mGameMap->MoveUnits(*player->GetSelectedCell(), c, unitsToMove, player);

            ClearSelection(player);
        }
        else
        {
            const Player * owner = mGameMap->GetCellOwner(c.row, c.col);
            const bool isLocalPlayer = owner == player;

            // own cell and not already changing
            if(isLocalPlayer && !mGameMap->IsCellChanging(c.row, c.col))
            {
                player->SetSelectedCell(c);
                panel->SetSelectedCell(mGameMap->GetCell(c.row, c.col));

                IsoLayer * layerSel = mIsoMap->GetLayer(SELECTION);
                layerSel->MoveObject(mPrevSel.row, mPrevSel.col, c.row, c.col, NO_ALIGNMENT);
                mIsoMap->SetLayerVisible(SELECTION, true);

                // store selection cell
                mPrevSel = c;
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
    auto pb = new CellProgressBar(playerId, 0.f, time);
    pb->SetValue(0.f);
    auto posCell = mIsoMap->GetCellPosition(cell.row, cell.col);
    const int pbX = posCell.x + (mIsoMap->GetTileWidth() - pb->GetWidth()) * 0.5f;
    const int pbY = posCell.y + (mIsoMap->GetTileHeight() * 0.75f - pb->GetHeight());
    pb->SetPosition(pbX, pbY);

    const int cellInd = CellToIndex(cell);
    mProgressBars.emplace(cellInd, pb);

    return pb;
}

void ScreenGame::UpdateProgressBars(float delta)
{
    for(auto pb : mProgressBars)
        pb.second->IncValue(delta);

    // delete progress bars when they finish
    while(!mProgressBarsToDelete.empty())
    {
        const int cellInd = mProgressBarsToDelete.back();
        mProgressBarsToDelete.pop_back();

        auto it = mProgressBars.find(cellInd);

        if(it != mProgressBars.end())
        {
            delete it->second;
            mProgressBars.erase(it);
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

void ScreenGame::UpdateAI(float delta)
{
    mTimerAI -= delta;

    if(mTimerAI < 0.f)
    {
        // reset current player if needed
        if(mCurrPlayerAI >= MAX_NUM_PLAYERS)
            mCurrPlayerAI = 0;

        // current index pooints to an actual AI player
        if(mCurrPlayerAI < mAiPlayers.size())
        {
            PlayerAI * ai = mAiPlayers[mCurrPlayerAI]->GetAI();
            ExecuteAIAction(ai);
        }

        // move to next player and update timer
        ++mCurrPlayerAI;
        mTimerAI = TIME_AI_MOVE;
    }
}

void ScreenGame::ExecuteAIAction(PlayerAI * ai)
{
    Player * player = ai->GetPlayer();
    ai->DecideActions();

    bool done = false;

    // execute planned action until one is successful or there's no more actions to do (NOP)
    while(!done)
    {
        const ActionAI action = ai->GetNextAction();

        switch(action.aid)
        {
            case ACT_CELL_FORTIFY:
            {
                std::cout << "AI " << mCurrPlayerAI << " - FORTIFY CELL" << std::endl;
                done = SetupCellFortify(action.src, player);
            }
            break;

            case ACT_CELL_UPGRADE:
            {
                std::cout << "AI " << mCurrPlayerAI << " - UPGRADE CELL" << std::endl;
                done = SetupCellUpgrade(action.src, player);
            }
            break;

            case ACT_NEW_UNIT:
            {
                std::cout << "AI " << mCurrPlayerAI << " - NEW UNIT" << std::endl;
                done = SetupNewUnit(action.src, player);
            }
            break;

            case ACT_UNIT_UPGRADE:
            {
                std::cout << "AI " << mCurrPlayerAI << " - UNIT UPGRADE" << std::endl;
                done = SetupUnitUpgrade(action.src, player);
            }
            break;

            case ACT_UNIT_DESTROY:
            {
                std::cout << "AI " << mCurrPlayerAI << " - UNIT DESTROY" << std::endl;
                done = SetupUnitDestroy(action.src, player);
            }
            break;

            case ACT_UNIT_MOVE:
            {
                std::cout << "AI " << mCurrPlayerAI << " - MOVE UNIT: "
                          << action.units << " from "
                          << action.src.row << "," << action.src.col
                          << " -> "
                          << action.dst.row << "," << action.dst.col
                          << std::endl;

                done = mGameMap->MoveUnits(action.src, action.dst, action.units, player);
            }
            break;

            case ACT_NOP:
                std::cout << "AI " << mCurrPlayerAI << " - NOP" << std::endl;
                done = true;
            break;

            default:
                std::cout << "AI " << mCurrPlayerAI << " - unkown action" << action.aid << std::endl;
            break;
        }
    }
}

int ScreenGame::CellToIndex(const Cell2D & cell) const
{
    return cell.row * mIsoMap->GetNumCols() + cell.col;
}

bool ScreenGame::SetupCellFortify(const Cell2D & cell, Player * player)
{
    // check if fortify is possible
    if(!mGameMap->CanFortifyCell(cell, player))
        return false;

    // start fortify
    mGameMap->StartFortifyCell(cell, player);

    // create and init progress bar
    CellProgressBar * pb = CreateProgressBar(cell, TIME_FORT_CELL, player->GetPlayerId());

    pb->SetFunctionOnCompleted([this, cell]
    {
        mGameMap->FortifyCell(cell);
        mProgressBarsToDelete.emplace_back(CellToIndex(cell));
    });

    return true;
}

bool ScreenGame::SetupCellUpgrade(const Cell2D & cell, Player * player)
{
    // check if upgrade is possible
    if(!mGameMap->CanUpgradeCell(cell, player))
        return false;

    // start upgrade
    mGameMap->StartUpgradeCell(cell, player);

    // create and init progress bar
    CellProgressBar * pb = CreateProgressBar(cell, TIME_UPG_CELL, player->GetPlayerId());

    pb->SetFunctionOnCompleted([this, cell, player]
    {
        mGameMap->UpgradeCell(cell, player);
        mProgressBarsToDelete.emplace_back(CellToIndex(cell));
    });

    return true;
}

bool ScreenGame::SetupNewUnit(const Cell2D & cell, Player * player)
{
    // check if create is possible
    if(!mGameMap->CanCreateUnit(cell, player))
        return false;

    // start create
    mGameMap->StartCreateUnit(cell, player);

    // create and init progress bar
    CellProgressBar * pb = CreateProgressBar(cell, TIME_NEW_UNIT, player->GetPlayerId());

    pb->SetFunctionOnCompleted([this, cell, player]
    {
        mGameMap->CreateUnit(cell, player);
        mProgressBarsToDelete.emplace_back(CellToIndex(cell));
    });

    return true;
}

bool ScreenGame::SetupUnitDestroy(const Cell2D & cell, Player * player)
{
    if(!mGameMap->CanDestroyUnit(cell, player))
        return false;

    mGameMap->DestroyUnit(cell, player);

    return true;
}

bool ScreenGame::SetupUnitUpgrade(const Cell2D & cell, Player * player)
{
    // check if upgrade is possible
    if(!mGameMap->CanUpgradeUnit(cell, player))
        return false;

    // start upgrade
    mGameMap->StartUpgradeUnit(cell, player);

    // create and init progress bar
    CellProgressBar * pb = CreateProgressBar(cell, TIME_UPG_UNIT, player->GetPlayerId());

    pb->SetFunctionOnCompleted([this, cell]
    {
        mGameMap->UpgradeUnit(cell);
        mProgressBarsToDelete.emplace_back(CellToIndex(cell));
    });

    return true;
}

} // namespace game
