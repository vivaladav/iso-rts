#include "Screens/ScreenGame.h"

#include "Game.h"
#include "GameConstants.h"
#include "GameMap.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "MapLoader.h"
#include "Player.h"
#include "Unit.h"
#include "AI/PlayerAI.h"
#include "Widgets/CellProgressBar.h"
#include "Widgets/PanelGameOver.h"
#include "Widgets/PanelGameWon.h"
#include "Widgets/PanelPlayer.h"

#include <core/event/KeyboardEvent.h>
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
    game->SetClearColor(0x0F, 0x0F, 0x0F, 0xFF);

    game->AddKeyboardListener(this);

    // -- ISOMETRIC MAP --
    CreateIsoMap();
    CreateLayers();

    // create game map
    mGameMap = new GameMap(GetGame(), this, mIsoMap);

    // load map file
    const std::string & mapFile = game->GetCurrentMapFile();
    MapLoader ml(mGameMap, mIsoMap);
    ml.Load(mapFile);

    // center map on screen
    const int mapH = mIsoMap->GetHeight();

    const int rendW = lib::graphic::Renderer::Instance()->GetWidth();
    const int rendH = lib::graphic::Renderer::Instance()->GetHeight();

    mIsoMap->SetOrigin(rendW * 0.5, (rendH - mapH) * 0.5);

    const int playerFaction = game->GetPlayer(0)->GetPlayerId();

    // create selection object
    mIsoMap->GetLayer(SELECTION)->AddObject(0, 0, playerFaction, ObjectAlignment::NO_ALIGNMENT);

    // set homes
    mGameMap->SetHomeCells();

    // -- PLAYERS --
    for(int i = 0; i < GetGame()->GetNumPlayers(); ++i)
    {
        Player * p = game->GetPlayer(i);

        // add start money
        p->SumMoney(START_ENERGY);

        // temporary disable AI for development
//        if(p->IsAI())
//        {
//            p->GetAI()->SetGameMap(mGameMap);
//            mAiPlayers.push_back(p);
//        }
    }

    // -- UI --
    Player * player = game->GetPlayer(0);
    mPanelPlayer = new PanelPlayer(player);

    // setup data update functions
    player->SetOnMoneyChanged([this](int money)
    {
        mPanelPlayer->UpdateCoins(money);
    });

    player->SetOnNumCellsChanged([this](int cells)
    {
        mPanelPlayer->UpdateCells(cells);
    });

    player->SetOnNumUnitsChanged([this](int units)
    {
        mPanelPlayer->UpdateUnits(units);
    });

    // -- UI actions --
    // CONQUEST CELL
    mPanelPlayer->SetFunctionCellConquest([this, player]
    {
       SetupCellConquest(player->GetSelectedCell(), player);

       // clear selection
       ClearSelection(player);
    });

    // CREATE NEW UNIT
    mPanelPlayer->SetFunctionNewUnit([this, player]
    {
        SetupNewUnit(player->GetSelectedCell(), player);

        // clear selection
        ClearSelection(player);
    });

    // UNIT DESTROY
    mPanelPlayer->SetFunctionUnitsDestroy([this, player]
    {
       SetupUnitDestroy(player->GetSelectedCell(), player);

       ClearSelection(player);
    });

    // UNIT UPGRADE
    mPanelPlayer->SetFunctionUnitsUpgrade([this, player]
    {
        SetupUnitUpgrade(player->GetSelectedCell(), player);

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
    // do nothing when paused
    if(mPaused)
        return ;

    // -- UPDATE COINS --
    mTimerEnergy -= delta;

    if(mTimerEnergy < 0.f)
    {
        Game * game = GetGame();

        for(int i = 0; i < game->GetNumPlayers(); ++i)
        {
            Player * p = game->GetPlayer(i);

            const int energyProd = p->GetEnergyProduction();
            const int energyUsed = p->GetEnergyUse();
            const int energyDiff = energyProd - energyUsed;
            p->SumMoney(energyDiff);
        }

        mTimerEnergy = TIME_ENERGY_USE;
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

void ScreenGame::CreateIsoMap()
{
    const int TILE_W = 96;

    const std::vector<std::string> tileFiles =
    {
        // scene
        "data/img/tile_empty.png",
        "data/img/tile_fow.png",
        "data/img/tile_scene_01.png",

        // faction 1
        "data/img/tile-f1.png",
        "data/img/tile-f1_connected.png",
        "data/img/tile-f1_influenced.png",

        // faction 2
        "data/img/tile-f2.png",
        "data/img/tile-f2_connected.png",
        "data/img/tile-f2_influenced.png",

        // faction 3
        "data/img/tile-f3.png",
        "data/img/tile-f3_connected.png",
        "data/img/tile-f3_influenced.png",
    };

    // iso map
    mIsoMap = new IsoMap(TILE_W);
    mIsoMap->SetTiles(tileFiles);
}

void ScreenGame::CreateLayers()
{
    // -- LAYERS --
    // SELECTION
    const std::vector<std::string> selImgs =
    {
        "data/img/selection-f1.png",
        "data/img/selection-f2.png",
        "data/img/selection-f3.png"
    };

    mIsoMap->CreateLayer(MapLayers::SELECTION, selImgs);
    mIsoMap->SetLayerVisible(SELECTION, false);

    // MOVE TARGETS
    const std::vector<std::string> mtImgs = { "data/img/move_target.png" };
    mIsoMap->CreateLayer(MapLayers::MOVE_TARGETS, mtImgs);

    // OBJECTS
    const std::vector<std::string> objImgs =
    {
        "data/img/obj_null.png",

        // FACTION 1
        "data/img/unit_01-f1.png",

        // FACTION 2
        "data/img/unit_01-f2.png",

        // FACTION 3
        "data/img/unit_01-f3.png",

        // ENERGY SOURCE
        "data/img/energy_source.png",
        "data/img/energy_source-f1.png",
        "data/img/energy_source-f2.png",
        "data/img/energy_source-f3.png",
    };

    mIsoMap->CreateLayer(MapLayers::OBJECTS, objImgs);
}

void ScreenGame::OnKeyUp(lib::core::KeyboardEvent & event)
{
    using namespace lib::core;

    const int key = event.GetKey();

    // P -> PAUSE
    if(key == KeyboardEvent::KEY_P)
    {
        mPaused = !mPaused;

        // disable player panel when paused
        mPanelPlayer->SetEnabled(!mPaused);
    }
}

void ScreenGame::OnMouseButtonUp(lib::core::MouseButtonEvent & event)
{
    if(mPaused)
        return ;

    // handle only LEFT click
    if(event.GetButton() != lib::core::MouseEvent::BUTTON_LEFT)
        return ;

    const Cell2D currSel = mIsoMap->CellFromScreenPoint(event.GetX(), event.GetY());

    const bool insideMap = mIsoMap->IsCellInside(currSel);

    Player * player = GetGame()->GetPlayer(0);

    if(insideMap)
    {
        const GameMapCell & gameCell = mGameMap->GetCell(currSel.row, currSel.col);
        const Player * owner = gameCell.owner;
        const bool isLocalPlayer = owner == player;
        const Unit * cellUnit = gameCell.GetUnit();
        const bool isPlayerUnit = cellUnit != nullptr && cellUnit->GetOwner() == player->GetPlayerId();
        const bool canSelect = (isLocalPlayer || isPlayerUnit) && !mGameMap->IsCellChanging(currSel.row, currSel.col);

        // 1 cell previously selected
        if(player->HasSelectedCell())
        {
            const Cell2D & prevSel = player->GetSelectedCell();

            const Unit * unit = mGameMap->GetCell(prevSel.row, prevSel.col).GetUnit();
            const int unitsToMove = unit ? unit->GetNumElements() : 0;

            const bool diffSel = prevSel.row != currSel.row || prevSel.col != currSel.col;

            // has units to move and it's selecting a different cell
            if(unitsToMove > 0 && diffSel)
            {
                // move successful -> select new cell
                if(mGameMap->MoveUnits(prevSel, currSel, unitsToMove, player))
                    SelectCell(currSel, player);
                // move failed
                else
                {
                    // try to conquest a resource generator
                    if(SetupResourceGeneratorConquest(prevSel, currSel, player))
                        ClearSelection(player);
                    // try to select the cell
                    else if(canSelect)
                        SelectCell(currSel, player);
                    // all failed, clear the selection
                    else
                        ClearSelection(player);
                }
            }
            else
            {
                if(canSelect)
                {
                    if(diffSel)
                        SelectCell(currSel, player);
                    else
                        ClearSelection(player);
                }
                else
                    ClearSelection(player);
            }
        }
        // no cell previously selected
        else
        {
            if(canSelect)
                SelectCell(currSel, player);
        }
    }
    // click outside the map
    else
    {
        // clear the previous selection, if any
        if(player->HasSelectedCell())
            ClearSelection(player);
    }
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

    mPanelPlayer->ClearSelectedCell();

    mIsoMap->SetLayerVisible(SELECTION, false);

    mIsoMap->GetLayer(MOVE_TARGETS)->ClearObjects();
}

void ScreenGame::SelectCell(const Cell2D & cell, Player * player)
{
    player->SetSelectedCell(cell);
    const GameMapCell & gameCell = mGameMap->GetCell(cell.row, cell.col);

    mPanelPlayer->SetSelectedCell(gameCell);

    IsoLayer * layerSel = mIsoMap->GetLayer(SELECTION);
    layerSel->MoveObject(mPrevSel.row, mPrevSel.col, cell.row, cell.col, NO_ALIGNMENT);
    mIsoMap->SetLayerVisible(SELECTION, true);

    // show move targets if it's player's unit
    const Unit * cellUnit = gameCell.GetUnit();

    if(cellUnit != nullptr && cellUnit->GetOwner() == player->GetPlayerId())
    {
        mIsoMap->GetLayer(MOVE_TARGETS)->ClearObjects();
        ShowMoveTargets(cell, player);
    }

    // store selection cell
    mPrevSel = cell;
}

void ScreenGame::ShowMoveTargets(const Cell2D & cell, Player * player)
{
    IsoLayer * layerTargets = mIsoMap->GetLayer(MOVE_TARGETS);

    const int numTargets = 8;
    const Cell2D targets[numTargets] =
    {
        {cell.row - 1, cell.col - 1},     // TL
        {cell.row - 1, cell.col},         // T
        {cell.row - 1, cell.col + 1},     // TR

        {cell.row, cell.col - 1},         // L
        {cell.row, cell.col + 1},         // R

        {cell.row + 1, cell.col - 1},     // BL
        {cell.row + 1, cell.col},         // B
        {cell.row + 1, cell.col + 1}      // BR
    };

    // add targets where unit can move
    for(int i = 0; i < numTargets; ++i)
    {
        if(mGameMap->CanUnitMove(cell, targets[i], player))
            layerTargets->AddObject(targets[i].row, targets[i].col, 0, ObjectAlignment::CENTER);
    }
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

bool ScreenGame::SetupCellConquest(const Cell2D & cell, Player * player)
{
    // check if conquest is possible
    if(!mGameMap->CanConquestCell(cell, player))
        return false;

    // start conquest
    mGameMap->StartConquestCell(cell, player);

    // create and init progress bar
    CellProgressBar * pb = CreateProgressBar(cell, TIME_CONQ_CELL, player->GetPlayerId());

    pb->SetFunctionOnCompleted([this, cell, player]
    {
        mGameMap->ConquestCell(cell, player);
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

bool ScreenGame::SetupResourceGeneratorConquest(const Cell2D & start, const Cell2D & end, Player * player)
{
    // check if conquest is possible
    if(!mGameMap->CanConquestResourceGenerator(start, end, player))
        return false;

    // start conquest
    mGameMap->StartConquestResourceGenerator(start, end, player);

    // create and init progress bar
    CellProgressBar * pb = CreateProgressBar(start, TIME_CONQ_RES_GEN, player->GetPlayerId());

    pb->SetFunctionOnCompleted([this, start, end, player]
    {
        mGameMap->ConquestResourceGenerator(start, end, player);
        mProgressBarsToDelete.emplace_back(CellToIndex(start));
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
