#include "GameHUD.h"

#include "Cell2D.h"
#include "ControlMap.h"
#include "Game.h"
#include "GameMap.h"
#include "IsoMap.h"
#include "IsoObject.h"
#include "Player.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Structure.h"
#include "GameObjects/Temple.h"
#include "GameObjects/Unit.h"
#include "Screens/ScreenGame.h"
#include "Widgets/ButtonMinimap.h"
#include "Widgets/ButtonQuickUnitSelection.h"
#include "Widgets/CountdownLabel.h"
#include "Widgets/DialogEndMission.h"
#include "Widgets/DialogExit.h"
#include "Widgets/DialogExploreTemple.h"
#include "Widgets/GameMapProgressBar.h"
#include "Widgets/DialogNewElement.h"
#include "Widgets/MiniMap.h"
#include "Widgets/PanelObjectActions.h"
#include "Widgets/PanelResources.h"

#include <sgl/graphic/Renderer.h>
#include <sgl/sgui/ButtonsGroup.h>

namespace game
{

GameHUD::GameHUD(ScreenGame * screen)
    : mScreen(screen)
{
    using namespace sgl;

    const int rendW = graphic::Renderer::Instance()->GetWidth();
    const int rendH = graphic::Renderer::Instance()->GetHeight();

    SetSize(rendW, rendH);

    // LOCAL PLAYER
    Player * player = mScreen->GetGame()->GetLocalPlayer();

    // react to local player changes in stats
    player->SetOnResourcesChanged([this]
    {
        if(mDialogNewElement != nullptr)
            mDialogNewElement->CheckBuild();
    });

    // TOP RESOURCE BAR
    mPanelRes = new PanelResources(player, this);
    mPanelRes->SetX((rendW - mPanelRes->GetWidth()) / 2);

    // MINIMAP
    mButtonMinimap = new ButtonMinimap(this);
    mButtonMinimap->SetX(rendW - mButtonMinimap->GetWidth());

    mButtonMinimap->AddOnClickFunction([this]
    {
        mButtonMinimap->SetVisible(false);
        mMiniMap->SetVisible(true);
    });

    mMiniMap = new MiniMap(mScreen->mCamController, mScreen->mIsoMap, this);
    mMiniMap->SetVisible(false);
    mMiniMap->SetX(rendW - mMiniMap->GetWidth());

    mMiniMap->AddFunctionOnClose([this]
    {
        mButtonMinimap->SetVisible(true);
        mMiniMap->SetVisible(false);
    });

    // QUICK UNIT SELECTION BUTTONS
    mGroupUnitSel = new sgl::sgui::ButtonsGroup(sgl::sgui::ButtonsGroup::HORIZONTAL, this);

    const int numButtons = player->GetMaxUnits();

    for(int i = 0; i < numButtons; ++i)
    {
        auto b = new ButtonQuickUnitSelection(i, screen);
        mGroupUnitSel->AddButton(b);
    }

    const int groupX = (rendW - mGroupUnitSel->GetWidth()) * 0.5f;
    const int groupY = rendH - mGroupUnitSel->GetHeight();
    mGroupUnitSel->SetPosition(groupX, groupY);

    player->SetOnNumUnitsChanged([this, player]
    {
        const int numUnits = player->GetNumUnits();
        const int maxUnits = player->GetMaxUnits();

        for(int i = 0; i < numUnits; ++i)
        {
            auto b = static_cast<ButtonQuickUnitSelection *>(mGroupUnitSel->GetButton(i));
            b->SetUnit(player->GetUnit(i));
        }

        for(int i = numUnits; i < maxUnits; ++i)
        {
            auto b = static_cast<ButtonQuickUnitSelection *>(mGroupUnitSel->GetButton(i));
            b->ClearUnit();
        }
    });

    // OBJECT ACTIONS
    mPanelObjActions = new PanelObjectActions(this);
    mPanelObjActions->SetVisible(false);
}

GameHUD::~GameHUD()
{
    Player * player = mScreen->GetGame()->GetLocalPlayer();

    player->SetOnNumUnitsChanged([]{});
    player->SetOnResourcesChanged([]{});
}

void GameHUD::SetMiniMapEnabled(bool val)
{
    if(mMiniMap->IsEnabled() == val)
        return ;

    mMiniMap->SetEnabled(val);
    mMiniMap->SetVisible(val);

    mButtonMinimap->SetEnabled(val);
    mButtonMinimap->SetVisible(false);
}

void GameHUD::HidePanelObjActions()
{
    mPanelObjActions->ClearObject();
    mPanelObjActions->SetVisible(false);
}

void GameHUD::ShowDialogEndMission(bool won)
{
    mScreen->SetPause(true);

    // stats
    GameMap * gm = mScreen->mGameMap;

    const PlayerFaction pf = mScreen->GetGame()->GetLocalPlayerFaction();
    const int territory = gm->GetControlMap()->GetPercentageControlledByFaction(pf);
    const unsigned int killed = gm->GetEnemiesKilled(pf);
    const unsigned int casualties = gm->GetCasualties(pf);
    const unsigned int played = mScreen->GetPlayTimeInSec();

    // create dialog
    auto dialog = new DialogEndMission(played, territory, killed, pf, won);
    dialog->SetFocus();

    dialog->SetFunctionOnClose([this, dialog, won]
    {
                                   dialog->DeleteLater();

        if(won)
            mScreen->HandleGameWon();
        else
            mScreen->HandleGameOver();
    });

    // position dialog
    CenterWidget(dialog);
}

void GameHUD::ShowDialogExit()
{
    if(mDialogExit != nullptr)
        return ;

    mScreen->SetPause(true);

    mDialogExit = new DialogExit(mScreen->GetGame(), mScreen);
    mDialogExit->SetFocus();

    mDialogExit->SetFunctionOnShowingDialogSettings([this]
    {
        // keep game paused
        mScreen->SetPause(true);
    });

    mDialogExit->SetFunctionOnHidingDialogSettings([this]
    {
        // un-pause game
        mScreen->SetPause(false);
    });

    mDialogExit->SetFunctionOnClose([this]
    {
        // schedule dialog deletion
        mDialogExit->DeleteLater();
        mDialogExit = nullptr;

        // un-pause game
        mScreen->SetPause(false);
    });

    // position dialog
    CenterWidget(mDialogExit);
}

void GameHUD::ShowDialogExploreTemple(Player * player, Temple * temple)
{
    if(mDialogExploreTemple != nullptr)
        return ;

    mScreen->SetPause(true);

    mDialogExploreTemple = new DialogExploreTemple(player, temple);

    mDialogExploreTemple->SetFunctionOnCancel([this]
    {
        HideDialogExploreTemple();
    });

    mDialogExploreTemple->SetFunctionOnExplore([this, player, temple]
    {
        HideDialogExploreTemple();

        // spend allocated resources
        player->SumResource(Player::Stat::MONEY, -temple->GetInvestedMoney());
        player->SumResource(Player::Stat::MATERIAL, -temple->GetInvestedMaterial());
        player->SumResource(Player::Stat::BLOBS, -temple->GetInvestedBlobs());
        player->SumResource(Player::Stat::DIAMONDS, -temple->GetInvestedDiamonds());

        // start progress bar
        const int time = temple->GetExplorationTime();
        const Cell2D cell(temple->GetRow1(), temple->GetCol1());
        const PlayerFaction faction = player->GetFaction();

        GameMapProgressBar * pb = CreateProgressBarInCell(cell, time, faction);

        pb->AddFunctionOnCompleted([this, player, temple]
        {
            temple->Explore();

            mScreen->CenterCameraOverObject(temple);

            ShowDialogExploreTempleOutcome(player, temple);
        });
    });

    // position dialog
    CenterWidget(mDialogExploreTemple);
}

void GameHUD::HideDialogExploreTemple()
{
    if(nullptr == mDialogExploreTemple)
        return ;

    // delete dialog
    mDialogExploreTemple->DeleteLater();
    mDialogExploreTemple = nullptr;

    // un-pause game
    mScreen->SetPause(false);
}

void GameHUD::ShowDialogNewElement(unsigned int type)
{
    if(mDialogNewElement != nullptr)
        return;

    mScreen->SetPause(true);

    Game * game = mScreen->GetGame();
    Player * player = game->GetLocalPlayer();

    auto t = static_cast<DialogNewElement::ElemType>(type);
    mDialogNewElement = new DialogNewElement(t, player, game->GetObjectsRegistry());
    mDialogNewElement->SetFocus();

    if(DialogNewElement::ETYPE_STRUCTURES == t)
    {
        // set unit's action to idle while dialog is open
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionType::IDLE);

        mDialogNewElement->SetFunctionOnBuild([this, unit]
        {
            unit->SetActiveAction(GameObjectActionType::BUILD_STRUCTURE);

            const GameObjectTypeId stype = mDialogNewElement->GetSelectedType();
            unit->SetStructureToBuild(stype);

            HideDialogNewElement();
        });

        mDialogNewElement->SetFunctionOnClose([this, unit]
        {
            unit->SetActiveActionToDefault();
            HideDialogNewElement();
        });
    }
    else
    {
        mDialogNewElement->SetFunctionOnBuild([this, player]
        {
            const GameObjectTypeId type = mDialogNewElement->GetSelectedType();
            mScreen->SetupNewUnit(type, player->GetSelectedObject(), player);

            HideDialogNewElement();
        });

        mDialogNewElement->SetFunctionOnClose([this]
        {
            HideDialogNewElement();
        });
    }

    // position dialog
    CenterWidget(mDialogNewElement);
}

void GameHUD::HideDialogNewElement()
{
    // no dialog -> nothing to do
    if(nullptr == mDialogNewElement)
        return ;

    // schedule dialog deletion
    mDialogNewElement->DeleteLater();
    mDialogNewElement = nullptr;

    // un-pause game
    mScreen->SetPause(false);
}

void GameHUD::ShowMissionCountdown(int secs)
{
    const Player * p = mScreen->GetGame()->GetLocalPlayer();
    const PlayerFaction pf = p->GetFaction();
    const auto bases = p->GetStructuresByType(GameObject::TYPE_BASE);

    // this shouldn't happen
    if(bases.empty())
        return ;

    const Structure * base = bases[0];
    const IsoObject * isoObj = base->GetIsoObject();

    mCountdownLabel = new CountdownLabel(pf, secs, this);

    const int x0 = isoObj->GetX() + (isoObj->GetWidth() - mCountdownLabel->GetWidth()) / 2;
    const int y0 = isoObj->GetY() - mCountdownLabel->GetHeight();

    mCountdownLabel->SetPosition(x0, y0);
}

void GameHUD::HideMissionCountdown()
{
    delete mCountdownLabel;
    mCountdownLabel = nullptr;
}

GameMapProgressBar * GameHUD::CreateProgressBarInCell(const Cell2D & cell, float time, PlayerFaction faction)
{
    GameMapProgressBar * pb = CreateProgressBar(time, faction);

    // set position
    IsoMap * isoMap = mScreen->mIsoMap;
    auto posCell = isoMap->GetCellPosition(cell.row, cell.col);
    const int pbX = posCell.x + (isoMap->GetTileWidth() - pb->GetWidth()) * 0.5f;
    const int pbY = posCell.y + (isoMap->GetTileHeight() * 0.75f - pb->GetHeight());
    pb->SetPosition(pbX, pbY);

    // progress bar visibility depends on local player's visibility map
    pb->SetVisible(mScreen->mGameMap->IsCellVisibleToLocalPlayer(cell.row, cell.col));

    return pb;
}

void GameHUD::ShowDialogExploreTempleOutcome(Player * player, Temple * temple)
{
    if(mDialogExploreTempleOutcome != nullptr)
        return ;

    mScreen->SetPause(true);

    mDialogExploreTempleOutcome = new DialogExploreTempleOutcome(player, temple);

    mDialogExploreTempleOutcome->SetFunctionOnClose([this]
    {
        HideDialogExploreTempleOutcome();
    });

    mDialogExploreTempleOutcome->SetFunctionOnOutcome1([this, player, temple]
    {
        HideDialogExploreTempleOutcome();

        const unsigned int outcome = temple->GetExplorationOutcome1();
        mScreen->mGameMap->HandleTempleExplorationOutcome(outcome, player, temple);
    });

    mDialogExploreTempleOutcome->SetFunctionOnOutcome2([this, player, temple]
    {
        HideDialogExploreTempleOutcome();

        const unsigned int outcome = temple->GetExplorationOutcome2();
        mScreen->mGameMap->HandleTempleExplorationOutcome(outcome, player, temple);
    });

    // position dialog
    CenterWidget(mDialogExploreTempleOutcome);
}

void GameHUD::HideDialogExploreTempleOutcome()
{
    // no dialog -> nothing to do
    if(nullptr == mDialogExploreTempleOutcome)
        return ;

    // schedule dialog deletion
    mDialogExploreTempleOutcome->DeleteLater();
    mDialogExploreTempleOutcome = nullptr;

    // un-pause game
    mScreen->SetPause(false);
}

GameMapProgressBar * GameHUD::CreateProgressBar(float time, PlayerFaction faction)
{
    GameMapProgressBar * pb = new GameMapProgressBar(faction, time, this);

    // schedule to delete progress bar when done
    pb->AddFunctionOnCompleted([pb]
    {
        pb->DeleteLater();
    });

    return pb;
}

void GameHUD::CenterWidget(sgl::sgui::Widget * w)
{
    auto renderer = sgl::graphic::Renderer::Instance();
    const int posX = (renderer->GetWidth() - w->GetWidth()) / 2;
    const int posY = (renderer->GetHeight() - w->GetHeight()) / 2;
    w->SetPosition(posX, posY);
}

} // namespace game
