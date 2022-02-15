#include "Screens/ScreenGame.h"

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
#include "Particles/UpdaterDamage.h"
#include "Particles/UpdaterSingleLaser.h"
#include "Widgets/ButtonQuickUnitSelection.h"
#include "Widgets/CellProgressBar.h"
#include "Widgets/DialogNewElement.h"
#include "Widgets/GameUIData.h"
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

constexpr int SCROLL_L = -1;
constexpr int SCROLL_R = 1;
constexpr int SCROLL_U = -1;
constexpr int SCROLL_D = 1;
constexpr int NO_SCROLL = 0;

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

    mCamera = sgl::graphic::Camera::GetDefaultCamera();
    mCamera->SetSize(rendW, rendH);

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
    mIsoMap->SetVisibleArea(mCamera->GetX(), mCamera->GetY(),
                            mCamera->GetWidth(), mCamera->GetHeight());

    mCamera->SetFunctionOnMove([this]
    {
        mIsoMap->SetVisibleArea(mCamera->GetX(), mCamera->GetY(),
                                mCamera->GetWidth(), mCamera->GetHeight());
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

    mCameraLimitL = p1.x - marginCameraH;
    mCameraLimitR = p3.x + tileW + marginCameraH - rendW;
    mCameraLimitT = p0.y - marginCameraV;
    mCameraLimitB = p2.y + tileH + marginCameraV - rendH;

    // init pathfinder
    mPathfinder->SetMap(mGameMap, mGameMap->GetNumRows(), mGameMap->GetNumCols());

    // -- PLAYERS --
    for(int i = 0; i < GetGame()->GetNumPlayers(); ++i)
    {
        Player * p = game->GetPlayerByIndex(i);

        // add start resources
        const int startEnergy = 400;
        p->SumResource(Player::Stat::ENERGY, startEnergy);

        const int startMaterial = 50;
        p->SumResource(Player::Stat::MATERIAL, startMaterial);

        const int startMoney = 500;
        p->SumResource(Player::Stat::MONEY, startMoney);

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

    // -- UPDATE CAMERA --
    const float cameraSpeed = 400.f;

    const float movX = mCameraDirX * cameraSpeed * delta;

    if(mCameraDirX < 0)
    {
        const int newX = static_cast<int>(movX - 0.5f) + mCamera->GetX();

        if(newX < mCameraLimitL)
            mCamera->SetX(mCameraLimitL);
        else
            mCamera->MoveX(movX);
    }
    else if(mCameraDirX > 0)
    {
        const int newX = static_cast<int>(movX + 0.5f) + mCamera->GetX();

        if(newX > mCameraLimitR)
            mCamera->SetX(mCameraLimitR);
        else
            mCamera->MoveX(movX);
    }

    const float movY = mCameraDirY * cameraSpeed * delta;

    if(mCameraDirY < 0)
    {
        const int newY = static_cast<int>(movY - 0.5f) + mCamera->GetY();

        if(newY < mCameraLimitT)
            mCamera->SetY(mCameraLimitT);
        else
            mCamera->MoveY(movY);
    }
    else if(mCameraDirY > 0)
    {
        const int newY = static_cast<int>(movY + 0.5f) + mCamera->GetY();

        if(newY > mCameraLimitB)
            mCamera->SetY(mCameraLimitB);
        else
            mCamera->MoveY(movY);
    }

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

    if(selObj != nullptr)
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

    // update quick selection buttons when selected unit
    if(obj->GetObjectType() == OBJ_UNIT)
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
    }

    mPanelObjActions->SetObject(obj);
    mPanelObjActions->SetVisible(true);
    sgl::sgui::Stage::Instance()->SetFocus();

    player->SetSelectedObject(obj);

    // check actions panel status
    const bool idle = obj->GetCurrentAction() == IDLE;
    mPanelObjActions->SetActionsEnabled(idle);
}

void ScreenGame::CenterCameraOverObject(GameObject * obj)
{
    const GameMapCell * cell = obj->GetCell();
    const sgl::core::Pointd2D pos = mIsoMap->GetCellPosition(cell->row, cell->col);
    const int cX = pos.x + mIsoMap->GetTileWidth() * 0.5f;
    const int cY = pos.y + mIsoMap->GetTileHeight() * 0.5f;

    mCamera->CenterToPoint(cX, cY);
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

    mIsoMap->CreateLayer(MapLayers::OBJECTS);

    mIsoMap->CreateLayer(MapLayers::CELL_OVERLAYS3);
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
                        unit->SetAttackTarget(nullptr);
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
    using namespace sgl::core;

    const int key = event.GetKey();

    if(key == KeyboardEvent::KEY_LEFT)
    {
        if(!mCameraMouseScrollX)
        {
            mCameraDirX = SCROLL_L;
            mCameraKeyScrollX = true;
        }
    }
    else if(key == KeyboardEvent::KEY_RIGHT)
    {
        if(!mCameraMouseScrollX)
        {
            mCameraDirX = SCROLL_R;
            mCameraKeyScrollX = true;
        }
    }
    else if(key == KeyboardEvent::KEY_UP)
    {
        if(!mCameraMouseScrollY)
        {
            mCameraDirY = SCROLL_U;
            mCameraKeyScrollY = true;
        }
    }
    else if(key == KeyboardEvent::KEY_DOWN)
    {
        if(!mCameraMouseScrollY)
        {
            mCameraDirY = SCROLL_D;
            mCameraKeyScrollY = true;
        }
    }
}

void ScreenGame::OnKeyUp(sgl::core::KeyboardEvent & event)
{
    using namespace sgl::core;

    const int key = event.GetKey();

    Player * p = GetGame()->GetLocalPlayer();

    // P -> PAUSE
    if(key == KeyboardEvent::KEY_P)
    {
        mPaused = !mPaused;

        // disable actions panel when paused
        mPanelObjActions->SetEnabled(!mPaused);
    }
    // ARROWS -> move camera
    else if(key == KeyboardEvent::KEY_LEFT)
    {
        if(!mCameraMouseScrollX)
            mCameraDirX = NO_SCROLL;

        mCameraKeyScrollX = false;
    }
    else if(key == KeyboardEvent::KEY_RIGHT)
    {
        if(!mCameraMouseScrollX)
            mCameraDirX = NO_SCROLL;

        mCameraKeyScrollX = false;
    }
    else if(key == KeyboardEvent::KEY_UP)
    {
        if(!mCameraMouseScrollY)
            mCameraDirY = NO_SCROLL;

        mCameraKeyScrollY = false;
    }
    else if(key == KeyboardEvent::KEY_DOWN)
    {
        if(!mCameraMouseScrollY)
            mCameraDirY = NO_SCROLL;

        mCameraKeyScrollY = false;
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
    // DEBUG: SHIFT/CTRL + R -> add/remove resources
    else if(key == KeyboardEvent::KEY_R)
    {
        if(event.IsModShiftDown())
        {
            p->SumResource(Player::Stat::ENERGY, 100);
            p->SumResource(Player::Stat::MATERIAL, 50);
        }
        else if(event.IsModCtrlDown())
        {
            p->SumResource(Player::Stat::ENERGY, -50);
            p->SumResource(Player::Stat::MATERIAL, -50);
        }
    }
}

void ScreenGame::OnMouseButtonUp(sgl::core::MouseButtonEvent & event)
{
    // no interaction while game is paused
    if(mPaused)
        return ;

    // handle only LEFT click
    if(event.GetButton() != sgl::core::MouseEvent::BUTTON_LEFT)
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

    // DEBUG
    std::cout << "CLICK: " << clickCell.row << "," << clickCell.col << std::endl;

    const GameMapCell & clickGameCell = mGameMap->GetCell(clickCell.row, clickCell.col);
    GameObject * clickObj = clickGameCell.obj;
    const bool isClickObjOwn = clickObj != nullptr && clickObj->GetOwner() == player;

    if(hasSelected)
    {
        GameObject * selObj = player->GetSelectedObject();
        const Cell2D selCell(selObj->GetRow0(), selObj->GetCol0());

        // clear selection when clicking on selected object again
        if(clickObj == selObj)
        {
            ClearSelection(player);
            return ;
        }

        // selected object is a unit
        if(selObj->GetObjectType() == OBJ_UNIT)
        {
            Unit * selUnit = static_cast<Unit *>(selObj);

            const GameObjectActionId action = selUnit->GetActiveAction();

            // select another own object when not attacking
            if(action != GameObjectActionId::ATTACK && isClickObjOwn && clickObj != selObj)
            {
                ClearSelection(player);
                SelectObject(clickObj, player);
                return ;
            }

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
                if(nullptr == clickObj)
                    return ;

                selUnit->SetAttackTarget(clickObj);

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

                        // disable action buttons
                        mPanelObjActions->SetActionsEnabled(false);

                        selUnit->SetActiveAction(GameObjectActionId::IDLE);
                        selUnit->SetCurrentAction(GameObjectActionId::CONQUER_CELL);

                        ClearCellOverlays();
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
        // selected object is not a unit
        else
        {
            // select another own object
            if(isClickObjOwn && clickObj != selObj)
            {
                ClearSelection(player);
                SelectObject(clickObj, player);
            }
        }
    }
    // no object currently selected
    else
    {
        if(isClickObjOwn)
            SelectObject(clickObj, player);
    }
}

void ScreenGame::OnMouseMotion(sgl::core::MouseMotionEvent & event)
{
    const int screenX = event.GetX();
    const int screenY = event.GetY();
    const int rendW = sgl::graphic::Renderer::Instance()->GetWidth();
    const int rendH = sgl::graphic::Renderer::Instance()->GetHeight();

    // -- handle scrolling --
    const int scrollingMargin = 5;

    if(screenX < scrollingMargin)
    {
        if(!mCameraKeyScrollX)
        {
            mCameraDirX = SCROLL_L;
            mCameraMouseScrollX = true;
        }
    }
    else if(screenX > (rendW - scrollingMargin))
    {
        if(!mCameraKeyScrollX)
        {
            mCameraDirX = SCROLL_R;
            mCameraMouseScrollX = true;
        }
    }
    else if(!mCameraKeyScrollX)
    {
        mCameraDirX = NO_SCROLL;
        mCameraMouseScrollX = false;
    }

    if(screenY < scrollingMargin)
    {
        if(!mCameraKeyScrollY)
        {
            mCameraDirY = SCROLL_U;
            mCameraMouseScrollY = true;
        }
    }
    else if(screenY > (rendH - scrollingMargin))
    {
        if(!mCameraKeyScrollY)
        {
            mCameraDirY = SCROLL_D;
            mCameraMouseScrollY = true;
        }
    }
    else if(!mCameraKeyScrollY)
    {
        mCameraDirY = NO_SCROLL;
        mCameraMouseScrollY = false;
    }

    // -- handle movement over cells --
    const int worldX = mCamera->GetScreenToWorldX(screenX);
    const int worldY = mCamera->GetScreenToWorldY(screenY);

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

        // clear action data once the action is completed
        SetObjectActionCompleted(unit);
    });

    // store active action
    const GameMapCell & targetCell = mGameMap->GetCell(end.row, end.col);

    mActiveObjActions.emplace_back(unit, targetCell.obj, GameObjectActionId::CONQUER_STRUCTURE, start);

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
    if(!mGameMap->CanBuildStructure(cellTarget, player, data))
        return false;

    mGameMap->StartBuildStructure(cellTarget, player, data);

    // create and init progress bar
    // TODO get time from unit
    CellProgressBar * pb = CreateProgressBar(cellTarget, TIME_CONQ_RES_GEN, player->GetFaction());

    pb->SetFunctionOnCompleted([this, unit, cellTarget, player, data]
    {
        mGameMap->BuildStructure(cellTarget, player, data);
        mProgressBarsToDelete.emplace_back(CellToIndex(cellTarget));

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

void ScreenGame::HandleUnitBuildStructureOnMouseMove(Unit * unit, const Cell2D & currCell)
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

            IsoLayer * layer = mIsoMap->GetLayer(MapLayers::OBJECTS);
            layer->AddObject(mTempStructIndicator, clickCell.row, clickCell.col);

            // move
            SetupUnitMove(unit, cellUnit, target, [this, unit, clickCell, player]
            {
                const Cell2D currCell(unit->GetRow0(), unit->GetCol0());

                SetupStructureBuilding(unit, clickCell, player);

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

void ScreenGame::CenterCameraOverPlayerBase()
{
    const Player * p = GetGame()->GetLocalPlayer();
    const Cell2D & cell = p->GetBaseCell();
    const sgl::core::Pointd2D pos = mIsoMap->GetCellPosition(cell.row, cell.col);
    const int cX = pos.x + mIsoMap->GetTileWidth() * 0.5f;
    const int cY = pos.y + mIsoMap->GetTileHeight() * 0.5f;

    mCamera->CenterToPoint(cX, cY);
}

} // namespace game
