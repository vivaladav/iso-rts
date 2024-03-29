﻿#include "Screens/ScreenGame.h"

#include "CameraMapController.h"
#include "Game.h"
#include "GameConstants.h"
#include "GameData.h"
#include "GameMap.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "MapLoader.h"
#include "MapsRegistry.h"
#include "Player.h"
#include "AI/ConquerPath.h"
#include "AI/ObjectPath.h"
#include "AI/PlayerAI.h"
#include "AI/WallBuildPath.h"
#include "GameObjects/DefensiveTower.h"
#include "GameObjects/ObjectsDataRegistry.h"
#include "GameObjects/Temple.h"
#include "GameObjects/Unit.h"
#include "GameObjects/WallGate.h"
#include "Indicators/AttackRangeIndicator.h"
#include "Indicators/ConquestIndicator.h"
#include "Indicators/HealingRangeIndicator.h"
#include "Indicators/MoveIndicator.h"
#include "Indicators/StructureIndicator.h"
#include "Indicators/WallIndicator.h"
#include "Particles/UpdaterDamage.h"
#include "Particles/UpdaterHealing.h"
#include "Particles/UpdaterLootboxPrize.h"
#include "Particles/UpdaterSingleLaser.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonQuickUnitSelection.h"
#include "Widgets/DialogNewElement.h"
#include "Widgets/GameHUD.h"
#include "Widgets/GameMapProgressBar.h"
#include "Widgets/MiniMap.h"
#include "Widgets/PanelObjectActions.h"

#include <sgl/ai/Pathfinder.h>
#include <sgl/core/event/ApplicationEvent.h>
#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/core/event/MouseButtonEvent.h>
#include <sgl/core/event/MouseMotionEvent.h>
#include <sgl/graphic/Camera.h>
#include <sgl/graphic/ParticlesManager.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/media/AudioManager.h>
#include <sgl/media/AudioPlayer.h>
#include <sgl/sgui/ButtonsGroup.h>
#include <sgl/sgui/Stage.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace game
{

// NOTE these will be replaced by dynamic values soon
constexpr float TIME_NEW_UNIT = 2.f;
constexpr float TIME_CONQ_RES_GEN = 2.f;
constexpr float TIME_UPG_UNIT = 5.f;

constexpr float TIME_ENERGY_USE = 8.f;
constexpr float TIME_AI_MOVE = 0.5f;

ScreenGame::ScreenGame(Game * game)
    : Screen(game)
    , mPartMan(new sgl::graphic::ParticlesManager)
    , mPathfinder(new sgl::ai::Pathfinder)
    , mCurrCell(-1, -1)
    , mTimerEnergy(TIME_ENERGY_USE)
    , mTimerAI(TIME_AI_MOVE)
{
    game->SetClearColor(0x1A, 0x1A, 0x1A, 0xFF);

    const int rendW = sgl::graphic::Renderer::Instance()->GetWidth();
    const int rendH = sgl::graphic::Renderer::Instance()->GetHeight();

    auto cam = sgl::graphic::Camera::GetDefaultCamera();
    cam->SetSize(rendW, rendH);

    mCamController = new CameraMapController(cam, game);
    mCamController->SetDraggingSpeed(game->GetMapDraggingSpeed());
    mCamController->SetScrollingSpeed(game->GetMapScrollingSpeed());

    mIdOnSettingsChanged = game->AddOnSettingsChangedFunction([this, game]
    {
        mCamController->SetDraggingSpeed(game->GetMapDraggingSpeed());
        mCamController->SetScrollingSpeed(game->GetMapScrollingSpeed());
    });

    InitParticlesSystem();

    // -- ISOMETRIC MAP --
    CreateIsoMap();
    CreateLayers();

    // create game map
    mGameMap = new GameMap(GetGame(), this, mIsoMap);

    LoadMapFile();

    // center map on screen
    const int mapH = mIsoMap->GetHeight();

    mIsoMap->SetOrigin(rendW * 0.5, (rendH - mapH) * 0.5);
    mIsoMap->SetVisibleArea(cam->GetX(), cam->GetY(), cam->GetWidth(), cam->GetHeight());

    cam->SetFunctionOnMove([this]
    {
        const sgl::graphic::Camera * cam = mCamController->GetCamera();
        const int camW = cam->GetWidth();
        const int camH = cam->GetHeight();
        const int camX0 = cam->GetX();
        const int camY0 = cam->GetY();
        const int camX1 = camX0 + camW;
        const int camY1 = camY0 + camH;

        // update map
        mIsoMap->SetVisibleArea(camX0, camY0, camW, camH);

        // update MiniMap
        MiniMap * mm = mHUD->GetMinimap();
        mm->SetCameraCells(mIsoMap->CellFromScreenPoint(camX0, camY0),
                           mIsoMap->CellFromScreenPoint(camX1, camY0),
                           mIsoMap->CellFromScreenPoint(camX0, camY1),
                           mIsoMap->CellFromScreenPoint(camX1, camY1));
    });

    // set reduced map area to cam controller so camera will stop closer to inside cells
    const sgl::core::Pointd2D isoMapO = mIsoMap->GetOrigin();
    const int isoMapHalfW = mIsoMap->GetWidth() / 2;
    const int isoMapHalfH = mIsoMap->GetHeight() / 2;
    const int marginCameraMult = 2;
    const int tileW = mIsoMap->GetTileWidth();
    const int tileH = mIsoMap->GetTileHeight();
    const int marginCameraX = marginCameraMult * tileW;
    const int marginCameraY = marginCameraMult * tileH;

    const sgl::core::Pointd2D pT(isoMapO.x, isoMapO.y + marginCameraY);
    const sgl::core::Pointd2D pR(pT.x + isoMapHalfW - marginCameraX, pT.y + isoMapHalfH);
    const sgl::core::Pointd2D pB(pT.x, pT.y + mIsoMap->GetHeight() - marginCameraY);
    const sgl::core::Pointd2D pL(pT.x - isoMapHalfW + marginCameraX, pT.y + isoMapHalfH);
    mCamController->SetMapArea(pT, pR, pB, pL);

    // init pathfinder
    mPathfinder->SetMap(mGameMap, mGameMap->GetNumRows(), mGameMap->GetNumCols());

    // -- PLAYERS --
    for(int i = 0; i < GetGame()->GetNumPlayers(); ++i)
    {
        Player * p = game->GetPlayerByIndex(i);

        if(p->IsAI())
        {
            p->GetAI()->SetGameMap(mGameMap);
            mAiPlayers.push_back(p);
        }
    }

    // UI
    CreateUI();

    // set initial camera position
    CenterCameraOverPlayerBase();

    // apply initial visibility to the game map
    Player * localPlayer = game->GetLocalPlayer();
    mGameMap->InitVisibility(localPlayer);

    InitMusic();

    // record start time
    mTimeStart = std::chrono::steady_clock::now();
}

ScreenGame::~ScreenGame()
{
    // clear Players
    Game * game = GetGame();

    for(int i = 0; i < game->GetNumPlayers(); ++i)
    {
        Player * p = game->GetPlayerByIndex(i);
        p->ClearMissionObjects();
        p->ClearSelectedObject();
    }

    game->RemoveOnSettingsChangedFunction(mIdOnSettingsChanged);

    delete mPartMan;

    for(auto ind : mAttIndicators)
        delete ind;

    for(auto ind : mHealIndicators)
        delete ind;

    for(auto ind : mConquestIndicators)
        delete ind;

    for(auto ind : mWallIndicators)
        delete ind;

    for(auto it : mStructIndicators)
        delete it.second;

    delete mTempStructIndicator;

    delete mIsoMap;
    delete mGameMap;

    delete mCamController;

    auto stage = sgl::sgui::Stage::Instance();
    stage->ClearWidgets();
    // make sure to reset stage visibility in case it was off before exit
    stage->SetVisible(true);
}

unsigned int ScreenGame::GetPlayTimeInSec() const
{
    const auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<double> played = now - mTimeStart;

    return played.count();
}

void ScreenGame::Update(float delta)
{
    // do nothing when paused
    if(mPaused)
        return ;

    // keep track of time played (while not paused)
    mTimePlayed += delta;

    // -- CAMERA --
    mCamController->Update(delta);

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
    // merge object actions to do with object actions list
    mObjActions.insert(mObjActions.end(), mObjActionsToDo.begin(), mObjActionsToDo.end());
    mObjActionsToDo.clear();

    mGameMap->Update(delta);

    // -- AI --
    if(!mAiPlayers.empty())
        UpdateAI(delta);

    // check game end
    UpdateGameEnd();
}

void ScreenGame::Render()
{
    mIsoMap->Render();

    mPartMan->Render();
}

void ScreenGame::ClearObjectAction(GameObject * obj)
{
    auto it = mObjActions.begin();

    // search selected object in active actions
    while(it != mObjActions.end())
    {
        if(it->obj == obj)
        {
            if(it->progressBar != nullptr)
                it->progressBar->DeleteLater();

            mObjActions.erase(it);

            return ;
        }

        ++it;
    }
}

sgl::graphic::ParticlesUpdater * ScreenGame::GetParticleUpdater(ParticlesUpdaterId updaterId)
{
    return mPartMan->GetUpdater(updaterId);
}

void ScreenGame::ClearSelection(Player * player)
{
    GameObject * selObj = player->GetSelectedObject();

    // no selection -> nothing to do
    if(nullptr == selObj)
        return ;

    mHUD->HidePanelObjActions();

    player->ClearSelectedObject();

    ClearCellOverlays();

    mHUD->HideDialogNewElement();
}

void ScreenGame::SelectObject(GameObject * obj, Player * player)
{
    player->SetSelectedObject(obj);

    // update quick selection buttons when selected unit
    sgl::sgui::ButtonsGroup * buttonsUnitSel = mHUD->GetButtonsGroupUnitSel();

    if(obj->GetObjectCategory() == GameObject::CAT_UNIT)
    {
        // check corresponding quick unit selection button
        const int numButtons = buttonsUnitSel->GetNumButtons();

        for(int i = 0; i < numButtons; ++i)
        {
            auto b = static_cast<ButtonQuickUnitSelection *>(buttonsUnitSel->GetButton(i));
            Unit * unit = b->GetUnit();

            if(unit == obj)
            {
                b->SetChecked(true);
                break;
            }
        }

        // show current indicator
        ShowActiveIndicators(static_cast<Unit *>(obj), mCurrCell);
    }
    // not a unit
    else
    {
        const int checked = buttonsUnitSel->GetIndexChecked();

        if(checked != -1)
            buttonsUnitSel->GetButton(checked)->SetChecked(false);

        // show attack range overlay for towers
        if(GameObject::TYPE_DEFENSIVE_TOWER == obj->GetObjectType())
        {
            auto tower = static_cast<DefensiveTower *>(obj);
            const int range = tower->GetAttackRange();
            ShowAttackIndicators(tower, range);
        }
    }

    PanelObjectActions * panelObjActions = mHUD->GetPanelObjectActions();
    panelObjActions->SetObject(obj);
    panelObjActions->SetVisible(true);
    panelObjActions->SetActionsEnabled(obj->GetCurrentAction() == IDLE);

    sgl::sgui::Stage::Instance()->SetFocus();
}

void ScreenGame::CenterCameraOverCell(int row, int col)
{
    const sgl::core::Pointd2D pos = mIsoMap->GetCellPosition(row, col);
    const int cX = pos.x + mIsoMap->GetTileWidth() * 0.5f;
    const int cY = pos.y + mIsoMap->GetTileHeight() * 0.5f;

    mCamController->CenterCameraToPoint(cX, cY);
}

void ScreenGame::CenterCameraOverObject(GameObject * obj)
{
    const GameMapCell * cell = obj->GetCell();
    CenterCameraOverCell(cell->row, cell->col);

    // update current cell like if mouse was moved
    UpdateCurrentCell();
}

MiniMap * ScreenGame::GetMiniMap() const
{
    if(mHUD)
        return mHUD->GetMinimap();
    else
        return nullptr;
}

void ScreenGame::SetMiniMapEnabled(bool val)
{
    mHUD->SetMiniMapEnabled(val);
}

void ScreenGame::SetPause(bool paused)
{
    mPaused = paused;

    mHUD->SetEnabled(!paused);
}

void ScreenGame::OnApplicationQuit(sgl::core::ApplicationEvent & event)
{
    mHUD->ShowDialogExit();

    event.SetConsumed();
}

void ScreenGame::InitMusic()
{
    auto am = sgl::media::AudioManager::Instance();
    auto ap = am->GetPlayer();

    ap->AddMusicToQueue("mission/music_01.ogg");
    ap->AddMusicToQueue("mission/music_02.ogg");
    ap->PlayMusicQueue();
}

void ScreenGame::InitParticlesSystem()
{
    sgl::graphic::ParticlesUpdater * updater;

    // DAMAGE
    updater = new UpdaterDamage;
    mPartMan->RegisterUpdater(PU_DAMAGE, updater);

    // HEALING
    updater = new UpdaterHealing;
    mPartMan->RegisterUpdater(PU_HEALING, updater);

    // LOOTBOX PRIZE
    updater = new UpdaterLootboxPrize;
    mPartMan->RegisterUpdater(PU_LOOTBOX_PRIZE, updater);

    // SINGLE LASER
    updater = new UpdaterSingleLaser;
    mPartMan->RegisterUpdater(PU_SINGLE_LASER, updater);
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
    mIsoMap->CreateLayer(MapLayers::CELL_OVERLAYS3);
    mIsoMap->CreateLayer(MapLayers::FACTION_INFLUENCE);
    mIsoMap->CreateLayer(MapLayers::OBJECTS1);
    mIsoMap->CreateLayer(MapLayers::OBJECTS2);
    mIsoMap->CreateLayer(MapLayers::CELL_OVERLAYS4);
}

void ScreenGame::CreateUI()
{
    using namespace sgl;

    const int rendW = graphic::Renderer::Instance()->GetWidth();
    const int rendH = graphic::Renderer::Instance()->GetHeight();

    Player * player = GetGame()->GetLocalPlayer();

    // init HUD layer
    mHUD = new GameHUD(this);

    mHUD->SetMiniMapEnabled(false);

    PanelObjectActions * panelObjActions = mHUD->GetPanelObjectActions();

    // create new unit
    panelObjActions->SetButtonFunction(PanelObjectActions::BTN_BUILD_UNIT,
                                       [this, player, panelObjActions]
    {
        mHUD->ShowDialogNewElement(DialogNewElement::ETYPE_UNITS);
    });

    // UNIT ACTIONS
    // build structure
    panelObjActions->SetButtonFunction(PanelObjectActions::BTN_BUILD_STRUCT,
                                       [this, player, panelObjActions]
    {
        mHUD->ShowDialogNewElement(DialogNewElement::ETYPE_STRUCTURES);

        ClearCellOverlays();
    });

    // build wall
    panelObjActions->SetButtonFunction(PanelObjectActions::BTN_BUILD_WALL, [this, player]
    {
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionType::BUILD_WALL);

        ClearCellOverlays();

        mWallPath.clear();

        ShowBuildWallIndicator(unit, mCurrCell);
    });

    // attack
    panelObjActions->SetButtonFunction(PanelObjectActions::BTN_ATTACK, [this, player]
    {
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionType::ATTACK);

        ClearCellOverlays();

        // show attack range overlay
        const int range = unit->GetRangeAttack();
        ShowAttackIndicators(unit, range);
    });

    // heal
    panelObjActions->SetButtonFunction(PanelObjectActions::BTN_HEAL, [this, player]
    {
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionType::HEAL);

        ClearCellOverlays();

        // show healing range overlay
        const int range = unit->GetRangeHealing();
        ShowHealingIndicators(unit, range);
    });

    // conquer
    panelObjActions->SetButtonFunction(PanelObjectActions::BTN_CONQUER_CELL, [this, player]
    {
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionType::CONQUER_CELL);

        ClearCellOverlays();

        mConquestPath.clear();

        ShowConquestIndicator(unit, mCurrCell);
    });

    // move
    panelObjActions->SetButtonFunction(PanelObjectActions::BTN_MOVE, [this, player]
    {
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionType::MOVE);

        ClearCellOverlays();

        ShowMoveIndicator(unit, mCurrCell);
    });

    // WALL GATE
    panelObjActions->SetButtonFunction(PanelObjectActions::BTN_OPEN_GATE,
                                       [this, player, panelObjActions]
    {
        // open gate
        auto gate = static_cast<WallGate *>(player->GetSelectedObject());
        const bool res = gate->Toggle();

        if(!res)
            return ;

        // move object down in game map
        mGameMap->MoveObjectDown(gate);

        // move to iso layer 1
        mIsoMap->ChangeObjectLayer(gate->GetIsoObject(), MapLayers::OBJECTS2, MapLayers::OBJECTS1);

        // update panel actions
        panelObjActions->SetObject(gate);

        // reset focus as buttons will change
        sgl::sgui::Stage::Instance()->SetFocus();
    });

    panelObjActions->SetButtonFunction(PanelObjectActions::BTN_CLOSE_GATE,
                                       [this, player, panelObjActions]
    {
        // close gate
        auto gate = static_cast<WallGate *>(player->GetSelectedObject());
        const bool res = gate->Toggle();

        if(!res)
            return ;

        // move object up in game map
        mGameMap->MoveObjectUp(gate);

        // move to iso layer 2
        mIsoMap->ChangeObjectLayer(gate->GetIsoObject(), MapLayers::OBJECTS1, MapLayers::OBJECTS2);

        // update panel actions
        panelObjActions->SetObject(gate);

        // reset focus as buttons will change
        sgl::sgui::Stage::Instance()->SetFocus();
    });

    // GENERIC ACTIONS
    // upgrade
    panelObjActions->SetButtonFunction(PanelObjectActions::BTN_UPGRADE, [this, player]
    {
        // TODO

        ClearCellOverlays();
    });

    // cancel
    panelObjActions->SetButtonFunction(PanelObjectActions::BTN_CANCEL,
                                       [this, player, panelObjActions]
    {
        GameObject * selObj = player->GetSelectedObject();

        if(nullptr == selObj)
            return ;

        const GameObjectActionType action = selObj->GetActiveAction();

        if(action == CONQUER_CELL || action == BUILD_WALL)
        {
            // clear overlays
            ClearCellOverlays();
            mConquestPath.clear();
            mWallPath.clear();

            // reset object action
            selObj->SetCurrentAction(GameObjectActionType::IDLE);
            selObj->SetActiveActionToDefault();

            // show current indicator
            ShowActiveIndicators(static_cast<Unit *>(selObj), mCurrCell);

            return ;
        }

        CancelObjectAction(selObj);
    });

    // MISSION COUNTDOWN
    if(MISSION_RESIST_TIME == mMissionType)
        mHUD->ShowMissionCountdown(mMissionTime);
}

void ScreenGame::LoadMapFile()
{
    const std::string & mapFile = GetGame()->GetCurrentMapFile();

    MapLoader ml;
    ml.Load(mapFile);

    const unsigned int rows = ml.GetMapRows();
    const unsigned int cols = ml.GetMapCols();

    // update iso map
    mIsoMap->SetSize(rows, cols, true);

    // update game map
    mGameMap->SetSize(rows, cols);

    const std::vector<unsigned int> & cells = ml.GetCellTypes();

    for(unsigned int r = 0; r < rows; ++r)
    {
        const unsigned int ind0 = r * cols;

        for(unsigned int c = 0; c < cols; ++c)
        {
            const unsigned int ind = ind0 + c;
            const auto t = static_cast<CellTypes>(cells[ind]);
            mGameMap->SetCellType(ind, t);

            // create collectable generators
            if(t == BLOBS_SOURCE)
                mGameMap->CreateCollectableGenerator(r, c, RES_BLOBS);
            else if(t == DIAMONDS_SOURCE)
                mGameMap->CreateCollectableGenerator(r, c, RES_DIAMONDS);
        }
    }

    // create objects
    const std::vector<MapObjectEntry> & objEntries = ml.GetObjectEntries();
    const unsigned int numEntries = objEntries.size();

    for(unsigned int i = 0; i < numEntries; ++i)
    {
        const auto & e = objEntries[i];

        mGameMap->CreateObjectFromFile(e.layerId, e.typeId, e.variantId, e.faction, e.r0, e.c0);
    }

    // get mission data
    mMissionType = ml.GetMissionType();
    mMissionTime = ml.GetMissionTime();
}

void ScreenGame::OnKeyDown(sgl::core::KeyboardEvent & event)
{
    mCamController->HandleKeyDown(event);
}

void ScreenGame::OnKeyUp(sgl::core::KeyboardEvent & event)
{
    using namespace sgl::core;

    // CAMERA
    mCamController->HandleKeyUp(event);

    UpdateCurrentCell();

    // GAME
    const int key = event.GetKey();

    Player * p = GetGame()->GetLocalPlayer();

    // P -> PAUSE
    if(key == KeyboardEvent::KEY_P)
        SetPause(!mPaused);
    else if(key == KeyboardEvent::KEY_ESCAPE)
        mHUD->ShowDialogExit();
    // SHIFT + B -> center camera on own base
    else if(key == KeyboardEvent::KEY_B)
    {
        if(event.IsModShiftDown())
            CenterCameraOverPlayerBase();
    }
    // DEBUG: ALT + U -> toggle UI
    else if(event.IsModAltDown() && key == KeyboardEvent::KEY_U)
        mHUD->SetVisible(!mHUD->IsVisible());
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
    // DEBUG: end mission dialog
    else if(event.IsModCtrlDown() && key == KeyboardEvent::KEY_W)
        mHUD->ShowDialogEndMission(true);
    else if(event.IsModCtrlDown() && key == KeyboardEvent::KEY_L)
        mHUD->ShowDialogEndMission(false);
    // DEBUG: explore temple dialog
    else if(event.IsModCtrlDown() && key == KeyboardEvent::KEY_E)
    {
        auto objs = mGameMap->GetObjects();

        for(GameObject * o : objs)
        {
            // assign first Temple found
            if(o->GetObjectType() == GameObject::TYPE_TEMPLE)
            {
                Player * p = GetGame()->GetLocalPlayer();
                mHUD->ShowDialogExploreTemple(p, static_cast<Temple *>(o));

                break;
            }
        }
    }
}

void ScreenGame::OnMouseButtonUp(sgl::core::MouseButtonEvent & event)
{
    // CAMERA
    const bool wasDragging = mCamController->IsDragging();

    mCamController->HandleMouseButtonUp(event);

    // do nothing if dragging
    if(wasDragging)
        return;

    // no interaction while game is paused
    if(mPaused)
        return ;

    if(event.GetButton() == sgl::core::MouseEvent::BUTTON_LEFT)
        HandleSelectionClick(event);
    else if(event.GetButton() == sgl::core::MouseEvent::BUTTON_RIGHT)
        HandleActionClick(event);
}

void ScreenGame::OnMouseMotion(sgl::core::MouseMotionEvent & event)
{
    mMousePos.x = event.GetX();
    mMousePos.y = event.GetY();

    // CAMERA
    mCamController->HandleMouseMotion(event);

    UpdateCurrentCell();
}

void ScreenGame::OnWindowExposed(sgl::graphic::WindowEvent &)
{
    SetPause(false);
}

void ScreenGame::OnWindowHidden(sgl::graphic::WindowEvent &)
{
    SetPause(true);
}

void ScreenGame::OnWindowMouseEntered(sgl::graphic::WindowEvent & event)
{
}

void ScreenGame::OnWindowMouseLeft(sgl::graphic::WindowEvent & event)
{
    mCamController->HandleMouseLeftWindow();
}

void ScreenGame::UpdateAI(float delta)
{
    mTimerAI -= delta;

    if(mTimerAI < 0.f)
    {
        PlayerAI * ai = mAiPlayers[mCurrPlayerAI]->GetAI();
        ai->Update(delta);
        ExecuteAIAction(ai);

        // move to next player and update timer
        ++mCurrPlayerAI;

        // reset current player if needed
        if(mCurrPlayerAI >= mAiPlayers.size())
            mCurrPlayerAI = 0;

        mTimerAI = TIME_AI_MOVE;
    }
}

void ScreenGame::ExecuteAIAction(PlayerAI * ai)
{
    bool done = false;

    Player * player = ai->GetPlayer();

    // execute planned action until one is successful or there's no more actions to do (NOP)
    while(!done)
    {
        const ActionAI * action = ai->GetNextActionTodo();

        if(nullptr == action)
            return ;

        auto basicOnDone = [action, ai](bool)
        {
             ai->SetActionDone(action);
        };

        // new higher action for busy object
        if(action->ObjSrc->IsBusy() && ai->IsActionHighestPriorityForObject(action))
        {
            std::cout << "ScreenGame::ExecuteAIAction - AI " << mCurrPlayerAI
                      << " - higher priority action for object " << action->ObjSrc << std::endl;

            // cancel current action
            ai->CancelObjectAction(action->ObjSrc);
            CancelObjectAction(action->ObjSrc);
        }

        switch(action->type)
        {
            case AIA_UNIT_ATTACK_ENEMY_UNIT:
            {
                auto unit = static_cast<Unit *>(action->ObjSrc);
                done = SetupUnitAttack(unit, action->ObjDst, player, basicOnDone);

                std::cout << "ScreenGame::ExecuteAIAction - AI " << mCurrPlayerAI
                          << " - ATTACK ENEMY UNIT "
                          << (done ? "DOING" : "FAILED") << std::endl;
            }
            break;

            case AIA_UNIT_CONQUER_GEN:
            {
                auto unit = static_cast<Unit *>(action->ObjSrc);
                const Cell2D start(unit->GetRow0(), unit->GetCol0());
                const Cell2D end(action->ObjDst->GetRow0(), action->ObjDst->GetCol0());

                // NOTE probably need to change actions to not take into consideration cells,
                // but only objects

                // unit and generator are next to each other
                if(mGameMap->AreObjectsOrthoAdjacent(action->ObjSrc, action->ObjDst))
                    done = SetupStructureConquest(unit, start, end, player, basicOnDone);
                // unit needs to move to the generator
                else
                {
                    Cell2D target = mGameMap->GetOrthoAdjacentMoveTarget(start, action->ObjDst);

                    // failed to find a suitable target
                    if(-1 == target.row || -1 == target.col)
                        done = false;
                    else
                    {
                        done = SetupUnitMove(unit, start, target,
                            [this, unit, end, player, basicOnDone](bool successful)
                            {
                                if(successful)
                                {
                                    const Cell2D currCell(unit->GetRow0(), unit->GetCol0());
                                    SetupStructureConquest(unit, currCell, end, player, basicOnDone);
                                }
                                else
                                    basicOnDone(successful);
                            });
                    }
                }

                std::cout << "ScreenGame::ExecuteAIAction - AI " << mCurrPlayerAI << " - CONQUER GENERATOR "
                          << (done ? "DOING" : "FAILED") << std::endl;
            }
            break;

            case AIA_UNIT_CONNECT_STRUCTURE:
            {
                auto unit = static_cast<Unit *>(action->ObjSrc);
                const Cell2D unitCell(unit->GetRow0(), unit->GetCol0());

                // unit and structure are next to each other
                if(mGameMap->AreObjectsOrthoAdjacent(unit, action->ObjDst))
                    done = SetupConnectCells(unit, basicOnDone);
                // unit needs to move to the structure
                else
                {
                    Cell2D target = mGameMap->GetOrthoAdjacentMoveTarget(unitCell, action->ObjDst);

                    // failed to find a suitable target
                    if(-1 == target.row || -1 == target.col)
                        done = false;
                    else
                    {
                        done = SetupUnitMove(unit, unitCell, target,
                            [this, unit, basicOnDone](bool successful)
                            {
                                if(successful)
                                    SetupConnectCells(unit, basicOnDone);
                                else
                                    basicOnDone(successful);
                            });
                    }
                }

                std::cout << "ScreenGame::ExecuteAIAction - AI " << mCurrPlayerAI << " -Result CONNECT STRUCTURE "
                          << (done ? "DOING" : "FAILED") << std::endl;
            }
            break;

            case AIA_NEW_UNIT:
            {
                auto a = static_cast<const ActionAINewUnit *>(action);

                done = SetupNewUnit(a->unitType, a->ObjSrc, ai->GetPlayer(), basicOnDone);

                std::cout << "ScreenGame::ExecuteAIAction - AI " << mCurrPlayerAI << " - NEW UNIT "
                          << (done ? "DOING" : "FAILED") << std::endl;
            }
            break;

            default:
                std::cout << "ScreenGame::ExecuteAIAction - AI " << mCurrPlayerAI << " - unkown action" << action->type << std::endl;
            break;
        }

        if(done)
            ai->RegisterActionInProgress(action);
    }
}

void ScreenGame::CancelObjectAction(GameObject * obj)
{
    auto it = mObjActions.begin();

    // search selected object in active actions
    while(it != mObjActions.end())
    {
        GameObjectAction & act = *it;

        if(act.obj == obj)
        {
            const GameObjectTypeId objType = act.obj->GetObjectType();
            const GameObjectActionType objActId = act.actId;

            // object is a Base
            if(objType == GameObject::TYPE_BASE)
            {
                // building a new unit
                if(objActId == GameObjectActionType::BUILD_UNIT)
                    act.progressBar->DeleteLater();
            }
            // object is a Unit
            else if(act.obj->GetObjectCategory() == GameObject::CAT_UNIT)
            {
                if(objActId == GameObjectActionType::MOVE)
                    mGameMap->AbortMove(obj);
                else if(objActId == GameObjectActionType::CONQUER_CELL)
                    mGameMap->AbortCellConquest(obj);
                else if(objActId == GameObjectActionType::BUILD_WALL)
                    mGameMap->AbortBuildWalls(obj);
                else if(objActId == GameObjectActionType::CONQUER_STRUCTURE)
                {
                    mGameMap->AbortConquerStructure(act.target);
                    act.progressBar->DeleteLater();
                }
                else if(objActId == GameObjectActionType::ATTACK)
                {
                    auto unit = static_cast<Unit *>(obj);
                    unit->ClearTargetAttack();
                }
                else if(objActId == GameObjectActionType::HEAL)
                {
                    auto unit = static_cast<Unit *>(obj);
                    unit->ClearTargetHealing();
                }
            }

            mObjActions.erase(it);

            obj->SetCurrentAction(GameObjectActionType::IDLE);
            obj->SetActiveActionToDefault();

            // re-enable actions for local player
            if(obj->GetFaction() == GetGame()->GetLocalPlayer()->GetFaction())
            {
                mHUD->GetPanelObjectActions()->SetActionsEnabled(true);

                // show current indicator for units
                if(obj->GetObjectCategory() == GameObject::CAT_UNIT)
                    ShowActiveIndicators(static_cast<Unit *>(act.obj), mCurrCell);
            }

            break;
        }
        else
            ++it;
    }
}

void ScreenGame::SetObjectActionDone(GameObject * obj, bool successful)
{
    auto it = mObjActions.begin();

    // search selected object in active actions
    while(it != mObjActions.end())
    {
        if(it->obj == obj)
        {
            // re-enable actions panel if obj is local
            if(obj->GetFaction() == GetGame()->GetLocalPlayerFaction())
                mHUD->GetPanelObjectActions()->SetActionsEnabled(true);

            // reset object's active action to default
            obj->SetActiveActionToDefault();
            // reset current action to idle
            obj->SetCurrentAction(IDLE);

            // execute done callback
            it->onDone(successful);

            // remove and destroy pending action
            mObjActions.erase(it);

            return ;
        }

        ++it;
    }
}

void ScreenGame::UpdateGameEnd()
{
    // check if player has base
    if(CheckGameOverForLocalPlayer())
    {
        mHUD->ShowDialogEndMission(false);
        return ;
    }

    switch(mMissionType)
    {
        case MISSION_DESTROY_ENEMY_BASE:
        {
            // check if destroyed all enemy bases
            bool bases = false;

            for(Player * p : mAiPlayers)
                bases |= p->HasStructure(GameObject::TYPE_BASE);

            if(!bases)
                mHUD->ShowDialogEndMission(true);
        }
        break;

        case MISSION_DESTROY_ALL_ENEMIES:
        {
            // check if destroyed all enemies
            int totEnemies = 0;

            for(Player * p : mAiPlayers)
                totEnemies += p->GetNumObjects();

            if(0 == totEnemies)
                mHUD->ShowDialogEndMission(true);
        }
        break;

        case MISSION_RESIST_TIME:
        {
            // check elapsed time
            const unsigned int playedTime = GetPlayTimeInSec();

            if(playedTime >= mMissionTime)
            {
                mHUD->HideMissionCountdown();
                mHUD->ShowDialogEndMission(true);
            }
        }
        break;

        default:
        break;
    }
}

void ScreenGame::HandleGameOver()
{
    // decide winner
    unsigned int winner = 0;
    int topObjs = 0;

    for(unsigned int i = 0; i < mAiPlayers.size(); ++i)
    {
        Player * p = mAiPlayers[i];

        const int numObjs = p->GetNumObjects();

        if(numObjs > topObjs)
        {
            winner = i;
            topObjs = numObjs;
        }
    }

    // assign territory to winner
    AssignWinningResources(mAiPlayers[winner]);
    AssignMapToFaction(mAiPlayers[winner]->GetFaction());
}

void ScreenGame::HandleGameWon()
{
    Game * game = GetGame();

    AssignWinningResources(game->GetLocalPlayer());
    AssignMapToFaction(game->GetLocalPlayerFaction());
}

void ScreenGame::AssignMapToFaction(PlayerFaction faction)
{
    Game * game = GetGame();
    MapsRegistry * mapReg = game->GetMapsRegistry();

    const unsigned int territory = game->GetCurrentTerritory();
    const Planets planet = game->GetCurrentPlanet();

    mapReg->SetMapOccupier(planet, territory, faction);
    mapReg->SetMapStatus(planet, territory, TER_ST_OCCUPIED);

    if(faction == game->GetLocalPlayerFaction())
        mapReg->SetMapMissionCompleted(planet, territory);

    game->RequestNextActiveState(StateId::PLANET_MAP);
}

void ScreenGame::AssignWinningResources(Player * player)
{
    const std::vector<GameObject *> & objs = mGameMap->GetObjects();

    const int bonusEnergy = 10;
    const int bonusMaterial = 25;
    const int bonusBlobs = 1;
    const int bonusDiamonds = 1;

    int energy = 0;
    int material = 0;
    int blobs = 0;
    int diamonds = 0;

    // assign energy and material
    for(GameObject * obj : objs)
    {
        const GameObjectTypeId type = obj->GetObjectType();

        if(type == GameObject::TYPE_RES_GEN_ENERGY)
            energy += bonusEnergy;
        else if(type == GameObject::TYPE_RES_GEN_MATERIAL)
            material += bonusMaterial;
    }

    player->SumResource(Player::ENERGY, energy);
    player->SumResource(Player::MATERIAL, material);

    // assign blobs and diamonds
    const std::vector<GameMapCell> & cells = mGameMap->GetCells();
    const unsigned int rows = mGameMap->GetNumRows();
    const unsigned int cols = mGameMap->GetNumCols();

    for(const GameMapCell & cell : cells)
    {
        const CellTypes t = cell.basicType;

        // create collectable generators
        if(t == BLOBS_SOURCE)
            blobs += bonusBlobs;
        else if(t == DIAMONDS_SOURCE)
            diamonds += bonusDiamonds;
    }

    player->SumResource(Player::BLOBS, blobs);
    player->SumResource(Player::DIAMONDS, diamonds);
}

bool ScreenGame::CheckGameOverForLocalPlayer()
{
    // check if player still has base
    const Player * player = GetGame()->GetLocalPlayer();

    return !player->HasStructure(GameObject::TYPE_BASE);
}

int ScreenGame::CellToIndex(const Cell2D & cell) const
{
    return cell.row * mIsoMap->GetNumCols() + cell.col;
}

bool ScreenGame::SetupNewUnit(GameObjectTypeId type, GameObject * gen, Player * player,
                              const std::function<void(bool)> & onDone)
{
    // check if create is possible
    if(!mGameMap->CanCreateUnit(type, gen, player))
        return false;

    Cell2D cell = mGameMap->GetNewUnitDestination(gen);

    if(-1 == cell.row || -1 == cell.col)
    {
        std::cerr << "GameMap::GetNewUnitDestination FAILED" << std::endl;
        return false;
    }

    // start create
    mGameMap->StartCreateUnit(type, gen, cell, player);

    // create and init progress bar
    GameMapProgressBar * pb = mHUD->CreateProgressBarInCell(cell, TIME_NEW_UNIT, player->GetFaction());

    pb->AddFunctionOnCompleted([this, cell, player, gen, type]
    {
        gen->SetCurrentAction(GameObjectActionType::IDLE);

        mGameMap->CreateUnit(type, gen, cell, player);

        // add unit to map if cell is visible to local player
        if(mGameMap->IsCellVisibleToLocalPlayer(cell.row, cell.col))
        {
            const ObjectBasicData & data = GetGame()->GetObjectsRegistry()->GetObjectData(type);

            const PlayerFaction faction = player->GetFaction();
            const MiniMap::MiniMapElemType type = static_cast<MiniMap::MiniMapElemType>(MiniMap::MME_FACTION1 + faction);
            MiniMap * mm = mHUD->GetMinimap();
            mm->AddElement(cell.row, cell.col, data.rows, data.cols, type, faction);
        }

        SetObjectActionCompleted(gen);
    });

    // store active action
    mObjActionsToDo.emplace_back(gen, GameObjectActionType::BUILD_UNIT, cell, pb, onDone);

    gen->SetActiveAction(GameObjectActionType::IDLE);
    gen->SetCurrentAction(GameObjectActionType::BUILD_UNIT);

    // disable actions panel (if action is done by local player)
    if(player->IsLocal())
        mHUD->GetPanelObjectActions()->SetActionsEnabled(false);

    return true;
}

bool ScreenGame::SetupStructureConquest(Unit * unit, const Cell2D & start, const Cell2D & end,
                                        Player * player, const std::function<void(bool)> & onDone)
{
    // check if conquest is possible
    if(!mGameMap->CanConquerStructure(unit, end, player))
        return false;

    // handle special case: TEMPLE
    if(player->IsLocal())
    {
        const GameMapCell & gameCell = mGameMap->GetCell(end.row, end.col);
        GameObject * obj = gameCell.objTop;

        if(obj != nullptr && obj->GetObjectType() == GameObject::TYPE_TEMPLE)
        {
            mHUD->ShowDialogExploreTemple(player, static_cast<Temple *>(obj));
            return false;
        }
    }

    // start conquest
    mGameMap->StartConquerStructure(end, player);

    // create and init progress bar
    GameMapProgressBar * pb = mHUD->CreateProgressBarInCell(start, TIME_CONQ_RES_GEN, player->GetFaction());

    pb->AddFunctionOnCompleted([this, start, end, player, unit]
    {
        mGameMap->ConquerStructure(end, player);

        unit->ConsumeEnergy(CONQUER_STRUCTURE);

        const GameMapCell & cellStruct = mGameMap->GetCell(end.row, end.col);
        const GameObject * objStruct = cellStruct.objTop;

        const PlayerFaction faction = player->GetFaction();
        const MiniMap::MiniMapElemType type = static_cast<MiniMap::MiniMapElemType>(MiniMap::MME_FACTION1 + faction);

        MiniMap * mm = mHUD->GetMinimap();
        mm->UpdateElement(objStruct->GetRow0(), objStruct->GetCol0(),
                          objStruct->GetRows(), objStruct->GetCols(), type, faction);

        // clear action data once the action is completed
        SetObjectActionCompleted(unit);
    });

    // store active action
    const GameMapCell & targetCell = mGameMap->GetCell(end.row, end.col);

    mObjActionsToDo.emplace_back(unit, targetCell.objTop, GameObjectActionType::CONQUER_STRUCTURE,
                             start, pb, onDone);

    unit->SetActiveAction(GameObjectActionType::IDLE);
    unit->SetCurrentAction(GameObjectActionType::CONQUER_STRUCTURE);

    // disable actions panel (if action is done by local player)
    if(player->IsLocal())
        mHUD->GetPanelObjectActions()->SetActionsEnabled(false);

    return true;
}

bool ScreenGame::SetupStructureBuilding(Unit * unit, const Cell2D & cellTarget, Player * player,
                                        const std::function<void(bool)> & onDone)
{
    const GameObjectTypeId st = unit->GetStructureToBuild();

    // check if building is possible
    if(!mGameMap->CanBuildStructure(unit, cellTarget, player, st))
        return false;

    mGameMap->StartBuildStructure(cellTarget, player, st);

    // create and init progress bar
    // TODO get time from unit
    GameMapProgressBar * pb = mHUD->CreateProgressBarInCell(cellTarget, TIME_CONQ_RES_GEN, player->GetFaction());

    pb->AddFunctionOnCompleted([this, unit, cellTarget, player, st]
    {
        mGameMap->BuildStructure(cellTarget, player, st);

        unit->ConsumeEnergy(BUILD_STRUCTURE);

        // add unit to map if cell is visible to local player
        const ObjectBasicData & data = GetGame()->GetObjectsRegistry()->GetObjectData(st);

        const unsigned int rTL = cellTarget.row - data.rows + 1;
        const unsigned int cTL = cellTarget.col - data.cols + 1;

        if(mGameMap->IsAnyCellVisibleToLocalPlayer(rTL, cTL, cellTarget.row, cellTarget.col))
        {
            const PlayerFaction faction = player->GetFaction();
            const MiniMap::MiniMapElemType type = static_cast<MiniMap::MiniMapElemType>(MiniMap::MME_FACTION1 + faction);
            MiniMap * mm = mHUD->GetMinimap();
            mm->AddElement(cellTarget.row, cellTarget.col, data.rows, data.cols, type, faction);
        }

        // clear action data once the action is completed
        SetObjectActionCompleted(unit);
    });

    // store active action
    mObjActionsToDo.emplace_back(unit, GameObjectActionType::BUILD_STRUCTURE, cellTarget, pb, onDone);

    // disable actions panel and clear overlays (if action is done by local player)
    if(player->IsLocal())
    {
        mHUD->GetPanelObjectActions()->SetActionsEnabled(false);

        ClearCellOverlays();
    }

    unit->SetActiveAction(GameObjectActionType::IDLE);
    unit->SetCurrentAction(GameObjectActionType::BUILD_STRUCTURE);
    unit->ClearStructureToBuild();

    return true;
}

bool ScreenGame::SetupUnitAttack(Unit * unit, GameObject * target, Player * player,
                                 const std::function<void(bool)> & onDone)
{
    const bool res = unit->SetTargetAttack(target);

    if(!res)
        return false;

    unit->SetActiveAction(GameObjectActionType::IDLE);
    unit->SetCurrentAction(GameObjectActionType::ATTACK);

    // disable actions panel (if action is done by local player)
    if(player->IsLocal())
        mHUD->GetPanelObjectActions()->SetActionsEnabled(false);

    mObjActionsToDo.emplace_back(unit, GameObjectActionType::ATTACK, onDone);

    return true;
}

bool ScreenGame::SetupUnitHeal(Unit * unit, GameObject * target, Player * player,
                               const std::function<void(bool)> & onDone)
{
    const bool res = unit->SetTargetHealing(target);

    if(!res)
        return false;

    unit->SetActiveAction(GameObjectActionType::IDLE);
    unit->SetCurrentAction(GameObjectActionType::HEAL);

    // disable actions panel (if action is done by local player)
    if(player->IsLocal())
        mHUD->GetPanelObjectActions()->SetActionsEnabled(false);

    mObjActionsToDo.emplace_back(unit, GameObjectActionType::HEAL, onDone);

    return true;
}

bool ScreenGame::SetupUnitMove(Unit * unit, const Cell2D & start, const Cell2D & end,
                               const std::function<void(bool)> & onDone)
{
    const auto path = mPathfinder->MakePath(start.row, start.col, end.row, end.col,
                                            sgl::ai::Pathfinder::ALL_OPTIONS);

    // empty path -> exit
    if(path.empty())
        return false;

    auto op = new ObjectPath(unit, mIsoMap, mGameMap, this);
    op->SetPathCells(path);

    const bool res = mGameMap->MoveUnit(op);

    // movement failed
    if(!res)
        return false;

    // disable actions panel (if action is done by local player)
    if(unit->GetFaction() == GetGame()->GetLocalPlayerFaction())
    {
        mHUD->GetPanelObjectActions()->SetActionsEnabled(false);

        ClearCellOverlays();
    }

    // store active action
    mObjActionsToDo.emplace_back(unit, GameObjectActionType::MOVE, onDone);

    unit->SetActiveAction(GameObjectActionType::IDLE);
    unit->SetCurrentAction(GameObjectActionType::MOVE);

    return true;
}

bool ScreenGame::SetupConnectCells(Unit * unit, const std::function<void (bool)> & onDone)
{
    const Player * player = GetGame()->GetPlayerByFaction(unit->GetFaction());
    const Cell2D startCell(unit->GetRow0(), unit->GetCol0());

    // find closest linked cell
    const std::vector<GameMapCell> & cells = mGameMap->GetCells();

    const int maxDist = mGameMap->GetNumRows() + mGameMap->GetNumCols();
    int minDist = maxDist;
    Cell2D targetCell(-1, -1);

    for(const GameMapCell & cell : cells)
    {
        if(cell.owner == player && cell.linked)
        {
            const Cell2D dest(cell.row, cell.col);
            int dist = mGameMap->ApproxDistance(startCell, dest);

            if(dist < minDist)
            {
                minDist = dist;
                targetCell = dest;
            }
        }
    }

    // can't find a target
    if(-1 == targetCell.row)
    {
        std::cout << "ScreenGame::ExecuteAIAction - AI " << mCurrPlayerAI
                  << " - CONNECT STRUCTURE FAILED (can't find target)" << std::endl;

        return false;
    }

    // if target cell has object try to find one next to it free
    if(mGameMap->HasObject(targetCell.row, targetCell.col))
    {
        targetCell = mGameMap->GetOrthoAdjacentMoveTarget(startCell, targetCell);

        // can't find an adjacent cell that's free
        if(-1 == targetCell.row)
        {
            std::cout << "ScreenGame::ExecuteAIAction - AI " << mCurrPlayerAI
                      << " - CONNECT STRUCTURE FAILED (GetOrthoAdjacentMoveTarget failed)" << std::endl;

            return false;
        }
    }

    const auto path = mPathfinder->MakePath(startCell.row, startCell.col,
                                            targetCell.row, targetCell.col,
                                            sgl::ai::Pathfinder::INCLUDE_START);

    // can't find a path from start to target
    if(path.empty())
    {
        std::cout << "ScreenGame::ExecuteAIAction - AI " << mCurrPlayerAI
                  << " - CONNECT STRUCTURE FAILED (no path)" << std::endl;

        return false;
    }

    // start conquest
    auto cp = new ConquerPath(unit, mIsoMap, mGameMap, this);
    cp->SetPathCells(path);

    // store active action
    mObjActionsToDo.emplace_back(unit, GameObjectActionType::CONQUER_CELL, onDone);

    mGameMap->ConquerCells(cp);

    return true;
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
    const GameObject * clickObj = clickGameCell.objTop;
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
        SetupStructureConquest(unit, selCell, clickCell, player);
    // object is far -> move close and then try to interact
    else
    {
        Cell2D target = mGameMap->GetAdjacentMoveTarget(selCell, clickObj);

        // failed to find a suitable target
        if(-1 == target.row || -1 == target.col)
            return ;

        SetupUnitMove(unit, selCell, target,
            [this, unit, clickCell, player](bool successful)
            {
                if(successful)
                {
                    const Cell2D currCell(unit->GetRow0(), unit->GetCol0());
                    SetupStructureConquest(unit, currCell, clickCell, player);
                }
            });
    }
}

void ScreenGame::HandleUnitBuildStructureOnMouseUp(Unit * unit, const Cell2D & clickCell)
{
    const int clickInd = clickCell.row * mGameMap->GetNumCols() + clickCell.col;

    Player * player = GetGame()->GetLocalPlayer();

    // destination is visible and walkable
    if(player->IsCellVisible(clickInd) && mGameMap->IsCellWalkable(clickCell.row, clickCell.col))
    {
        const GameMapCell * gmc = unit->GetCell();
        const Cell2D cellUnit(gmc->row, gmc->col);

        const GameObjectTypeId st = unit->GetStructureToBuild();
        const ObjectsDataRegistry * dataReg = GetGame()->GetObjectsRegistry();
        const ObjectBasicData & objData = dataReg->GetObjectData(st);
        const ObjectFactionData & factData = dataReg->GetFactionData(unit->GetFaction(), st);

        // if unit is next to any target cell -> try to build
        const int indRows = objData.rows;
        const int indCols = objData.cols;
        const int r0 = clickCell.row >= indRows ? 1 + clickCell.row - indRows : 0;
        const int c0 = clickCell.col >= indCols ? 1 + clickCell.col - indCols : 0;

        bool next2Target = false;

        for(int r = r0; r <= clickCell.row; ++r)
        {
            for(int c = c0; c <= clickCell.col; ++c)
            {
                next2Target = mGameMap->AreCellsAdjacent(cellUnit, {r, c});

                if(next2Target)
                    break;
            }

            if(next2Target)
                break;
        }

        if(next2Target)
            SetupStructureBuilding(unit, clickCell, player);
        // unit is far -> move close then try to build
        else
        {
            Cell2D target = mGameMap->GetAdjacentMoveTarget(cellUnit, {r0, c0}, clickCell);

            // failed to find a suitable target
            if(-1 == target.row || -1 == target.col)
                return ;

            // add temporary indicator for tower
            mTempStructIndicator = new StructureIndicator(objData, factData);
            mTempStructIndicator->SetFaction(player->GetFaction());

            IsoLayer * layer = mIsoMap->GetLayer(MapLayers::OBJECTS2);
            layer->AddObject(mTempStructIndicator, clickCell.row, clickCell.col);

            // move
            SetupUnitMove(unit, cellUnit, target,
                [this, unit, clickCell, player](bool successful)
            {
                if(successful)
                {
                    const Cell2D currCell(unit->GetRow0(), unit->GetCol0());
                    SetupStructureBuilding(unit, clickCell, player);
                }

                ClearTempStructIndicator();
            });
        }
    }
}

void ScreenGame::HandleUnitBuildWallOnMouseUp(Unit * unit, const Cell2D & clickCell)
{
    const Cell2D unitCell(unit->GetRow0(), unit->GetCol0());
    const int clickInd = clickCell.row * mGameMap->GetNumCols() + clickCell.col;
    const bool diffClick = unitCell != clickCell;

    Player * player = GetGame()->GetLocalPlayer();

    // not clicking on unit cell, destination is visible and walkable
    if(!diffClick || !player->IsCellVisible(clickInd) ||
       !mGameMap->IsCellWalkable(clickCell.row, clickCell.col))
        return ;

    // default is starting pathfinding from unit position
    sgl::ai::Pathfinder::PathOptions po = sgl::ai::Pathfinder::INCLUDE_START;
    unsigned int startR = unitCell.row;
    unsigned int startC = unitCell.col;

    if(!mWallPath.empty())
    {
        // reclicked on same cell of last path -> double click -> finalize path
        if(mWallPath.back() == clickInd)
        {
            const unsigned int minPathSize = 2;

            if(mWallPath.size() > minPathSize)
            {
                auto onFail = [this]
                {
                    mWallPath.clear();
                    ClearCellOverlays();
                };

                auto onDone = [this, unit, onFail](bool successful)
                {
                    if(successful)
                        StartUnitBuildWall(unit);
                    else
                        onFail();
                };

                const unsigned int prevInd = mWallPath[mWallPath.size() - minPathSize];
                const int endR = prevInd / mIsoMap->GetNumCols();
                const int endC = prevInd % mIsoMap->GetNumCols();

                const auto pathMov = mPathfinder->MakePath(unitCell.row, unitCell.col,
                                                           endR, endC);

                // empty path -> exit
                if(pathMov.empty())
                {
                    onFail();
                    return ;
                }

                auto op = new ObjectPath(unit, mIsoMap, mGameMap, this);
                op->SetPathCells(pathMov);

                const bool res = mGameMap->MoveUnit(op);

                // movement failed
                if(!res)
                {
                    onFail();
                    return;
                }

                // disable actions panel (if action is done by local player)
                mHUD->GetPanelObjectActions()->SetActionsEnabled(false);

                // store active action
                mObjActionsToDo.emplace_back(unit, GameObjectActionType::MOVE, onDone);

                unit->SetActiveAction(GameObjectActionType::IDLE);
                unit->SetCurrentAction(GameObjectActionType::MOVE);
            }
            // only 1 block of wall -> no movement, start building
            else
                StartUnitBuildWall(unit);

            return ;
        }
        // continue wall planning
        else
        {
            po = sgl::ai::Pathfinder::NO_OPTION;

            const unsigned int pathInd = mWallPath.back();
            startR = pathInd / mIsoMap->GetNumCols();
            startC = pathInd % mIsoMap->GetNumCols();
        }
    }

    const auto path = mPathfinder->MakePath(startR, startC, clickCell.row, clickCell.col, po);

    // empty path -> nothing to do
    if(path.empty())
        return ;

    mWallPath.reserve(mWallPath.size() + path.size());
    mWallPath.insert(mWallPath.end(), path.begin(), path.end());
}

void ScreenGame::HandleSelectionClick(sgl::core::MouseButtonEvent & event)
{
    Player * player = GetGame()->GetLocalPlayer();

    const sgl::graphic::Camera * cam = mCamController->GetCamera();
    const int worldX = cam->GetScreenToWorldX(event.GetX());
    const int worldY = cam->GetScreenToWorldY(event.GetY());
    const Cell2D clickCell = mIsoMap->CellFromScreenPoint(worldX, worldY);

    // clicked outside the map -> clear current selection
    if(!mIsoMap->IsCellInside(clickCell))
    {
        ClearSelection(player);
        return ;
    }

    // get clicked object, if any
    const GameMapCell & clickGameCell = mGameMap->GetCell(clickCell.row, clickCell.col);
    GameObject * clickObj = clickGameCell.objTop ? clickGameCell.objTop : clickGameCell.objBottom;
    const bool isClickObjOwn = clickObj != nullptr && clickObj->GetFaction() == player->GetFaction();

    // clicked non-own or no object -> nothing to do
    if(!isClickObjOwn)
        return ;

    GameObject * currSel = player->GetSelectedObject();

    // clicked selected object -> deselect it
    if(clickObj == currSel)
    {
        ClearSelection(player);
        return ;
    }

    // normal selection -> clear current selection and select clicked object
    ClearSelection(player);
    SelectObject(clickObj, player);
}

void ScreenGame::HandleActionClick(sgl::core::MouseButtonEvent & event)
{
    Player * player = GetGame()->GetLocalPlayer();

    // no object selected -> nothing to do
    if(!player->HasSelectedObject())
        return ;

    const sgl::graphic::Camera * cam = mCamController->GetCamera();
    const int worldX = cam->GetScreenToWorldX(event.GetX());
    const int worldY = cam->GetScreenToWorldY(event.GetY());

    const Cell2D clickCell = mIsoMap->CellFromScreenPoint(worldX, worldY);

    // clicked outside the map -> nothing to do
    if(!mIsoMap->IsCellInside(clickCell))
        return ;

    GameObject * selObj = player->GetSelectedObject();
    const Cell2D selCell(selObj->GetRow0(), selObj->GetCol0());

    // check if there's a lower object when top is empty
    const GameMapCell & clickGameCell = mGameMap->GetCell(clickCell.row, clickCell.col);
    GameObject * clickObj = clickGameCell.objTop ? clickGameCell.objTop : clickGameCell.objBottom;

    PanelObjectActions * panelObjActions = mHUD->GetPanelObjectActions();

    // selected object is a unit
    if(selObj->GetObjectCategory() == GameObject::CAT_UNIT)
    {
        Unit * selUnit = static_cast<Unit *>(selObj);

        const GameObjectActionType action = selUnit->GetActiveAction();

        if(action == GameObjectActionType::MOVE)
        {
            const bool diffClick = selCell != clickCell;

            // try to move only if clicked on a different cell
            if(diffClick)
                HandleUnitMoveOnMouseUp(selUnit, clickCell);
        }
        else if(action == GameObjectActionType::ATTACK)
            SetupUnitAttack(selUnit, clickObj, player);
        else if(action == GameObjectActionType::HEAL)
            SetupUnitHeal(selUnit, clickObj, player);
        else if(action == GameObjectActionType::CONQUER_CELL)
        {
            const int clickInd = clickCell.row * mGameMap->GetNumCols() + clickCell.col;

            // destination is visible and walkable or conquering unit cell
            if(player->IsCellVisible(clickInd) &&
               (mGameMap->IsCellWalkable(clickCell.row, clickCell.col) || clickCell == selCell))
            {
                // init start for empty path
                sgl::ai::Pathfinder::PathOptions po = sgl::ai::Pathfinder::INCLUDE_START;
                unsigned int startR = selCell.row;
                unsigned int startC = selCell.col;

                // continue building path
                if(!mConquestPath.empty())
                {
                    // reclicked on same cell of last path -> double click -> finalize path
                    if(mConquestPath.back() == clickInd)
                    {
                        // store active action
                        mObjActionsToDo.emplace_back(selUnit, action, [](bool){});

                        // disable action buttons
                        panelObjActions->SetActionsEnabled(false);

                        selUnit->SetActiveAction(GameObjectActionType::IDLE);
                        selUnit->SetCurrentAction(GameObjectActionType::CONQUER_CELL);

                        ClearCellOverlays();

                        // start conquest
                        auto cp = new ConquerPath(selUnit, mIsoMap, mGameMap, this);
                        cp->SetPathCells(mConquestPath);

                        mGameMap->ConquerCells(cp);

                        mConquestPath.clear();

                        return ;
                    }
                    // continue pathfinfing from latest click
                    else
                    {
                        po = sgl::ai::Pathfinder::NO_OPTION;

                        const unsigned int pathInd = mConquestPath.back();
                        startR = pathInd / mIsoMap->GetNumCols();
                        startC = pathInd % mIsoMap->GetNumCols();
                    }
                }

                const auto path = mPathfinder->MakePath(startR, startC, clickCell.row,
                                                        clickCell.col, po);

                // empty path -> nothing to do
                if(path.empty())
                    return ;

                mConquestPath.reserve(mConquestPath.size() + path.size());
                mConquestPath.insert(mConquestPath.end(), path.begin(), path.end());
            }
        }
        else if(action == GameObjectActionType::BUILD_WALL)
            HandleUnitBuildWallOnMouseUp(selUnit, clickCell);
        else if (action == GameObjectActionType::BUILD_STRUCTURE)
            HandleUnitBuildStructureOnMouseUp(selUnit, clickCell);
    }
}

void ScreenGame::StartUnitBuildWall(Unit * unit)
{
    // store active action
    mObjActionsToDo.emplace_back(unit, GameObjectActionType::BUILD_WALL, [](bool){});

    // disable action buttons
    PanelObjectActions * panelObjActions = mHUD->GetPanelObjectActions();
    panelObjActions->SetActionsEnabled(false);

    unit->SetActiveAction(GameObjectActionType::IDLE);
    unit->SetCurrentAction(GameObjectActionType::BUILD_WALL);

    // clear temporary overlay (if action is done by local player)
    if(unit->GetFaction() == GetGame()->GetLocalPlayerFaction())
        ClearCellOverlays();

    // setup build
    auto wbp = new WallBuildPath(unit, mIsoMap, mGameMap, this);
    wbp->SetPathCells(mWallPath);
    // NOTE only level 0 for now
    wbp->SetWallLevel(0);

    mGameMap->BuildWalls(wbp);

    mWallPath.clear();
}

void ScreenGame::ShowActiveIndicators(Unit * unit, const Cell2D & cell)
{
    const GameObjectActionType action = unit->GetActiveAction();

    if(action == GameObjectActionType::MOVE)
        ShowMoveIndicator(unit, cell);
    else if(action == GameObjectActionType::CONQUER_CELL)
        ShowConquestIndicator(unit, cell);
    else if(action == GameObjectActionType::BUILD_WALL)
        ShowBuildWallIndicator(unit, cell);
    else if(action == GameObjectActionType::BUILD_STRUCTURE)
        ShowBuildStructureIndicator(unit, cell);
}

void ScreenGame::ShowAttackIndicators(const GameObject * obj, int range)
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS3);

    const int rows = mIsoMap->GetNumRows();
    const int cols = mIsoMap->GetNumCols();
    const int r0 = obj->GetRow0();
    const int c0 = obj->GetCol0();
    const int rowTL = obj->GetRow1() - range > 0 ? obj->GetRow1() - range : 0;
    const int colTL = obj->GetCol1() - range > 0 ? obj->GetCol1() - range : 0;
    const int rowBR = r0 + range < rows ? r0 + range : rows - 1;
    const int colBR = c0 + range < cols ? c0 + range : cols - 1;

    const int neededInd = (rowBR - rowTL + 1) * (colBR - colTL + 1);
    const int existingInd = mAttIndicators.size();
    const int missingInd = neededInd - existingInd;

    // create missing indicators
    if(missingInd > 0)
    {
        for(int i = 0; i < missingInd; ++i)
            mAttIndicators.push_back(new AttackRangeIndicator);
    }

    // init needed indicators
    const PlayerFaction faction = obj->GetFaction();

    for(int i = 0; i < neededInd; ++i)
    {
        mAttIndicators[i]->SetVisible(true);
        mAttIndicators[i]->SetFaction(faction);
    }

    // hide other indicators
    const int existingInd2 = mAttIndicators.size();

    for(int i = neededInd; i < existingInd2; ++i)
        mAttIndicators[i]->SetVisible(false);

    int ind = 0;

    for(int r = rowTL; r <= rowBR; ++r)
    {
        for(int c = colTL; c <= colBR; ++c)
        {
            if(r != r0 || c != c0)
                layer->AddObject(mAttIndicators[ind++], r, c);
        }
    }
}

void ScreenGame::ShowBuildStructureIndicator(Unit * unit, const Cell2D & currCell)
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS4);

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
                                            sgl::ai::Pathfinder::ALL_OPTIONS);

    if(path.empty())
        layer->ClearObjects();

    Player * player = GetGame()->GetLocalPlayer();

    // get an indicator
    const GameObjectTypeId st = unit->GetStructureToBuild();

    StructureIndicator * ind = nullptr;
    auto it = mStructIndicators.find(st);

    if(it != mStructIndicators.end())
        ind = it->second;
    else
    {
        const ObjectsDataRegistry * dataReg = GetGame()->GetObjectsRegistry();
        const ObjectBasicData & objData = dataReg->GetObjectData(st);
        const ObjectFactionData & factData = dataReg->GetFactionData(unit->GetFaction(), st);
        ind = new StructureIndicator(objData, factData);
        mStructIndicators.emplace(st, ind);
    }

    // add indicator to layer
    layer->AddObject(ind, currCell.row, currCell.col);

    // set visibility
    const int indRows = ind->GetRows();
    const int indCols = ind->GetCols();
    const int r0 = currCell.row >= indRows ? 1 + currCell.row - indRows : 0;
    const int c0 = currCell.col >= indCols ? 1 + currCell.col - indCols : 0;

    bool showIndicator = true;

    for(int r = r0; r <= currCell.row; ++r)
    {
        const int idx0 = r * mGameMap->GetNumCols();

        for(int c = c0; c <= currCell.col; ++c)
        {
            const int idx = idx0 + c;

            showIndicator = player->IsCellVisible(idx) && mGameMap->IsCellWalkable(idx);

            if(!showIndicator)
                break;
        }

        if(!showIndicator)
            break;
    }

    layer->SetObjectVisible(ind, showIndicator);

    // indicator visible -> update faction
    if(showIndicator)
        ind->SetFaction(player->GetFaction());
}

void ScreenGame::ShowConquestIndicator(Unit * unit, const Cell2D & dest)
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS2);

    // first clear all objects from the layer
    layer->ClearObjects();

    const bool currInside = mIsoMap->IsCellInside(dest);

    // mouse outside the map
    if(!currInside)
        return ;

    const int currInd = dest.row * mGameMap->GetNumCols() + dest.col;

    Player * player = GetGame()->GetLocalPlayer();

    const bool currVisible = player->IsCellVisible(currInd);
    const bool currWalkable = mGameMap->IsCellWalkable(currInd);
    const bool currIsUnitCell = dest.row == unit->GetRow0() && dest.col == unit->GetCol0();

    const bool canConquer = currVisible && (currWalkable || currIsUnitCell);

    if(!canConquer)
        return ;

    sgl::ai::Pathfinder::PathOptions po;
    unsigned int startR;
    unsigned int startC;

    // start pathfinding from unit position
    if(mConquestPath.empty())
    {
        po = sgl::ai::Pathfinder::INCLUDE_START;

        startR = unit->GetRow0();
        startC = unit->GetCol0();
    }
    // continue pathfinfing from latest click
    else
    {
        po = sgl::ai::Pathfinder::NO_OPTION;

        const unsigned int pathInd = mConquestPath.back();
        startR = pathInd / mIsoMap->GetNumCols();
        startC = pathInd % mIsoMap->GetNumCols();
    }

    // show path cost when destination is visible
    const auto path = mPathfinder->MakePath(startR, startC, dest.row, dest.col, po);

    // this should never happen, but just in case
    if(path.empty() && mConquestPath.empty())
        return ;

    std::vector<unsigned int> totPath;
    totPath.reserve(mConquestPath.size() + path.size());

    totPath = mConquestPath;
    totPath.insert(totPath.end(), path.begin(), path.end());

    const unsigned int lastIdx = totPath.size() - 1;

    const PlayerFaction faction = player->GetFaction();

    for(unsigned int i = 0; i < totPath.size(); ++i)
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
        const unsigned int pathInd = totPath[i];
        const unsigned int indRow = pathInd / mIsoMap->GetNumCols();
        const unsigned int indCol = pathInd % mIsoMap->GetNumCols();

        layer->AddObject(ind, indRow, indCol);

        ind->SetFaction(faction);
        ind->ShowCost(i == lastIdx);
    }

    ConquerPath cp(unit, mIsoMap, mGameMap, this);
    cp.SetPathCells(totPath);

    mConquestIndicators[lastIdx]->SetCost(cp.GetPathCost());
}

void ScreenGame::ShowBuildWallIndicator(Unit * unit, const Cell2D & dest)
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS2);

    // first clear all objects from the layer
    layer->ClearObjects();

    const bool currInside = mIsoMap->IsCellInside(dest);

    // mouse outside the map
    if(!currInside)
        return ;

    const int currInd = dest.row * mGameMap->GetNumCols() + dest.col;

    Player * player = GetGame()->GetLocalPlayer();

    const bool currVisible = player->IsCellVisible(currInd);
    const bool currWalkable = mGameMap->IsCellWalkable(currInd);
    const bool currIsUnitCell = dest.row == unit->GetRow0() && dest.col == unit->GetCol0();

    const bool canBuild = currVisible && (currWalkable || currIsUnitCell);

    if(!canBuild)
        return ;

    sgl::ai::Pathfinder::PathOptions po;
    unsigned int startR;
    unsigned int startC;

    // start pathfinding from unit position
    if(mWallPath.empty())
    {
        po = sgl::ai::Pathfinder::INCLUDE_START;

        startR = unit->GetRow0();
        startC = unit->GetCol0();
    }
    // continue pathfinfing from latest click
    else
    {
        po = sgl::ai::Pathfinder::NO_OPTION;

        const unsigned int pathInd = mWallPath.back();
        startR = pathInd / mIsoMap->GetNumCols();
        startC = pathInd % mIsoMap->GetNumCols();
    }

    // show path cost when destination is visible
    const auto path = mPathfinder->MakePath(startR, startC, dest.row, dest.col, po);

    // this should not happen
    if(path.size() < 2 && mWallPath.empty())
        return ;

    std::vector<unsigned int> totPath;
    totPath.reserve(mWallPath.size() + path.size());

    totPath = mWallPath;
    totPath.insert(totPath.end(), path.begin(), path.end());

    const unsigned int lastIdx = totPath.size() - 1;

    const PlayerFaction faction = player->GetFaction();

    std::vector<Cell2D> cellsPath;
    cellsPath.reserve(totPath.size());

    // store coordinates of start cell
    const unsigned int pathInd0 = totPath[0];
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
        const unsigned int pathInd = totPath[i + 1];
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
    wbp.SetPathCells(totPath);
    wbp.SetIndicatorsType(cellsPath, mWallIndicators);

    mWallIndicators[lastIndicator]->SetCost(wbp.GetEnergyCost(), wbp.GetMateriaCost());
}

void ScreenGame::ShowHealingIndicators(const GameObject * obj, int range)
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS3);

    const int rows = mIsoMap->GetNumRows();
    const int cols = mIsoMap->GetNumCols();
    const int r0 = obj->GetRow0();
    const int c0 = obj->GetCol0();
    const int rowTL = obj->GetRow1() - range > 0 ? obj->GetRow1() - range : 0;
    const int colTL = obj->GetCol1() - range > 0 ? obj->GetCol1() - range : 0;
    const int rowBR = r0 + range < rows ? r0 + range : rows - 1;
    const int colBR = c0 + range < cols ? c0 + range : cols - 1;

    const int neededInd = (rowBR - rowTL + 1) * (colBR - colTL + 1);
    const int existingInd = mHealIndicators.size();
    const int missingInd = neededInd - existingInd;

    // create missing indicators
    if(missingInd > 0)
    {
        for(int i = 0; i < missingInd; ++i)
            mHealIndicators.push_back(new HealingRangeIndicator);
    }

    // init needed indicators
    const PlayerFaction faction = obj->GetFaction();

    for(int i = 0; i < neededInd; ++i)
    {
        mHealIndicators[i]->SetVisible(true);
        mHealIndicators[i]->SetFaction(faction);
    }

    // hide other indicators
    const int existingInd2 = mHealIndicators.size();

    for(int i = neededInd; i < existingInd2; ++i)
        mHealIndicators[i]->SetVisible(false);

    int ind = 0;

    for(int r = rowTL; r <= rowBR; ++r)
    {
        for(int c = colTL; c <= colBR; ++c)
        {
            if(r != r0 || c != c0)
                layer->AddObject(mHealIndicators[ind++], r, c);
        }
    }
}

void ScreenGame::ShowMoveIndicator(GameObject * obj, const Cell2D & dest)
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS2);

    // mouse outside the map
    if(!mIsoMap->IsCellInside(dest))
    {
        // hide the indicator, if any
        if(mMoveInd != nullptr)
            layer->SetObjectVisible(mMoveInd, false);

        return ;
    }

    // move indicator already created -> move it and continue
    if(mMoveInd != nullptr)
        layer->MoveObject(mMoveInd->GetRow(), mMoveInd->GetCol(), dest.row, dest.col);
    // create new move indicator
    else
    {
        mMoveInd = new MoveIndicator;
        layer->AddObject(mMoveInd, dest.row, dest.col);
    }

    const int destInd = dest.row * mGameMap->GetNumCols() + dest.col;

    Player * player = GetGame()->GetLocalPlayer();

    const bool destVisible = player->IsCellVisible(destInd);
    const bool destVisited = mGameMap->IsCellObjectVisited(destInd);
    const bool destWalkable = mGameMap->IsCellWalkable(destInd);

    const bool showIndicator = (!destVisible && !destVisited) || destWalkable;

    layer->SetObjectVisible(mMoveInd, showIndicator);

    // stop here if not showing indicator
    if(!showIndicator)
        return ;

    if(destVisible)
    {
        // set indicator type
        mMoveInd->SetIndicatorType(MoveIndicator::NORMAL);

        // show path cost when destination is visible
        const auto path = mPathfinder->MakePath(obj->GetRow0(), obj->GetCol0(),
                                                dest.row, dest.col,
                                                sgl::ai::Pathfinder::ALL_OPTIONS);

        ObjectPath op(obj, mIsoMap, mGameMap, this);
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

void ScreenGame::ClearCellOverlays()
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS2);
    layer->ClearObjects();

    layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS3);
    layer->ClearObjects();

    layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS4);
    layer->ClearObjects();

    // delete move indicator
    delete mMoveInd;
    mMoveInd = nullptr;
}

void ScreenGame::ClearTempStructIndicator()
{
    if(mTempStructIndicator)
    {
        IsoLayer * layer = mIsoMap->GetLayer(MapLayers::OBJECTS2);
        layer->ClearObject(mTempStructIndicator);

        delete mTempStructIndicator;
        mTempStructIndicator = nullptr;
    }
}

void ScreenGame::CenterCameraOverPlayerBase()
{
    const Player * p = GetGame()->GetLocalPlayer();
    const Cell2D & cell = p->GetBaseCell();
    const sgl::core::Pointd2D pos = mIsoMap->GetCellPosition(cell.row, cell.col);
    const int cX = pos.x + mIsoMap->GetTileWidth() * 0.5f;
    const int cY = pos.y + mIsoMap->GetTileHeight() * 0.5f;

    mCamController->CenterCameraToPoint(cX, cY);
}

void ScreenGame::UpdateCurrentCell()
{
    // NOTE this could be moved to cam->SetFunctionOnMove if things get more complicated
    // for now I am calling it manually to minimize calls
    const sgl::graphic::Camera * cam = mCamController->GetCamera();
    const int worldX = cam->GetScreenToWorldX(mMousePos.x);
    const int worldY = cam->GetScreenToWorldY(mMousePos.y);

    const Cell2D cell = mIsoMap->CellFromScreenPoint(worldX, worldY);

    if(cell == mCurrCell)
        return ;

    mCurrCell = cell;

    // react to change of cell like if mouse was moved
    GameObject * sel = GetGame()->GetLocalPlayer()->GetSelectedObject();

    if(sel != nullptr && sel->GetObjectCategory() == GameObject::CAT_UNIT)
        ShowActiveIndicators(static_cast<Unit *>(sel), cell);
}

} // namespace game
