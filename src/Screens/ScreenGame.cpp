﻿#include "Screens/ScreenGame.h"

#include "Game.h"
#include "GameConstants.h"
#include "GameData.h"
#include "GameMap.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "MapLoader.h"
#include "Player.h"
#include "AI/ConquerPath.h"
#include "AI/ObjectPath.h"
#include "AI/PlayerAI.h"
#include "AI/WallBuildPath.h"
#include "GameObjects/Unit.h"
#include "Indicators/ConquestIndicator.h"
#include "Indicators/MoveIndicator.h"
#include "Indicators/StructureIndicator.h"
#include "Indicators/WallIndicator.h"
#include "Particles/UpdaterSingleLaser.h"
#include "Widgets/CellProgressBar.h"
#include "Widgets/DialogNewUnit.h"
#include "Widgets/GameUIData.h"
#include "Widgets/PanelObjectActions.h"
#include "Widgets/PanelGameOver.h"
#include "Widgets/PanelGameWon.h"
#include "Widgets/PanelResources.h"

#include <ai/Pathfinder.h>
#include <core/event/KeyboardEvent.h>
#include <core/event/MouseButtonEvent.h>
#include <core/event/MouseMotionEvent.h>
#include <graphic/Camera.h>
#include <graphic/ParticlesManager.h>
#include <graphic/Renderer.h>
#include <graphic/TextureManager.h>
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
    , mPartMan(new lib::graphic::ParticlesManager)
    , mPathfinder(new lib::ai::Pathfinder)
    , mPrevCell(-1, -1)
    , mTimerEnergy(TIME_ENERGY_USE)
    , mTimerAI(TIME_AI_MOVE)
{
    game->SetClearColor(0x0F, 0x0F, 0x0F, 0xFF);

    game->AddKeyboardListener(this);

    const int rendW = lib::graphic::Renderer::Instance()->GetWidth();
    const int rendH = lib::graphic::Renderer::Instance()->GetHeight();

    mCamera = lib::graphic::Camera::GetDefaultCamera();

    InitSprites();

    InitParticlesSystem();

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

    mIsoMap->SetOrigin(rendW * 0.5, (rendH - mapH) * 0.5);

    // init pathfinder
    mPathfinder->SetMap(mGameMap, mGameMap->GetNumRows(), mGameMap->GetNumCols());

    // -- PLAYERS --
    for(int i = 0; i < GetGame()->GetNumPlayers(); ++i)
    {
        Player * p = game->GetPlayerByIndex(i);

        // add start resources
        const int startEnergy = 400;
        p->GetStat(Player::Stat::ENERGY).SetValue(startEnergy);

        const int startMaterial = 50;

        p->GetStat(Player::Stat::MATERIAL).SetValue(startMaterial);

        // temporary disable AI for development
//        if(p->IsAI())
//        {
//            p->GetAI()->SetGameMap(mGameMap);
//            mAiPlayers.push_back(p);
//        }
    }

    // LOCAL PLAYER
    Player * localPlayer = game->GetLocalPlayer();

    // react to local player changes in stats
    localPlayer->SetOnResourcesChanged([this]
    {
        if(mDialogNewUnit != nullptr)
            mDialogNewUnit->UpdateSlots();
    });

    // apply initial visibility to the game map
    mGameMap->ApplyLocalVisibility();

    // UI
    CreateUI();
}

ScreenGame::~ScreenGame()
{
    delete mPartMan;

    for(auto ind : mConquestIndicators)
        delete ind;

    for(auto ind : mWallIndicators)
        delete ind;

    for(auto it : mStructIndicators)
        delete it.second;

    delete mTempStructIndicator;

    delete mIsoMap;
    delete mGameMap;

    GetGame()->ClearPlayers();

    auto stage = lib::sgui::Stage::Instance();

    stage->ClearWidgets();
    // make sure to reset stage visibility in case it was off before exit
    stage->SetVisible(true);
}

void ScreenGame::Update(float delta)
{
    // handle Widgets scheduled for deletion
    for(auto w : mWidgetsToDelete)
        delete w;

    mWidgetsToDelete.clear();

    // do nothing when paused
    if(mPaused)
        return ;

    // -- UPDATE CAMERA --
    const float cameraSpeed = 400.f;

    if(mCameraDirX != 0)
        mCamera->MoveX(mCameraDirX * cameraSpeed * delta);

    if(mCameraDirY != 0)
        mCamera->MoveY(mCameraDirY * cameraSpeed * delta);

    // -- PARTICLES --
    mPartMan->Update(delta);

    // -- UPDATE PLAYERS RESOURCES --
    mTimerEnergy -= delta;

    if(mTimerEnergy < 0.f)
    {
        Game * game = GetGame();

        for(int i = 0; i < game->GetNumPlayers(); ++i)
        {
            Player * p = game->GetPlayerByIndex(i);
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

    mPartMan->Render();
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
    CellProgressBar * pb = CreateProgressBar(cell, time, player->GetFaction());

    // TODO make widgets accept multiple callback functions so that this code can be changed
    // returning the ProgressBar and letting the caller set another callback while the
    // callback handling the deletion is added here
    pb->SetFunctionOnCompleted([this, cell, onCompleted]
    {
        onCompleted();

        mProgressBarsToDelete.emplace_back(CellToIndex(cell));
    });
}

void ScreenGame::ClearObjectAction(GameObject * obj)
{
    auto it = mActiveObjActions.begin();

    // search selected object in active actions
    while(it != mActiveObjActions.end())
    {
        if(it->obj == obj)
        {
            mActiveObjActions.erase(it);
            return ;
        }

        ++it;
    }
}

lib::graphic::ParticlesUpdater * ScreenGame::GetParticleUpdater(ParticlesUpdaterId updaterId)
{
    return mPartMan->GetUpdater(updaterId);
}

void ScreenGame::InitSprites()
{
    auto tm = lib::graphic::TextureManager::Instance();

    int x = 0;
    int y = 0;
    int indRows = 0;
    int indCols = 0;
    int indW = 0;
    int indH = 0;

    // CELLS
    std::vector<lib::core::Rectd> rectsCells;
    indRows = 2;
    indCols = 9;
    indW = 96;
    indH = 48;

    x = 0;
    y = 0;

    for(int r = 0; r < indRows; ++r)
    {
        for(int c = 0; c < indCols; ++c)
        {
            rectsCells.emplace_back(x, y, indW, indH);

            x += indW;
        }

        x = 0;
        y += indH;
    }

    tm->RegisterSprite(SpriteFileCells, rectsCells);

    // COLLECTIBLES
    const std::vector<lib::core::Rectd> rectsColl
    {
        // diamonds
        { 0, 0, 96, 51 },
        { 96, 0, 96, 51 },
        { 192, 0, 96, 51 },
        { 288, 0, 96, 70 },

        // blobs
        { 0, 70, 96, 48 },
        { 96, 70, 96, 48 },
        { 192, 70, 96, 48 },
        { 288, 70, 96, 58 }
    };

    tm->RegisterSprite(SpriteCollectiblesFile, rectsColl);

    // INDICATORS
    std::vector<lib::core::Rectd> rectsInds;
    indRows = 4;
    indCols = 6;
    indW = 96;
    indH = 48;

    x = 0;
    y = 0;

    for(int r = 0; r < indRows; ++r)
    {
        for(int c = 0; c < indCols; ++c)
        {
            rectsInds.emplace_back(x, y, indW, indH);

            if(rectsInds.size() == NUM_IND_SPRITES)
                break;

            x += indW;
        }

        if(rectsInds.size() == NUM_IND_SPRITES)
            break;

        x = 0;
        y += indH;
    }

    tm->RegisterSprite(SpriteFileIndicators, rectsInds);

    // ROCKS
    const std::vector<lib::core::Rectd> rectsRocks
    {
        // row 0
        { 0, 0, 96, 51 },
        { 96, 0, 96, 54 },
        { 192, 0, 96, 54 },
        { 288, 0, 96, 51 },
        { 384, 0, 96, 56 },
        { 480, 0, 96, 55 },
        { 576, 0, 96, 58 },
        { 672, 0, 96, 56 },
        { 768, 0, 96, 55 },
        { 864, 0, 96, 58 },
        // row 1
        { 0, 58, 96, 60 },
        { 96, 58, 96, 57 },
        { 192, 58, 96, 53 },
        { 288, 58, 96, 60 },
        { 384, 58, 96, 59 },
        { 480, 58, 96, 59 },
        { 576, 58, 96, 59 },
        { 672, 58, 96, 59 },
        { 768, 58, 96, 59 }
    };

    tm->RegisterSprite(SpriteRocksFile, rectsRocks);

    // SCENE ELEMENTS
    const std::vector<lib::core::Rectd> rectsSceneElements
    {
        { 0, 0, 96, 48 }
    };

    tm->RegisterSprite(SpriteFileSceneElements, rectsSceneElements);

    // STRUCTURES
    const std::vector<lib::core::Rectd> rectsStructures
    {
        // base lvl 1
        { 0, 0, 288, 163 },
        { 288, 0, 288, 163 },
        { 576, 0, 288, 163 },
        { 0, 163, 288, 163 },
        { 288, 163, 288, 163 },
        { 576, 163, 288, 163 },

        // def tower lvl 1
        { 0, 326, 96, 78 },
        { 96, 326, 96, 78 },
        { 192, 326, 96, 78 },

        // radar
        { 0, 404, 192, 119 },
        { 192, 404, 192, 119 },
        { 384, 404, 192, 119 },
        { 576, 404, 192, 119 },
        { 0, 523, 192, 119 },
        { 192, 523, 192, 119 },
        { 384, 523, 192, 119 }
    };

    tm->RegisterSprite(SpriteFileStructures, rectsStructures);

    // UNITS
    const std::vector<lib::core::Rectd> rectsUnits
    {
        // row 0
        { 0, 0, 96, 54 },
        { 96, 0, 96, 54 },
        { 192, 0, 96, 58 },
        { 288, 0, 96, 58 },
        { 384, 0, 96, 54 },
        { 480, 0, 96, 54 },

        // row 1
        { 0, 58, 96, 54 },
        { 96, 58, 96, 54 },
        { 192, 58, 96, 58 },
        { 288, 58, 96, 58 },
        { 384, 58, 96, 54 },
        { 480, 58, 96, 54 },

        // row 2
        { 0, 116, 96, 54 },
        { 96, 116, 96, 54 },
        { 192, 116, 96, 58 },
        { 288, 116, 96, 58 },
        { 384, 116, 96, 54 },
        { 480, 116, 96, 54 }
    };

    tm->RegisterSprite(SpriteFileUnits, rectsUnits);

    // UNIT PARTICLES
    const std::vector<lib::core::Rectd> rectsUnitsPart
    {
        { 0, 0, 10, 2 },
        { 0, 2, 10, 2 },
        { 0, 4, 10, 2 }
    };

    tm->RegisterSprite(SpriteFileUnitsParticles, rectsUnitsPart);

    // WALLS
    std::vector<lib::core::Rectd> rectsWall;

    // level 1 walls
    indRows = 3;
    indCols = 11;
    indW = 96;
    indH = 48;

    x = 0;
    y = 0;

    for(int r = 0; r < indRows; ++r)
    {
        for(int c = 0; c < indCols; ++c)
        {
            rectsWall.emplace_back(x, y, indW, indH);
            x += indW;
        }

        x = 0;
        y += indH;
    }

    tm->RegisterSprite(SpriteWallsFile, rectsWall);

    // -- UI --
    // NEW UNIT DIALOG
    std::vector<lib::core::Rectd> rectsNewUnitDialog;

    rectsNewUnitDialog.emplace_back(0, 0, 180, 375);
    rectsNewUnitDialog.emplace_back(180, 0, 120, 375);
    rectsNewUnitDialog.emplace_back(300, 0, 10, 375);

    rectsNewUnitDialog.emplace_back(310, 0, 116, 41);
    rectsNewUnitDialog.emplace_back(426, 0, 116, 41);
    rectsNewUnitDialog.emplace_back(310, 41, 116, 41);
    rectsNewUnitDialog.emplace_back(426, 41, 116, 41);
    rectsNewUnitDialog.emplace_back(310, 82, 116, 41);

    rectsNewUnitDialog.emplace_back(310, 123, 150, 180);
    rectsNewUnitDialog.emplace_back(460, 82, 110, 100);
    rectsNewUnitDialog.emplace_back(460, 182, 110, 180);

    rectsNewUnitDialog.emplace_back(310, 303, 4, 13);
    rectsNewUnitDialog.emplace_back(314, 303, 8, 14);
    rectsNewUnitDialog.emplace_back(322, 303, 11, 12);
    rectsNewUnitDialog.emplace_back(333, 303, 10, 8);

    tm->RegisterSprite(SpriteFileNewUnitDialog, rectsNewUnitDialog);

    // OBJECT ACTION BUTTON
    std::vector<lib::core::Rectd> rectsObjActButton;

    // button bg
    indCols = 5;
    indW = 64;
    indH = 64;

    x = 0;
    y = 0;

    for(int c = 0; c < indCols; ++c)
    {
        rectsObjActButton.emplace_back(x, y, indW, indH);
        x += indW;
    }

    y += indH;

    // icons
    indCols = 8;
    indW = 48;
    indH = 48;

    x = 0;

    for(int c = 0; c < indCols; ++c)
    {
        rectsObjActButton.emplace_back(x, y, indW, indH);
        x += indW;
    }

    tm->RegisterSprite(SpriteFileObjActionButton, rectsObjActButton);
}

void ScreenGame::InitParticlesSystem()
{
    // SINGlE LASER
    auto u = new UpdaterSingleLaser;
    mPartMan->RegisterUpdater(PU_SINGLE_LASER, u);
}

void ScreenGame::CreateIsoMap()
{
    const int TILE_W = 96;

    // iso map
    mIsoMap = new IsoMap(TILE_W);
    mIsoMap->SetTiles(SpriteFileCells, NUM_IND_CELLS);
}

void ScreenGame::CreateLayers()
{
    mIsoMap->CreateLayer(MapLayers::CELL_OVERLAYS1);

    mIsoMap->CreateLayer(MapLayers::CELL_OVERLAYS2);

    mIsoMap->CreateLayer(MapLayers::OBJECTS);

    mIsoMap->CreateLayer(MapLayers::CELL_OVERLAYS3);
}

void ScreenGame::CreateUI()
{
    const int rendW = lib::graphic::Renderer::Instance()->GetWidth();

    Player * player = GetGame()->GetLocalPlayer();

    // top resources bar
    mPanelResBar = new PanelResources(player);
    mPanelResBar->SetX((rendW - mPanelResBar->GetWidth()) * 0.5f);

    // BASE ACTIONS
    mPanelObjActions = new PanelObjectActions;
    mPanelObjActions->SetVisible(false);

    // create new unit
    mPanelObjActions->SetButtonFunction(PanelObjectActions::BTN_BUILD_UNIT, [this, player]
    {
        if(nullptr == mDialogNewUnit)
        {
            mDialogNewUnit = new DialogNewUnit(player);

            mDialogNewUnit->SetFunctionOnClose([this]
            {
                // schedule dialog deletion
                mWidgetsToDelete.push_back(mDialogNewUnit);
                mDialogNewUnit = nullptr;
            });

            mDialogNewUnit->SetFunctionOnBuild([this, player]
            {
                const UnitType type = mDialogNewUnit->GetTypeToBuild();
                SetupNewUnit(type, player->GetSelectedObject(), player);

                ClearNewUnitDialog();
            });

            // populate available units
            const std::vector<UnitData> & unitsData = player->GetAvailableUnits();

            for(const UnitData & data : unitsData)
                mDialogNewUnit->AddUnitSlot(data);

            // position dialog
            const int posX = mPanelObjActions->GetX();
            const int posY = mPanelObjActions->GetY() - mDialogNewUnit->GetHeight();

            mDialogNewUnit->SetPosition(posX, posY);
        }
    });

    // UNIT ACTIONS
    // build structure
    mPanelObjActions->SetButtonFunction(PanelObjectActions::BTN_BUILD_STRUCT, [this, player]
    {
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionId::BUILD_DEF_TOWER);

        ClearCellOverlays();
    });

    // build wall
    mPanelObjActions->SetButtonFunction(PanelObjectActions::BTN_BUILD_WALL, [this, player]
    {
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionId::BUILD_WALL);

        ClearCellOverlays();
    });

    // attack
    mPanelObjActions->SetButtonFunction(PanelObjectActions::BTN_ATTACK, [this, player]
    {
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionId::ATTACK);

        ClearCellOverlays();
    });

    // conquer
    mPanelObjActions->SetButtonFunction(PanelObjectActions::BTN_CONQUER_CELL, [this, player]
    {
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionId::CONQUER_CELL);

        ClearCellOverlays();
    });

    // move
    mPanelObjActions->SetButtonFunction(PanelObjectActions::BTN_MOVE, [this, player]
    {
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionId::MOVE);

        ClearCellOverlays();
    });

    // GENERIC ACTIONS
    // upgrade
    mPanelObjActions->SetButtonFunction(PanelObjectActions::BTN_UPGRADE, [this, player]
    {
        // TODO

        ClearCellOverlays();
    });

    // cancel
    mPanelObjActions->SetButtonFunction(PanelObjectActions::BTN_CANCEL, [this, player]
    {
        GameObject * selObj = player->GetSelectedObject();

        auto it = mActiveObjActions.begin();

        // search selected object in active actions
        while(it != mActiveObjActions.end())
        {
            if(it->obj == selObj)
            {
                GameObjectAction & act = *it;

                const GameObjectType objType = act.obj->GetObjectType();
                const GameObjectActionId objActId = act.actId;

                // object is a Base
                if(objType == OBJ_BASE)
                {
                    // building a new unit
                    if(objActId == GameObjectActionId::BUILD_UNIT)
                    {
                        CancelProgressBar(act.actionCell);

                        act.obj->SetActiveAction(GameObjectActionId::IDLE);
                        act.obj->SetBusy(false);
                    }
                }
                // object is a Unit
                else if(objType == OBJ_UNIT)
                {
                    // moving
                    if(objActId == GameObjectActionId::MOVE)
                        mGameMap->AbortMove(selObj);
                    else if(objActId == GameObjectActionId::CONQUER_CELL)
                        mGameMap->AbortCellConquest(selObj);
                    else if(objActId == GameObjectActionId::BUILD_WALL)
                        mGameMap->AbortBuildWalls(selObj);
                    else if(objActId == GameObjectActionId::CONQUER_STRUCTURE)
                        mGameMap->AbortConquerStructure(act.actionCell, act.target);
                }

                mActiveObjActions.erase(it);

                break;
            }
            else
                ++it;
        }
    });
}

void ScreenGame::HidePanelObjActions()
{
    mPanelObjActions->ClearObject();
    mPanelObjActions->SetVisible(false);
}

void ScreenGame::ClearNewUnitDialog()
{
    // no dialog -> nothing to do
    if(nullptr == mDialogNewUnit)
        return ;

    // schedule dialog deletion
    mWidgetsToDelete.push_back(mDialogNewUnit);
    mDialogNewUnit = nullptr;
}

void ScreenGame::OnKeyDown(lib::core::KeyboardEvent & event)
{
    using namespace lib::core;

    const int key = event.GetKey();

    if(key == KeyboardEvent::KEY_LEFT)
        mCameraDirX = -1;
    else if(key == KeyboardEvent::KEY_RIGHT)
        mCameraDirX = 1;
    else if(key == KeyboardEvent::KEY_UP)
        mCameraDirY = -1;
    else if(key == KeyboardEvent::KEY_DOWN)
        mCameraDirY = 1;
}

void ScreenGame::OnKeyUp(lib::core::KeyboardEvent & event)
{
    using namespace lib::core;

    const int key = event.GetKey();

    Player * p = GetGame()->GetLocalPlayer();

    // P -> PAUSE
    if(key == KeyboardEvent::KEY_P)
    {
        mPaused = !mPaused;

        // disable actions panel when paused
        mPanelObjActions->SetEnabled(!mPaused);
    }
    // ARROS -> move camera
    else if(key == KeyboardEvent::KEY_LEFT)
        mCameraDirX = 0;
    else if(key == KeyboardEvent::KEY_RIGHT)
        mCameraDirX = 0;
    else if(key == KeyboardEvent::KEY_UP)
        mCameraDirY = 0;
    else if(key == KeyboardEvent::KEY_DOWN)
        mCameraDirY = 0;
    // C -> recenter camera
    else if(key == KeyboardEvent::KEY_C)
        mCamera->ResetPosition();
    // DEBUG: ALT + U -> toggle UI
    else if(event.IsModAltDown() && key == KeyboardEvent::KEY_U)
    {
        auto stage = lib::sgui::Stage::Instance();
        stage->SetVisible(!stage->IsVisible());
    }
    // DEBUG: SHIFT/CTRL + V -> add/remove visibility to whole map
    else if(key == KeyboardEvent::KEY_V)
    {
        if(event.IsModShiftDown())
        {
            p->AddVisibilityToAll();
            mGameMap->ApplyVisibility(p);
        }
        else if(event.IsModCtrlDown())
        {
            p->RemVisibilityToAll();
            mGameMap->ApplyVisibility(p);
        }
    }
    // DEBUG: SHIFT/CTRL + R -> add/remove resources
    else if(key == KeyboardEvent::KEY_R)
    {
        if(event.IsModShiftDown())
        {
            p->GetStat(Player::Stat::ENERGY).SumValue(100);
            p->GetStat(Player::Stat::MATERIAL).SumValue(50);
        }
        else if(event.IsModCtrlDown())
        {
            p->GetStat(Player::Stat::ENERGY).SumValue(-50);
            p->GetStat(Player::Stat::MATERIAL).SumValue(-50);
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

    const int worldX = mCamera->GetScreenToWorldX(event.GetX());
    const int worldY = mCamera->GetScreenToWorldY(event.GetY());

    const Cell2D clickCell = mIsoMap->CellFromScreenPoint(worldX, worldY);
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
    const bool isClickObjOwn = clickObj != nullptr && clickObj->GetOwner() == player;

    if(hasSelected)
    {
        GameObject * selObj = player->GetSelectedObject();
        const Cell2D selCell(selObj->GetRow0(), selObj->GetCol0());

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

            const GameObjectActionId action = selUnit->GetActiveAction();

            if(action == GameObjectActionId::MOVE)
            {
                const bool diffClick = selCell != clickCell;

                // try to move only if clicked on a different cell
                if(diffClick)
                    HandleUnitMoveOnMouseUp(selUnit, clickCell);
            }
            if(action == GameObjectActionId::ATTACK)
            {
                // clicked on nothing
                if(isClickObjOwn || nullptr == clickObj)
                    return ;

                selUnit->SetAttackTarget(clickObj);
            }
            else if(action == GameObjectActionId::CONQUER_CELL)
            {
                const int clickInd = clickCell.row * mGameMap->GetNumCols() + clickCell.col;

                // destination is visible and walkable or conquering unit cell
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

                        // store active action
                        mActiveObjActions.emplace_back(selUnit, action);

                        ClearSelection(player);
                    }
                }
            }
            else if (action == GameObjectActionId::BUILD_WALL)
            {
                const int clickInd = clickCell.row * mGameMap->GetNumCols() + clickCell.col;

                const bool diffClick = selCell != clickCell;

                // not clicking on unit cell, destination is visible and walkable
                if(diffClick &&
                   player->IsCellVisible(clickInd) &&
                   mGameMap->IsCellWalkable(clickCell.row, clickCell.col))
                {
                    const auto path = mPathfinder->MakePath(selCell.row, selCell.col,
                                                            clickCell.row, clickCell.col);

                    // path available -> start building
                    if(!path.empty())
                    {
                        auto wbp = new WallBuildPath(selUnit, mIsoMap, mGameMap, this);
                        wbp->SetPathCells(path);
                        // only first level for now
                        wbp->SetWallLevel(0);

                        mGameMap->BuildWalls(wbp);

                        // store active action
                        mActiveObjActions.emplace_back(selUnit, action);

                        ClearSelection(player);
                    }
                }
            }
            else if (action == GameObjectActionId::BUILD_DEF_TOWER)
                HandleUnitBuildStructureOnMouseUp(selUnit, clickCell, GameObjectType::OBJ_DEF_TOWER);
        }
    }
    // no object currently selected
    else
    {
        if(isClickObjOwn)
            SelectObject(clickObj, player);
    }
}

void ScreenGame::OnMouseMotion(lib::core::MouseMotionEvent & event)
{
    const int worldX = mCamera->GetScreenToWorldX(event.GetX());
    const int worldY = mCamera->GetScreenToWorldY(event.GetY());

    const Cell2D currCell = mIsoMap->CellFromScreenPoint(worldX, worldY);

    // still in the same cell -> nothing to do
    if(mPrevCell == currCell)
        return ;

    Player * player = GetGame()->GetLocalPlayer();
    GameObject * selObj = player->GetSelectedObject();

    Unit * selUnit = (selObj && selObj->GetObjectType() == OBJ_UNIT) ?
                     static_cast<Unit *>(selObj) : nullptr;

    // unit selected -> handle mouse motion
    if(selUnit != nullptr)
    {
        const GameObjectActionId action = selUnit->GetActiveAction();

        if(action == GameObjectActionId::MOVE)
            HandleUnitMoveOnMouseMove(selUnit, currCell);
        else if(action == GameObjectActionId::CONQUER_CELL)
            HandleUnitConquestOnMouseMove(selUnit, currCell);
        else if(action == GameObjectActionId::BUILD_WALL)
            HandleUnitBuildWallOnMouseMove(selUnit, currCell);
        else if(action == GameObjectActionId::BUILD_DEF_TOWER)
            HandleUnitBuildStructureOnMouseMove(selUnit, currCell, GameObjectType::OBJ_DEF_TOWER);
    }

    // update previous cell before exit
    mPrevCell = currCell;
}

CellProgressBar * ScreenGame::CreateProgressBar(const Cell2D & cell, float time, PlayerFaction playerFaction)
{
    auto pb = new CellProgressBar(playerFaction, 0.f, time);
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
    GameObject * selObj = player->GetSelectedObject();

    if(selObj != nullptr)
        HidePanelObjActions();

    player->ClearSelectedObject();

    mPanelObjActions->ClearObject();
    mPanelObjActions->SetVisible(false);

    ClearCellOverlays();

    ClearNewUnitDialog();
}

void ScreenGame::SelectObject(GameObject * obj, Player * player)
{
    obj->SetSelected(true);

    mPanelObjActions->SetObject(obj);
    mPanelObjActions->SetVisible(true);

    player->SetSelectedObject(obj);
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
    //Player * player = ai->GetPlayer();
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

bool ScreenGame::SetupNewUnit(UnitType type, GameObject * gen, Player * player)
{
    const UnitData & data = player->GetAvailableUnitData(type);

    // check if create is possible
    if(!mGameMap->CanCreateUnit(data, gen, player))
        return false;

    Cell2D cell = mGameMap->GetNewUnitDestination(gen);

    if(-1 == cell.row || -1 == cell.col)
    {
        std::cerr << "GameMap::GetNewUnitDestination FAILED" << std::endl;
        return false;
    }

    // start create
    mGameMap->StartCreateUnit(data, gen, cell, player);

    gen->SetActiveAction(GameObjectActionId::BUILD_UNIT);

    // create and init progress bar
    CellProgressBar * pb = CreateProgressBar(cell, TIME_NEW_UNIT, player->GetFaction());

    pb->SetFunctionOnCompleted([this, cell, player, gen, data]
    {
        gen->SetActiveAction(GameObjectActionId::IDLE);

        mGameMap->CreateUnit(data, gen, cell, player);
        mProgressBarsToDelete.emplace_back(CellToIndex(cell));

        ClearObjectAction(gen);
    });

    // store active action
    mActiveObjActions.emplace_back(gen, GameObjectActionId::BUILD_UNIT, cell);

    return true;
}

bool ScreenGame::SetupStructureConquest(const Cell2D & start, const Cell2D & end, Player * player)
{
    // check if conquest is possible
    if(!mGameMap->CanConquerStructure(start, end, player))
        return false;

    // start conquest
    mGameMap->StartConquerStructure(start, end, player);

    const GameMapCell & unitCell = mGameMap->GetCell(start.row, start.col);
    GameObject * unit = unitCell.obj;

    // create and init progress bar
    CellProgressBar * pb = CreateProgressBar(start, TIME_CONQ_RES_GEN, player->GetFaction());

    pb->SetFunctionOnCompleted([this, start, end, player, unit]
    {
        mGameMap->ConquerStructure(start, end, player);
        mProgressBarsToDelete.emplace_back(CellToIndex(start));

        // clear action data once the action is completed
        ClearObjectAction(unit);
    });

    // store active action
    const GameMapCell & targetCell = mGameMap->GetCell(end.row, end.col);

    mActiveObjActions.emplace_back(unit, targetCell.obj, GameObjectActionId::CONQUER_STRUCTURE, start);

    return true;
}

bool ScreenGame::SetupStructureBuilding(Unit * unit, const Cell2D & cellTarget, Player * player, GameObjectType structure)
{
    // check if building is possible
    if(!mGameMap->CanBuildStructure(cellTarget, player, structure))
        return false;

    mGameMap->StartBuildStructure(cellTarget, player, structure);

    // create and init progress bar
    // TODO get time from unit
    CellProgressBar * pb = CreateProgressBar(cellTarget, TIME_CONQ_RES_GEN, player->GetFaction());

    pb->SetFunctionOnCompleted([this, unit, cellTarget, player, structure]
    {
        mGameMap->BuildStructure(cellTarget, player, structure);
        mProgressBarsToDelete.emplace_back(CellToIndex(cellTarget));

        // clear action data once the action is completed
        ClearObjectAction(unit);
    });

    // store active action
    mActiveObjActions.emplace_back(unit, GameObjectActionId::BUILD_DEF_TOWER, cellTarget);

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
    CellProgressBar * pb = CreateProgressBar(cell, TIME_UPG_UNIT, player->GetFaction());

    pb->SetFunctionOnCompleted([this, cell]
    {
        mGameMap->UpgradeUnit(cell);
        mProgressBarsToDelete.emplace_back(CellToIndex(cell));
    });

    return true;
}

void ScreenGame::SetupUnitMove(Unit * unit, const Cell2D & start, const Cell2D & end,
                               const std::function<void()> & onCompleted)
{
    const auto path = mPathfinder->MakePath(start.row, start.col, end.row, end.col,
                                            lib::ai::Pathfinder::ALL_OPTIONS);

    // empty path -> exit
    if(path.empty())
        return ;

    auto op = new ObjectPath(unit, mIsoMap, mGameMap, this);
    op->SetPathCells(path);
    op->SetOnCompleted(onCompleted);

    mGameMap->MoveUnit(op);

    ClearCellOverlays();

    // store active action
    mActiveObjActions.emplace_back(unit, GameObjectActionId::MOVE);
}

void ScreenGame::HandleUnitMoveOnMouseMove(Unit * unit, const Cell2D & currCell)
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS2);

    const bool currInside = mIsoMap->IsCellInside(currCell);

    // mouse outside the map
    if(!currInside)
    {
        // hide the indicator, if any
        if(mMoveInd != nullptr)
            layer->SetObjectVisible(mMoveInd, false);

        return ;
    }

    // move indicator already created -> move it and continue
    if(mMoveInd != nullptr)
        layer->MoveObject(mMoveInd->GetRow(), mMoveInd->GetCol(), currCell.row, currCell.col);
    // create new move indicator
    else
    {
        mMoveInd = new MoveIndicator;
        layer->AddObject(mMoveInd, currCell.row, currCell.col);
    }

    const int currInd = currCell.row * mGameMap->GetNumCols() + currCell.col;

    Player * player = GetGame()->GetLocalPlayer();

    const bool currVisible = player->IsCellVisible(currInd);
    const bool currVisited = mGameMap->IsCellObjectVisited(currInd);
    const bool currWalkable = mGameMap->IsCellWalkable(currInd);

    const bool showIndicator = (!currVisible && !currVisited) || currWalkable;

    layer->SetObjectVisible(mMoveInd, showIndicator);

    // stop here if not showing indicator
    if(!showIndicator)
        return ;

    if(currVisible)
    {
        // set indicator type
        mMoveInd->SetIndicatorType(MoveIndicator::NORMAL);

        // show path cost when destination is visible
        const auto path = mPathfinder->MakePath(unit->GetRow0(), unit->GetCol0(),
                                                currCell.row, currCell.col,
                                                lib::ai::Pathfinder::ALL_OPTIONS);

        ObjectPath op(unit, mIsoMap, mGameMap, this);
        op.SetPathCells(path);

        mMoveInd->SetCost(op.GetPathCost());
    }
    // not visible destination
    else
    {
        // set indicator type
        mMoveInd->SetIndicatorType(MoveIndicator::NO_VIS_CELL);

        // hide cost when destination is not visible
        mMoveInd->SetCostUnknown();
    }
}

void ScreenGame::HandleUnitConquestOnMouseMove(Unit * unit, const Cell2D & currCell)
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS2);

    // first clear all objects from the layer
    layer->ClearObjects();

    const bool currInside = mIsoMap->IsCellInside(currCell);

    // mouse outside the map
    if(!currInside)
        return ;

    const int currInd = currCell.row * mGameMap->GetNumCols() + currCell.col;

    Player * player = GetGame()->GetLocalPlayer();

    const bool currVisible = player->IsCellVisible(currInd);
    const bool currWalkable = mGameMap->IsCellWalkable(currInd);
    const bool currIsUnitCell = currCell.row == unit->GetRow0() && currCell.col == unit->GetCol0();

    const bool canConquer = currVisible && (currWalkable || currIsUnitCell);

    if(!canConquer)
        return ;

    // show path cost when destination is visible
    const auto path = mPathfinder->MakePath(unit->GetRow0(), unit->GetCol0(),
                                            currCell.row, currCell.col);

    // this should never happen, but just in case
    if(path.empty())
        return ;

    const unsigned int lastIdx = path.size() - 1;

    const PlayerFaction faction = player->GetFaction();

    for(unsigned int i = 0; i < path.size(); ++i)
    {
        ConquestIndicator * ind = nullptr;

        if(i < mConquestIndicators.size())
            ind = mConquestIndicators[i];
        else
        {
            ind = new ConquestIndicator;
            mConquestIndicators.emplace_back(ind);
        }

        // add indicator to layer
        const unsigned int pathInd = path[i];
        const unsigned int indRow = pathInd / mIsoMap->GetNumCols();
        const unsigned int indCol = pathInd % mIsoMap->GetNumCols();

        layer->AddObject(ind, indRow, indCol);

        ind->SetFaction(faction);
        ind->ShowCost(i == lastIdx);
    }

    ConquerPath cp(unit, mIsoMap, mGameMap, this);
    cp.SetPathCells(path);

    mConquestIndicators[lastIdx]->SetCost(cp.GetPathCost());
}

void ScreenGame::HandleUnitBuildWallOnMouseMove(Unit * unit, const Cell2D & currCell)
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS2);

    // first clear all objects from the layer
    layer->ClearObjects();

    const bool currInside = mIsoMap->IsCellInside(currCell);

    // mouse outside the map
    if(!currInside)
        return ;

    const int currInd = currCell.row * mGameMap->GetNumCols() + currCell.col;

    Player * player = GetGame()->GetLocalPlayer();

    const bool currVisible = player->IsCellVisible(currInd);
    const bool currWalkable = mGameMap->IsCellWalkable(currInd);
    const bool currIsUnitCell = currCell.row == unit->GetRow0() && currCell.col == unit->GetCol0();

    const bool canBuild = currVisible && (currWalkable || currIsUnitCell);

    if(!canBuild)
        return ;

    // show path cost when destination is visible
    const auto path = mPathfinder->MakePath(unit->GetRow0(), unit->GetCol0(),
                                            currCell.row, currCell.col);

    // this should not happen
    if(path.size() < 2)
        return ;

    const unsigned int lastIdx = path.size() - 1;

    const PlayerFaction faction = player->GetFaction();

    std::vector<Cell2D> cellsPath;
    cellsPath.reserve(path.size());

    // store coordinates of start cell
    const unsigned int pathInd0 = path[0];
    const unsigned int indRow0 = pathInd0 / mIsoMap->GetNumCols();
    const unsigned int indCol0 = pathInd0 % mIsoMap->GetNumCols();
    cellsPath.emplace_back(indRow0, indCol0);

    for(unsigned int i = 0; i < lastIdx; ++i)
    {
        WallIndicator * ind = nullptr;

        if(i < mWallIndicators.size())
            ind = mWallIndicators[i];
        else
        {
            ind = new WallIndicator;
            mWallIndicators.emplace_back(ind);
        }

        // add indicator to layer - skip path[0] as that's start
        const unsigned int pathInd = path[i + 1];
        const unsigned int indRow = pathInd / mIsoMap->GetNumCols();
        const unsigned int indCol = pathInd % mIsoMap->GetNumCols();
        cellsPath.emplace_back(indRow, indCol);

        layer->AddObject(ind, indRow, indCol);

        ind->SetFaction(faction);
        ind->ShowCost(false);
    }

    // -- set directions and costs --
    const unsigned int lastIndicator = lastIdx - 1;

    WallBuildPath wbp(unit, mIsoMap, mGameMap, this);
    wbp.SetPathCells(path);
    wbp.SetIndicatorsType(cellsPath, mWallIndicators);

    mWallIndicators[lastIndicator]->SetCost(wbp.GetEnergyCost(), wbp.GetMateriaCost());
}

void ScreenGame::HandleUnitBuildStructureOnMouseMove(Unit * unit, const Cell2D & currCell, GameObjectType structure)
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS3);

    // clear any current indicator
    layer->ClearObjects();

    // check if mouse is inside map
    const bool currInside = mIsoMap->IsCellInside(currCell);

    if(!currInside)
        return ;

    // check if unit is next to destination or if there's any walkable cell surrounding destination
    const int nextDist = 1;

    if((std::abs(unit->GetRow0() - currCell.row) > nextDist ||
       std::abs(unit->GetCol0() - currCell.col) > nextDist) &&
        !mGameMap->IsAnyNeighborCellWalkable(currCell.row, currCell.col))
        return ;

    // check if there's a path between unit and destination
    const auto path = mPathfinder->MakePath(unit->GetRow0(), unit->GetCol0(),
                                            currCell.row, currCell.col,
                                            lib::ai::Pathfinder::ALL_OPTIONS);

    if(path.empty())
        layer->ClearObjects();

    Player * player = GetGame()->GetLocalPlayer();

    // get an indicator
    StructureIndicator * ind = nullptr;
    auto it = mStructIndicators.find(structure);

    if(it != mStructIndicators.end())
        ind = it->second;
    else
    {
        ind = new StructureIndicator(structure);
        mStructIndicators.emplace(structure, ind);
    }

    // add indicator to layer
    layer->AddObject(ind, currCell.row, currCell.col);

    // set visibility
    const int currInd = currCell.row * mGameMap->GetNumCols() + currCell.col;
    const bool currVisible = player->IsCellVisible(currInd);
    const bool currWalkable = mGameMap->IsCellWalkable(currInd);
    const bool showIndicator = currVisible && currWalkable;

    layer->SetObjectVisible(ind, showIndicator);

    // indicator visible -> update faction
    if(showIndicator)
        ind->SetFaction(player->GetFaction());
}

void ScreenGame::HandleUnitMoveOnMouseUp(Unit * unit, const Cell2D & clickCell)
{
    const Cell2D selCell(unit->GetRow0(), unit->GetCol0());

    const bool clickWalkable = mGameMap->IsCellWalkable(clickCell.row, clickCell.col);

    // destination is walkable -> try to generate a path and move
    if(clickWalkable)
    {
        SetupUnitMove(unit, selCell, clickCell);
        return ;
    }

    Player * player = GetGame()->GetLocalPlayer();

    const GameMapCell & clickGameCell = mGameMap->GetCell(clickCell.row, clickCell.col);
    const GameObject * clickObj = clickGameCell.obj;
    const bool clickVisited = clickObj && clickObj->IsVisited();

    // destination never visited (hence not visible as well) -> try to move close
    if(!clickVisited)
    {
        Cell2D target = mGameMap->GetCloseMoveTarget(selCell, clickCell);

        // failed to find a suitable target
        if(-1 == target.row || -1 == target.col)
            return ;

        SetupUnitMove(unit, selCell, target);
        return ;
    }

    // check if destination obj is visible
    const bool clickVisible = clickObj && clickObj->IsVisible();

    // visited, but not visible object -> exit
    if(!clickVisible)
        return ;

    // visible, but it can't be conquered -> exit
    if(!clickObj->CanBeConquered())
        return ;

    // object is adjacent -> try to interact
    if(mGameMap->AreObjectsAdjacent(unit, clickObj))
    {
        if(SetupStructureConquest(selCell, clickCell, player))
            ClearSelection(player);
    }
    // object is far -> move close and then try to interact
    else
    {
        Cell2D target = mGameMap->GetAdjacentMoveTarget(selCell, clickObj);

        // failed to find a suitable target
        if(-1 == target.row || -1 == target.col)
            return ;

        SetupUnitMove(unit, selCell, target, [this, unit, clickCell, player]
        {
            const Cell2D currCell(unit->GetRow0(), unit->GetCol0());

            if(SetupStructureConquest(currCell, clickCell, player))
                ClearSelection(player);
        });
    }
}

void ScreenGame::HandleUnitBuildStructureOnMouseUp(Unit * unit, const Cell2D & clickCell, GameObjectType structure)
{
    const int clickInd = clickCell.row * mGameMap->GetNumCols() + clickCell.col;

    Player * player = GetGame()->GetLocalPlayer();

    // destination is visible and walkable
    if(player->IsCellVisible(clickInd) && mGameMap->IsCellWalkable(clickCell.row, clickCell.col))
    {
        const GameMapCell * gmc = unit->GetCell();
        const Cell2D cellUnit(gmc->row, gmc->col);

        // unit is next to target cell -> try to build
        if(mGameMap->AreCellsAdjacent(cellUnit, clickCell))
        {
            if(SetupStructureBuilding(unit, clickCell, player, structure))
                ClearSelection(player);
        }
        // unit is far -> move close then try to build
        else
        {
            Cell2D target = mGameMap->GetAdjacentMoveTarget(cellUnit, clickCell, clickCell);

            // failed to find a suitable target
            if(-1 == target.row || -1 == target.col)
                return ;

            // add temporary indicator for tower
            mTempStructIndicator = new StructureIndicator(structure);
            mTempStructIndicator->SetFaction(player->GetFaction());

            IsoLayer * layer = mIsoMap->GetLayer(MapLayers::OBJECTS);
            layer->AddObject(mTempStructIndicator, clickCell.row, clickCell.col);

            // move
            SetupUnitMove(unit, cellUnit, target, [this, unit, clickCell, player, structure]
            {
                const Cell2D currCell(unit->GetRow0(), unit->GetCol0());

                if(SetupStructureBuilding(unit, clickCell, player, structure))
                    ClearSelection(player);

                // get rid of temporary indicator
                if(mTempStructIndicator)
                {
                    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::OBJECTS);
                    layer->ClearObject(mTempStructIndicator);

                    delete mTempStructIndicator;
                    mTempStructIndicator = nullptr;
                }
            });
        }
    }
}

void ScreenGame::ClearCellOverlays()
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS2);
    layer->ClearObjects();

    layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS3);
    layer->ClearObjects();

    // delete move indicator
    delete mMoveInd;
    mMoveInd = nullptr;
}

} // namespace game
