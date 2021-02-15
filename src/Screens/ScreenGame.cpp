#include "Screens/ScreenGame.h"

#include "Game.h"
#include "GameConstants.h"
#include "GameMap.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "MapLoader.h"
#include "Player.h"
#include "AI/ConquerPath.h"
#include "AI/ObjectPath.h"
#include "AI/PlayerAI.h"
#include "GameObjects/Unit.h"
#include "Widgets/CellProgressBar.h"
#include "Widgets/PanelGameOver.h"
#include "Widgets/PanelGameWon.h"
#include "Widgets/PanelPlayer.h"

#include <ai/Pathfinder.h>
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

// NOTE these will be replaced by dynamic values soon
constexpr float TIME_NEW_UNIT = 2.f;
constexpr float TIME_CONQ_RES_GEN = 2.f;
constexpr float TIME_UPG_UNIT = 1.f;

constexpr float TIME_ENERGY_USE = 2.f;
constexpr float TIME_AI_MOVE = 0.5f;

ScreenGame::ScreenGame(Game * game)
    : Screen(game)
    , mPathfinder(new lib::ai::Pathfinder)
    , mTimerEnergy(TIME_ENERGY_USE)
    , mTimerAI(TIME_AI_MOVE)
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

    // init pathfinder
    mPathfinder->SetMap(mGameMap, mGameMap->GetNumRows(), mGameMap->GetNumCols());
    mPathfinder->SetAllowDiagonals(false);

    // -- PLAYERS --
    for(int i = 0; i < GetGame()->GetNumPlayers(); ++i)
    {
        Player * p = game->GetPlayer(i);

        // add start resources
        const int startEnergy = 150;
        p->SumEnergy(startEnergy);

        const int startMaterial = 10;
        p->SumMaterial(startMaterial);

        // temporary disable AI for development
//        if(p->IsAI())
//        {
//            p->GetAI()->SetGameMap(mGameMap);
//            mAiPlayers.push_back(p);
//        }
    }

    // apply initial visibility to the game map
    mGameMap->ApplyVisibility(game->GetPlayer(0));

    // -- UI --
    Player * player = game->GetPlayer(0);
    mPanelPlayer = new PanelPlayer(player);

    // setup data update functions
    player->SetOnEnergyChanged([this](int val)
    {
        mPanelPlayer->UpdateEnergy(val);
    });

    player->SetOnMaterialChanged([this](int val)
    {
        mPanelPlayer->UpdateMaterial(val);
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
    // CONQUER CELL
    mPanelPlayer->SetFunctionCellConquest([this]
    {
        Player * player = GetGame()->GetLocalPlayer();
        GameObject * obj = player->GetSelectedObject();

        if(obj != nullptr && obj->GetObjectType() == OBJ_UNIT)
        {
            auto unit = static_cast<Unit *>(obj);
            unit->SetActiveAction(UnitAction::CONQUER);
        }
    });

    // CREATE NEW UNIT
    mPanelPlayer->SetFunctionNewUnit([this, player]
    {
        SetupNewUnit(player->GetSelectedObject(), player);

        // clear selection
        ClearSelection(player);
    });

    // UNIT UPGRADE
    mPanelPlayer->SetFunctionUnitsUpgrade([this, player]
    {
        SetupUnitUpgrade(player->GetSelectedObject(), player);

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

    // -- UPDATE PLAYERS RESOURCES --
    mTimerEnergy -= delta;

    if(mTimerEnergy < 0.f)
    {
        Game * game = GetGame();

        for(int i = 0; i < game->GetNumPlayers(); ++i)
        {
            Player * p = game->GetPlayer(i);
            p->UpdateResources();
        }

        mTimerEnergy = TIME_ENERGY_USE;
    }

    // -- GAME MAP AND OBJECTS --
    mGameMap->Update(delta);

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

void ScreenGame::CreateProgressBar(const Cell2D & cell, float time, Player * player,
                                  const std::function<void()> & onCompleted)
{
    CellProgressBar * pb = CreateProgressBar(cell, time, player->GetPlayerId());

    // TODO make widgets accept multiple callback functions so that this code can be changed
    // returning the ProgressBar and letting the caller set another callback while the
    // callback handling the deletion is added here
    pb->SetFunctionOnCompleted([this, cell, onCompleted]
    {
        onCompleted();

        mProgressBarsToDelete.emplace_back(CellToIndex(cell));
    });
}

void ScreenGame::CreateIsoMap()
{
    const int TILE_W = 96;

    const std::vector<std::string> tileFiles =
    {
        // scene
        "data/img/tile_empty.png",
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

        // fog of war
        "data/img/tile_fow.png",
    };

    // iso map
    mIsoMap = new IsoMap(TILE_W);
    mIsoMap->SetTiles(tileFiles);
}

void ScreenGame::CreateLayers()
{
    mIsoMap->CreateLayer(MapLayers::SELECTION);

    mIsoMap->CreateLayer(MapLayers::MOVE_TARGETS);

    mIsoMap->CreateLayer(MapLayers::OBJECTS);
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
    else if(key == KeyboardEvent::KEY_V)
    {
        Player * p = GetGame()->GetLocalPlayer();

        // SHIFT + V -> add visibility to all cells
        if(event.IsModShiftDown())
        {
            p->AddVisibilityToAll();
            mGameMap->ApplyVisibility(p);
        }
        // CTRL + V -> remove visibility from all cells
        else if(event.IsModCtrlDown())
        {
            p->RemVisibilityToAll();
            mGameMap->ApplyVisibility(p);
        }
    }
}

void ScreenGame::OnMouseButtonUp(lib::core::MouseButtonEvent & event)
{
    // no interaction while game is paused
    if(mPaused)
        return ;

    // handle only LEFT click
    if(event.GetButton() != lib::core::MouseEvent::BUTTON_LEFT)
        return ;

    Player * player = GetGame()->GetLocalPlayer();

    const Cell2D clickCell = mIsoMap->CellFromScreenPoint(event.GetX(), event.GetY());
    const bool insideMap = mIsoMap->IsCellInside(clickCell);

    const bool hasSelected = player->HasSelectedObject();

    // clicked outside the map
    if(!insideMap)
    {
        // clear the previous selection, if any
        if(hasSelected)
            ClearSelection(player);

        return ;
    }

    const GameMapCell & clickGameCell = mGameMap->GetCell(clickCell.row, clickCell.col);
    GameObject * clickObj = clickGameCell.obj;
    const bool isClickObjOwn = clickObj != nullptr && clickObj->GetOwner() == player->GetPlayerId();

    if(hasSelected)
    {
        GameObject * selObj = player->GetSelectedObject();
        const Cell2D selCell(selObj->GetRow0(), selObj->GetCol0());
        const bool diffClick = selCell.row != clickCell.row  || selCell.col != clickCell.col;

        // select another own object
        if(isClickObjOwn && clickObj != selObj)
        {
            ClearSelection(player);
            SelectObject(clickObj, player);

            return ;
        }

        // selected object is a unit
        if(selObj->GetObjectType() == OBJ_UNIT)
        {
            Unit * selUnit = static_cast<Unit *>(selObj);

            const UnitAction action = selUnit->GetActiveAction();

            // move
            if(action == UnitAction::MOVE)
            {
                // click cell is different from unit cell -> try to move
                if(diffClick)
                {
                    // destination is walkable -> try to generate a path
                    if(mGameMap->IsCellWalkable(clickCell.row, clickCell.col))
                    {
                        auto path = mPathfinder->MakePath(selCell.row, selCell.col,
                                                          clickCell.row, clickCell.col);

                        // path available -> start moving
                        if(!path.empty())
                        {
                            auto op = new ObjectPath(selUnit, mIsoMap, mGameMap);
                            op->SetPathCells(path);

                            mGameMap->MoveUnit(op);
                        }
                    }
                    // destination not walkable
                    else
                    {
                        // destination object is visible -> try to interact
                        if(clickObj != nullptr && clickObj->IsVisible())
                        {
                            // object is adjacent
                            if(mGameMap->AreObjectsAdjacent(selObj, clickObj))
                            {
                                if(SetupResourceGeneratorConquest(selCell, clickCell, player))
                                    ClearSelection(player);
                            }
                            // object is far
                            else
                            {
                                Cell2D target = mGameMap->GetAdjacentMoveTarget(selCell, clickObj);

                                if(target.row != -1 && target.col != -1)
                                {
                                    auto path = mPathfinder->MakePath(selCell.row, selCell.col,
                                                                      target.row, target.col);

                                    // path available -> start moving
                                    if(!path.empty())
                                    {
                                        auto op = new ObjectPath(selUnit, mIsoMap, mGameMap);
                                        op->SetPathCells(path);

                                        auto fun = [this, selUnit, clickCell, player]
                                        {

                                            const Cell2D selCell(selUnit->GetRow0(), selUnit->GetCol0());

                                            if(SetupResourceGeneratorConquest(selCell, clickCell, player))
                                                ClearSelection(player);
                                        };

                                        op->SetOnCompleted(fun);

                                        mGameMap->MoveUnit(op);
                                    }
                                }
                            }
                        }
                        // no destination object or object not visible -> try to walk close to destination
                        else
                        {
                            Cell2D target = mGameMap->GetCloseMoveTarget(selCell, clickCell);

                            if(target.row != -1 && target.col != -1)
                            {
                                auto path = mPathfinder->MakePath(selCell.row, selCell.col,
                                                                  target.row, target.col);

                                // path available -> start moving
                                if(!path.empty())
                                {
                                    auto op = new ObjectPath(selUnit, mIsoMap, mGameMap);
                                    op->SetPathCells(path);

                                    mGameMap->MoveUnit(op);
                                }
                            }
                        }
                    }
                }
            }
            else if(action == UnitAction::CONQUER)
            {
                const int clickInd = clickCell.row * mGameMap->GetNumCols() + clickCell.col;

                // destination is visible and walkable
                if(player->IsCellVisible(clickInd) &&
                   (mGameMap->IsCellWalkable(clickCell.row, clickCell.col) || clickCell == selCell))
                {
                    auto path = mPathfinder->MakePath(selCell.row, selCell.col,
                                                      clickCell.row, clickCell.col);

                    // path available -> start conquering
                    if(!path.empty())
                    {

                        auto cp = new ConquerPath(selUnit, mIsoMap, mGameMap, this);
                        cp->SetPathCells(path);

                        mGameMap->ConquerCells(cp);
                    }
                }
            }
        }
    }
    else
    {
        if(isClickObjOwn)
            SelectObject(clickObj, player);
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
    player->ClearSelectedObject();

    mPanelPlayer->ClearSelectedCell();

    mIsoMap->GetLayer(MOVE_TARGETS)->ClearObjects();
}

void ScreenGame::SelectObject(GameObject * obj, Player * player)
{
    obj->SetSelected(true);

    player->SetSelectedObject(obj);

    mPanelPlayer->SetSelectedObject(obj);
}

void ScreenGame::ShowMoveTargets(const Cell2D & cell, Player * player)
{
    /*
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
    */
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
//                done = SetupNewUnit(action.src, player);
                done = true;
            }
            break;

            case ACT_UNIT_UPGRADE:
            {
                std::cout << "AI " << mCurrPlayerAI << " - UNIT UPGRADE" << std::endl;
                //done = SetupUnitUpgrade(action.obj, player);
                done = true;
            }
            break;

            case ACT_UNIT_MOVE:
            {
                std::cout << "AI " << mCurrPlayerAI << " - MOVE UNIT: from "
                          << action.src.row << "," << action.src.col
                          << " -> "
                          << action.dst.row << "," << action.dst.col
                          << std::endl;

                // TODO move unit
                done = false;
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

bool ScreenGame::SetupNewUnit(GameObject * gen, Player * player)
{
    // check if create is possible
    if(!mGameMap->CanCreateUnit(gen, player))
        return false;

    Cell2D cell = mGameMap->GetNewUnitDestination(gen);

    if(-1 == cell.row || -1 == cell.col)
    {
        std::cerr << "GameMap::GetNewUnitDestination FAILED" << std::endl;
        return false;
    }

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
    if(!mGameMap->CanConquerResourceGenerator(start, end, player))
        return false;

    // start conquest
    mGameMap->StartConquerResourceGenerator(start, end, player);

    // create and init progress bar
    CellProgressBar * pb = CreateProgressBar(start, TIME_CONQ_RES_GEN, player->GetPlayerId());

    pb->SetFunctionOnCompleted([this, start, end, player]
    {
        mGameMap->ConquerResourceGenerator(start, end, player);
        mProgressBarsToDelete.emplace_back(CellToIndex(start));
    });

    return true;
}

bool ScreenGame::SetupUnitUpgrade(GameObject * obj, Player * player)
{
    // check if upgrade is possible
    if(!mGameMap->CanUpgradeUnit(obj, player))
        return false;

    // start upgrade
    mGameMap->StartUpgradeUnit(obj, player);

    const Cell2D cell(obj->GetRow0(), obj->GetCol0());

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
