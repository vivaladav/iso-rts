#include "Screens/ScreenGame.h"

#include "CameraMapController.h"
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
#include "GameObjects/DefensiveTower.h"
#include "GameObjects/Unit.h"
#include "GameObjects/WallGate.h"
#include "Indicators/AttackRangeIndicator.h"
#include "Indicators/ConquestIndicator.h"
#include "Indicators/MoveIndicator.h"
#include "Indicators/StructureIndicator.h"
#include "Indicators/WallIndicator.h"
#include "Particles/UpdaterDamage.h"
#include "Particles/UpdaterSingleLaser.h"
#include "Widgets/ButtonMinimap.h"
#include "Widgets/ButtonQuickUnitSelection.h"
#include "Widgets/CellProgressBar.h"
#include "Widgets/DialogNewElement.h"
#include "Widgets/MiniMap.h"
#include "Widgets/PanelObjectActions.h"
#include "Widgets/PanelGameOver.h"
#include "Widgets/PanelGameWon.h"
#include "Widgets/PanelResources.h"

#include <sgl/ai/Pathfinder.h>
#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/core/event/MouseButtonEvent.h>
#include <sgl/core/event/MouseMotionEvent.h>
#include <sgl/graphic/Camera.h>
#include <sgl/graphic/ParticlesManager.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/ButtonsGroup.h>
#include <sgl/sgui/Stage.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace game
{

// NOTE these will be replaced by dynamic values soon
constexpr float TIME_NEW_UNIT = 0.5f;
constexpr float TIME_CONQ_RES_GEN = 0.5f;
constexpr float TIME_UPG_UNIT = 1.f;

constexpr float TIME_ENERGY_USE = 2.f;
constexpr float TIME_AI_MOVE = 0.5f;

ScreenGame::ScreenGame(Game * game)
    : Screen(game)
    , mPartMan(new sgl::graphic::ParticlesManager)
    , mPathfinder(new sgl::ai::Pathfinder)
    , mPrevCell(-1, -1)
    , mTimerEnergy(TIME_ENERGY_USE)
    , mTimerAI(TIME_AI_MOVE)
{
    game->SetClearColor(0x0F, 0x0F, 0x0F, 0xFF);

    game->AddKeyboardListener(this);

    const int rendW = sgl::graphic::Renderer::Instance()->GetWidth();
    const int rendH = sgl::graphic::Renderer::Instance()->GetHeight();

    auto cam = sgl::graphic::Camera::GetDefaultCamera();
    cam->SetSize(rendW, rendH);
    mCamController = new CameraMapController(cam);

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
    mIsoMap->SetVisibleArea(cam->GetX(), cam->GetY(), cam->GetWidth(), cam->GetHeight());

    cam->SetFunctionOnMove([this]
    {
        const sgl::graphic::Camera * cam = mCamController->GetCamera();
        const int camX0 = cam->GetX();
        const int camY0 = cam->GetY();
        const int camW = cam->GetWidth();
        const int camH = cam->GetHeight();
        const int camX1 = camX0 + camW;
        const int camY1 = camY0 + camH;

        // update map
        mIsoMap->SetVisibleArea(camX0, camY0, camW, camH);

        // update MiniMap
        Cell2D cellTL = mIsoMap->CellFromScreenPoint(camX0, camY0);
        Cell2D cellTR = mIsoMap->CellFromScreenPoint(camX1, camY0);
        Cell2D cellBL = mIsoMap->CellFromScreenPoint(camX0, camY1);
        Cell2D cellBR = mIsoMap->CellFromScreenPoint(camX1, camY1);

        mMiniMap->SetCameraCells(cellTL, cellTR, cellBL, cellBR);
    });

    // set camera limits
    const sgl::core::Pointd2D p0 = mIsoMap->GetCellPosition(0, 0);
    const sgl::core::Pointd2D p1 = mIsoMap->GetCellPosition(mIsoMap->GetNumRows() - 1, 0);
    const sgl::core::Pointd2D p2 = mIsoMap->GetCellPosition(mIsoMap->GetNumRows() - 1, mIsoMap->GetNumCols() - 1);
    const sgl::core::Pointd2D p3 = mIsoMap->GetCellPosition(0, mIsoMap->GetNumCols() - 1);
    const int tileW = mIsoMap->GetTileWidth();
    const int tileH = mIsoMap->GetTileHeight();
    const int marginCameraH = tileW;
    const int marginCameraV = tileH * 2;

    const int cameraL = p1.x - marginCameraH;
    const int cameraR = p3.x + tileW + marginCameraH - rendW;
    const int cameraT = p0.y - marginCameraV;
    const int cameraB = p2.y + tileH + marginCameraV - rendH;

    mCamController->SetLimits(cameraL, cameraR, cameraT, cameraB);

    // init pathfinder
    mPathfinder->SetMap(mGameMap, mGameMap->GetNumRows(), mGameMap->GetNumCols());

    // -- PLAYERS --
//    for(int i = 0; i < GetGame()->GetNumPlayers(); ++i)
//    {
//        // temporary disable AI for development
//        Player * p = game->GetPlayerByIndex(i);

//        if(p->IsAI())
//        {
//            p->GetAI()->SetGameMap(mGameMap);
//            mAiPlayers.push_back(p);
//        }
//    }

    // LOCAL PLAYER
    Player * localPlayer = game->GetLocalPlayer();

    // react to local player changes in stats
    localPlayer->SetOnResourcesChanged([this]
    {
        if(mDialogNewElement != nullptr)
            mDialogNewElement->CheckBuild();
    });

    // apply initial visibility to the game map
    mGameMap->ApplyLocalVisibility();

    // UI
    CreateUI();

    // set initial camera position
    CenterCameraOverPlayerBase();
}

ScreenGame::~ScreenGame()
{
    delete mPartMan;

    for(auto ind : mAttIndicators)
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

void ScreenGame::Update(float delta)
{
    // handle Widgets scheduled for deletion
    for(auto w : mWidgetsToDelete)
        delete w;

    mWidgetsToDelete.clear();

    // do nothing when paused
    if(mPaused)
        return ;

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
    const int rendW = sgl::graphic::Renderer::Instance()->GetWidth();
    const int rendH = sgl::graphic::Renderer::Instance()->GetHeight();

    auto panel = new PanelGameOver(GetGame());

    const int x = (rendW - panel->GetWidth()) * 0.5f;
    const int y = rendH * 0.25f;

    panel->SetPosition(x, y);
}

void ScreenGame::GameWon()
{
    const int rendW = sgl::graphic::Renderer::Instance()->GetWidth();
    const int rendH = sgl::graphic::Renderer::Instance()->GetHeight();

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

void ScreenGame::SetObjectActionCompleted(GameObject * obj)
{
    auto it = mActiveObjActions.begin();

    // search selected object in active actions
    while(it != mActiveObjActions.end())
    {
        if(it->obj == obj)
        {
            // remove pending action
            mActiveObjActions.erase(it);

            // re-enable actions panel
            mPanelObjActions->SetActionsEnabled(true);

            // reset object's active action to default
            obj->SetActiveActionToDefault();
            // reset current action to idle
            obj->SetCurrentAction(IDLE);

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

    HidePanelObjActions();

    player->ClearSelectedObject();

    mPanelObjActions->ClearObject();
    mPanelObjActions->SetVisible(false);

    ClearCellOverlays();

    ClearNewElemDialog();
}

void ScreenGame::SelectObject(GameObject * obj, Player * player)
{
    obj->SetSelected(true);

    const GameObjectType got = obj->GetObjectType();

    // update quick selection buttons when selected unit
    if(OBJ_UNIT == got)
    {
        const int numButtons = mGroupQuickUnitSel->GetNumButtons();

        for(int i = 0; i < numButtons; ++i)
        {
            auto b = static_cast<ButtonQuickUnitSelection *>(mGroupQuickUnitSel->GetButton(i));
            Unit * unit = b->GetUnit();

            if(unit == obj)
            {
                b->SetChecked(true);
                break;
            }
        }
    }
    // not a unit
    else
    {
        const int checked = mGroupQuickUnitSel->GetIndexChecked();

        if(checked != -1)
            mGroupQuickUnitSel->GetButton(checked)->SetChecked(false);


        // show attack range overlay for towers
        if(OBJ_DEF_TOWER == got)
        {
            auto tower = static_cast<DefensiveTower *>(obj);
            const int range = tower->GetAttackRange();
            ShowAttackIndicators(tower, range);
        }
    }

    mPanelObjActions->SetObject(obj);
    mPanelObjActions->SetVisible(true);
    sgl::sgui::Stage::Instance()->SetFocus();

    player->SetSelectedObject(obj);

    // check actions panel status
    const bool idle = obj->GetCurrentAction() == IDLE;
    mPanelObjActions->SetActionsEnabled(idle);
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
}

void ScreenGame::InitParticlesSystem()
{
    sgl::graphic::ParticlesUpdater * updater;

    // DAMAGE
    updater = new UpdaterDamage;
    mPartMan->RegisterUpdater(PU_DAMAGE, updater);

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
    mIsoMap->CreateLayer(MapLayers::OBJECTS1);
    mIsoMap->CreateLayer(MapLayers::OBJECTS2);
    mIsoMap->CreateLayer(MapLayers::CELL_OVERLAYS4);
}

void ScreenGame::CreateUI()
{
    const int rendW = sgl::graphic::Renderer::Instance()->GetWidth();
    const int rendH = sgl::graphic::Renderer::Instance()->GetHeight();

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
        if(mDialogNewElement != nullptr)
            return ;

        const std::vector<ObjectData> & unitsData = player->GetAvailableUnits();
        mDialogNewElement = new DialogNewElement(unitsData, "CREATE NEW UNIT", player);
        mDialogNewElement->SetFocus();

        mDialogNewElement->SetFunctionOnClose([this]
        {
            ClearNewElemDialog();
        });

        mDialogNewElement->SetFunctionOnBuild([this, player]
        {
            const auto type = static_cast<UnitType>(mDialogNewElement->GetSelectedData().objType);
            SetupNewUnit(type, player->GetSelectedObject(), player);

            ClearNewElemDialog();
        });

        // position dialog
        const int rendW = sgl::graphic::Renderer::Instance()->GetWidth();
        const int posX = (rendW - mDialogNewElement->GetWidth()) * 0.5f;
        const int posY = mPanelObjActions->GetY() - mDialogNewElement->GetHeight();
        mDialogNewElement->SetPosition(posX, posY);
    });

    // UNIT ACTIONS
    // build structure
    mPanelObjActions->SetButtonFunction(PanelObjectActions::BTN_BUILD_STRUCT, [this, player]
    {
        if(mDialogNewElement != nullptr)
            return ;

        const std::vector<ObjectData> & unitsData = player->GetAvailableStructures();
        mDialogNewElement = new DialogNewElement(unitsData, "CREATE NEW STRUCTURE", player);
        mDialogNewElement->SetFocus();

        // set unit's action to idle while dialog is open
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionId::IDLE);

        mDialogNewElement->SetFunctionOnClose([this, unit]
        {
            unit->SetActiveActionToDefault();
            ClearNewElemDialog();
        });

        mDialogNewElement->SetFunctionOnBuild([this, unit]
        {
            unit->SetActiveAction(GameObjectActionId::BUILD_STRUCTURE);

            const auto stype = static_cast<StructureType>(mDialogNewElement->GetSelectedData().objType);
            unit->SetStructureToBuild(stype);

            ClearNewElemDialog();
        });

        // position dialog
        const int rendW = sgl::graphic::Renderer::Instance()->GetWidth();
        const int posX = (rendW - mDialogNewElement->GetWidth()) * 0.5f;
        const int posY = mPanelObjActions->GetY() - mDialogNewElement->GetHeight();
        mDialogNewElement->SetPosition(posX, posY);

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

        // show attack range overlay
        const int range = unit->GetAttackRange();
        ShowAttackIndicators(unit, range);
    });

    // conquer
    mPanelObjActions->SetButtonFunction(PanelObjectActions::BTN_CONQUER_CELL, [this, player]
    {
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionId::CONQUER_CELL);

        ClearCellOverlays();

        mConquestPath.clear();
    });

    // move
    mPanelObjActions->SetButtonFunction(PanelObjectActions::BTN_MOVE, [this, player]
    {
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionId::MOVE);

        ClearCellOverlays();
    });

    // WALL GATE
    mPanelObjActions->SetButtonFunction(PanelObjectActions::BTN_OPEN_GATE, [this, player]
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
        mPanelObjActions->SetObject(gate);

        // reset focus as buttons will change
        sgl::sgui::Stage::Instance()->SetFocus();
    });

    mPanelObjActions->SetButtonFunction(PanelObjectActions::BTN_CLOSE_GATE, [this, player]
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
        mPanelObjActions->SetObject(gate);

        // reset focus as buttons will change
        sgl::sgui::Stage::Instance()->SetFocus();
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
            GameObjectAction & act = *it;

            if(act.obj == selObj)
            {
                const GameObjectType objType = act.obj->GetObjectType();
                const GameObjectActionId objActId = act.actId;

                // object is a Base
                if(objType == OBJ_BASE)
                {
                    // building a new unit
                    if(objActId == GameObjectActionId::BUILD_UNIT)
                    {
                        CancelProgressBar(act.actionCell);

                        selObj->SetCurrentAction(GameObjectActionId::IDLE);
                        selObj->SetBusy(false);
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
                    else if(objActId == GameObjectActionId::ATTACK)
                    {
                        auto unit = static_cast<Unit *>(selObj);
                        unit->ClearAttackTarget();
                    }

                    selObj->SetCurrentAction(GameObjectActionId::IDLE);
                }

                mActiveObjActions.erase(it);

                // re-enable actions
                mPanelObjActions->SetActionsEnabled(true);
                selObj->SetActiveActionToDefault();

                break;
            }
            else
                ++it;
        }
    });

    // CREATE QUICK UNIT SELECTION BUTTONS
    mGroupQuickUnitSel = new sgl::sgui::ButtonsGroup(sgl::sgui::ButtonsGroup::HORIZONTAL);

    const int numButtons = player->GetMaxUnits();

    for(int i = 0; i < numButtons; ++i)
    {
        auto b = new ButtonQuickUnitSelection(i, this);
        mGroupQuickUnitSel->AddButton(b);
    }

    const int groupX = (rendW - mGroupQuickUnitSel->GetWidth()) * 0.5f;
    const int groupY = rendH - mGroupQuickUnitSel->GetHeight();
    mGroupQuickUnitSel->SetPosition(groupX, groupY);

    player->SetOnNumUnitsChanged([this, player]
    {
        const int numUnits = player->GetNumUnits();
        const int maxUnits = player->GetMaxUnits();

        for(int i = 0; i < numUnits; ++i)
        {
            auto b = static_cast<ButtonQuickUnitSelection *>(mGroupQuickUnitSel->GetButton(i));
            b->SetUnit(player->GetUnit(i));
        }

        for(int i = numUnits; i < maxUnits; ++i)
        {
            auto b = static_cast<ButtonQuickUnitSelection *>(mGroupQuickUnitSel->GetButton(i));
            b->ClearUnit();
        }
    });

    // MINIMAP
    mButtonMinimap = new ButtonMinimap;
    mButtonMinimap->SetX(rendW - mButtonMinimap->GetWidth());

    mButtonMinimap->AddOnClickFunction([this]
    {
        mButtonMinimap->SetVisible(false);
        mMiniMap->SetVisible(true);
    });

    mMiniMap = new MiniMap(mCamController, mIsoMap);
    mMiniMap->SetVisible(false);
    mMiniMap->SetX(rendW - mMiniMap->GetWidth());

    mMiniMap->AddFunctionOnClose([this]
    {
        mButtonMinimap->SetVisible(true);
        mMiniMap->SetVisible(false);
    });

    auto objs = mGameMap->GetVisibleObjects(0, 0, mIsoMap->GetNumRows(), mIsoMap->GetNumCols());

    for(GameObject * obj : objs)
    {
        const Player * p = obj->GetOwner();
        PlayerFaction faction = NO_FACTION;
        MiniMap::MiniMapElemType type = MiniMap::MME_SCENE;

        if(p != nullptr)
        {
            faction = p->GetFaction();
            type = static_cast<MiniMap::MiniMapElemType>(MiniMap::MME_FACTION1 + faction);
        }
        else if(obj->CanBeConquered())
            type = MiniMap::MME_CONQUERABLE;

        mMiniMap->AddElement(obj->GetRow0(), obj->GetCol0(), obj->GetRows(), obj->GetCols(),
                             type, faction);
    }
}

void ScreenGame::HidePanelObjActions()
{
    mPanelObjActions->ClearObject();
    mPanelObjActions->SetVisible(false);
}

void ScreenGame::ClearNewElemDialog()
{
    // no dialog -> nothing to do
    if(nullptr == mDialogNewElement)
        return ;

    // schedule dialog deletion
    mWidgetsToDelete.push_back(mDialogNewElement);
    mDialogNewElement = nullptr;
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

    // GAME
    const int key = event.GetKey();

    Player * p = GetGame()->GetLocalPlayer();

    // P -> PAUSE
    if(key == KeyboardEvent::KEY_P)
    {
        mPaused = !mPaused;

        // disable actions panel when paused
        mPanelObjActions->SetEnabled(!mPaused);
    }
    // SHIFT + B -> center camera on own base
    else if(key == KeyboardEvent::KEY_B)
    {
        if(event.IsModShiftDown())
            CenterCameraOverPlayerBase();
    }
    // DEBUG: ALT + U -> toggle UI
    else if(event.IsModAltDown() && key == KeyboardEvent::KEY_U)
    {
        auto stage = sgl::sgui::Stage::Instance();
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
}

void ScreenGame::OnMouseButtonUp(sgl::core::MouseButtonEvent & event)
{
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
    // CAMERA
    mCamController->HandleMouseMotion(event);

    const sgl::graphic::Camera * cam = mCamController->GetCamera();
    const int worldX = cam->GetScreenToWorldX(event.GetX());
    const int worldY = cam->GetScreenToWorldY(event.GetY());

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
        else if(action == GameObjectActionId::BUILD_STRUCTURE)
            HandleUnitBuildStructureOnMouseMove(selUnit, currCell);
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
    const ObjectData & data = player->GetAvailableUnit(type);

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

    gen->SetActiveAction(GameObjectActionId::IDLE);
    gen->SetCurrentAction(GameObjectActionId::BUILD_UNIT);

    // create and init progress bar
    CellProgressBar * pb = CreateProgressBar(cell, TIME_NEW_UNIT, player->GetFaction());

    pb->SetFunctionOnCompleted([this, cell, player, gen, data]
    {
        gen->SetCurrentAction(GameObjectActionId::IDLE);

        mGameMap->CreateUnit(data, gen, cell, player);
        mProgressBarsToDelete.emplace_back(CellToIndex(cell));

        const PlayerFaction faction = player->GetFaction();
        const MiniMap::MiniMapElemType type = static_cast<MiniMap::MiniMapElemType>(MiniMap::MME_FACTION1 + faction);

        mMiniMap->AddElement(cell.row, cell.col, data.rows, data.cols, type, faction);

        SetObjectActionCompleted(gen);
    });

    // store active action
    mActiveObjActions.emplace_back(gen, GameObjectActionId::BUILD_UNIT, cell);

    gen->SetActiveAction(GameObjectActionId::IDLE);
    gen->SetCurrentAction(GameObjectActionId::BUILD_UNIT);

    // disable actions panel
    mPanelObjActions->SetActionsEnabled(false);

    return true;
}

bool ScreenGame::SetupStructureConquest(Unit * unit, const Cell2D & start, const Cell2D & end, Player * player)
{
    // check if conquest is possible
    if(!mGameMap->CanConquerStructure(unit, end, player))
        return false;

    // start conquest
    mGameMap->StartConquerStructure(start, end, player);

    // create and init progress bar
    CellProgressBar * pb = CreateProgressBar(start, TIME_CONQ_RES_GEN, player->GetFaction());

    pb->SetFunctionOnCompleted([this, start, end, player, unit]
    {
        mGameMap->ConquerStructure(start, end, player);
        mProgressBarsToDelete.emplace_back(CellToIndex(start));

        unit->ConsumeEnergy(CONQUER_STRUCTURE);

        const GameMapCell & cellStruct = mGameMap->GetCell(end.row, end.col);
        const GameObject * objStruct = cellStruct.objTop;

        const PlayerFaction faction = player->GetFaction();
        const MiniMap::MiniMapElemType type = static_cast<MiniMap::MiniMapElemType>(MiniMap::MME_FACTION1 + faction);

        mMiniMap->UpdateElement(objStruct->GetRow0(), objStruct->GetCol0(),
                                objStruct->GetRows(), objStruct->GetCols(), type, faction);

        // clear action data once the action is completed
        SetObjectActionCompleted(unit);
    });

    // store active action
    const GameMapCell & targetCell = mGameMap->GetCell(end.row, end.col);

    mActiveObjActions.emplace_back(unit, targetCell.objTop, GameObjectActionId::CONQUER_STRUCTURE, start);

    unit->SetActiveAction(GameObjectActionId::IDLE);
    unit->SetCurrentAction(GameObjectActionId::CONQUER_STRUCTURE);

    // disable actions panel
    mPanelObjActions->SetActionsEnabled(false);

    return true;
}

bool ScreenGame::SetupStructureBuilding(Unit * unit, const Cell2D & cellTarget, Player * player)
{
    const StructureType st = unit->GetStructureToBuild();
    const ObjectData & data = player->GetAvailableStructure(st);

    // check if building is possible
    if(!mGameMap->CanBuildStructure(unit, cellTarget, player, data))
        return false;

    mGameMap->StartBuildStructure(cellTarget, player, data);

    // create and init progress bar
    // TODO get time from unit
    CellProgressBar * pb = CreateProgressBar(cellTarget, TIME_CONQ_RES_GEN, player->GetFaction());

    pb->SetFunctionOnCompleted([this, unit, cellTarget, player, data]
    {
        mGameMap->BuildStructure(cellTarget, player, data);
        mProgressBarsToDelete.emplace_back(CellToIndex(cellTarget));

        unit->ConsumeEnergy(BUILD_STRUCTURE);

        const PlayerFaction faction = player->GetFaction();
        const MiniMap::MiniMapElemType type = static_cast<MiniMap::MiniMapElemType>(MiniMap::MME_FACTION1 + faction);

        mMiniMap->AddElement(cellTarget.row, cellTarget.col, data.rows, data.cols, type, faction);

        // clear action data once the action is completed
        SetObjectActionCompleted(unit);
    });

    // store active action
    mActiveObjActions.emplace_back(unit, GameObjectActionId::BUILD_STRUCTURE, cellTarget);

    // disable action buttons
    mPanelObjActions->SetActionsEnabled(false);

    unit->SetActiveAction(GameObjectActionId::IDLE);
    unit->SetCurrentAction(GameObjectActionId::BUILD_STRUCTURE);
    unit->ClearStructureToBuild();

    ClearCellOverlays();

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
                                            sgl::ai::Pathfinder::ALL_OPTIONS);

    // empty path -> exit
    if(path.empty())
        return ;

    auto op = new ObjectPath(unit, mIsoMap, mGameMap, this);
    op->SetPathCells(path);
    op->SetOnCompleted(onCompleted);

    const bool res = mGameMap->MoveUnit(op);

    // movement failed
    if(!res)
        return ;

    ClearCellOverlays();

    mPanelObjActions->SetActionsEnabled(false);

    // store active action
    mActiveObjActions.emplace_back(unit, GameObjectActionId::MOVE);

    unit->SetActiveAction(GameObjectActionId::IDLE);
    unit->SetCurrentAction(GameObjectActionId::MOVE);
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
                                                sgl::ai::Pathfinder::ALL_OPTIONS);

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
    const auto path = mPathfinder->MakePath(startR, startC, currCell.row, currCell.col, po);

    // this should never happen, but just in case
    if(path.empty())
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

void ScreenGame::HandleUnitBuildStructureOnMouseMove(Unit * unit, const Cell2D & currCell)
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
    const StructureType st = unit->GetStructureToBuild();

    StructureIndicator * ind = nullptr;
    auto it = mStructIndicators.find(st);

    if(it != mStructIndicators.end())
        ind = it->second;
    else
    {
        const ObjectData & data = player->GetAvailableStructure(st);
        ind = new StructureIndicator(data);
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

        SetupUnitMove(unit, selCell, target, [this, unit, clickCell, player]
        {
            const Cell2D currCell(unit->GetRow0(), unit->GetCol0());

            SetupStructureConquest(unit, currCell, clickCell, player);
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

        const StructureType st = unit->GetStructureToBuild();
        const ObjectData & data = player->GetAvailableStructure(st);

        // if unit is next to any target cell -> try to build
        const int indRows = data.rows;
        const int indCols = data.cols;
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
            mTempStructIndicator = new StructureIndicator(data);
            mTempStructIndicator->SetFaction(player->GetFaction());

            IsoLayer * layer = mIsoMap->GetLayer(MapLayers::OBJECTS2);
            layer->AddObject(mTempStructIndicator, clickCell.row, clickCell.col);

            // move
            SetupUnitMove(unit, cellUnit, target, [this, unit, clickCell, player]
            {
                const Cell2D currCell(unit->GetRow0(), unit->GetCol0());

                SetupStructureBuilding(unit, clickCell, player);

                // get rid of temporary indicator
                if(mTempStructIndicator)
                {
                    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::OBJECTS2);
                    layer->ClearObject(mTempStructIndicator);

                    delete mTempStructIndicator;
                    mTempStructIndicator = nullptr;
                }
            });
        }
    }
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
    const bool isClickObjOwn = clickObj != nullptr && clickObj->GetOwner() == player;

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
            const bool res = selUnit->SetAttackTarget(clickObj);

            if(!res)
                return ;

            selUnit->SetActiveAction(GameObjectActionId::IDLE);
            selUnit->SetCurrentAction(GameObjectActionId::ATTACK);

            // disable action buttons
            mPanelObjActions->SetActionsEnabled(false);

            mActiveObjActions.emplace_back(selUnit, GameObjectActionId::ATTACK);
        }
        else if(action == GameObjectActionId::CONQUER_CELL)
        {
            const int clickInd = clickCell.row * mGameMap->GetNumCols() + clickCell.col;

            // destination is visible and walkable or conquering unit cell
            if(player->IsCellVisible(clickInd) &&
               (mGameMap->IsCellWalkable(clickCell.row, clickCell.col) || clickCell == selCell))
            {
                if(!mConquestPath.empty())
                {
                    // reclicked on same cell of last path -> double click -> finalize path
                    if(mConquestPath.back() == clickInd)
                    {
                        std::cout << "DOUBLE CLICK on " << clickInd << std::endl;
                        std::cout << "TOTAL PATH size: " << mConquestPath.size() << std::endl;

                        // store active action
                        mActiveObjActions.emplace_back(selUnit, action);

                        // disable action buttons
                        mPanelObjActions->SetActionsEnabled(false);

                        selUnit->SetActiveAction(GameObjectActionId::IDLE);
                        selUnit->SetCurrentAction(GameObjectActionId::CONQUER_CELL);

                        ClearCellOverlays();

                        // start conquest
                        auto cp = new ConquerPath(selUnit, mIsoMap, mGameMap, this);
                        cp->SetPathCells(mConquestPath);

                        mGameMap->ConquerCells(cp);

                        mConquestPath.clear();

                        return ;
                    }
                }

                sgl::ai::Pathfinder::PathOptions po;
                unsigned int startR;
                unsigned int startC;

                // start pathfinding from unit position
                if(mConquestPath.empty())
                {
                    po = sgl::ai::Pathfinder::INCLUDE_START;

                    startR = selCell.row;
                    startC = selCell.col;
                }
                // continue pathfinfing from latest click
                else
                {
                    po = sgl::ai::Pathfinder::NO_OPTION;

                    const unsigned int pathInd = mConquestPath.back();
                    startR = pathInd / mIsoMap->GetNumCols();
                    startC = pathInd % mIsoMap->GetNumCols();
                }

                auto path = mPathfinder->MakePath(startR, startC,
                                                  clickCell.row, clickCell.col, po);

                // empty path -> nothing to do
                if(path.empty())
                    return ;

                std::cout << "ADDING PATH of size " << path.size()
                          << " to existing conquest path of size " << mConquestPath.size() << std::endl;

                mConquestPath.reserve(mConquestPath.size() + path.size());
                mConquestPath.insert(mConquestPath.end(), path.begin(), path.end());
            }
        }
        else if(action == GameObjectActionId::BUILD_WALL)
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
                    // store active action
                    mActiveObjActions.emplace_back(selUnit, action);

                    // disable action buttons
                    mPanelObjActions->SetActionsEnabled(false);

                    selUnit->SetActiveAction(GameObjectActionId::IDLE);
                    selUnit->SetCurrentAction(GameObjectActionId::BUILD_WALL);

                    // clear temporary overlay
                    ClearCellOverlays();

                    // setup build
                    auto wbp = new WallBuildPath(selUnit, mIsoMap, mGameMap, this);
                    wbp->SetPathCells(path);
                    // NOTE only level 0 for now
                    wbp->SetWallLevel(0);

                    mGameMap->BuildWalls(wbp);
                }
            }
        }
        else if (action == GameObjectActionId::BUILD_STRUCTURE)
            HandleUnitBuildStructureOnMouseUp(selUnit, clickCell);
    }
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
    const PlayerFaction faction = obj->GetOwner()->GetFaction();

    for(unsigned int i = 0; i < neededInd; ++i)
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

void ScreenGame::CenterCameraOverPlayerBase()
{
    const Player * p = GetGame()->GetLocalPlayer();
    const Cell2D & cell = p->GetBaseCell();
    const sgl::core::Pointd2D pos = mIsoMap->GetCellPosition(cell.row, cell.col);
    const int cX = pos.x + mIsoMap->GetTileWidth() * 0.5f;
    const int cY = pos.y + mIsoMap->GetTileHeight() * 0.5f;

    mCamController->CenterCameraToPoint(cX, cY);
}

} // namespace game
