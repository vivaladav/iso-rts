#include "GameHUD.h"

#include "ControlMap.h"
#include "Game.h"
#include "GameMap.h"
#include "IsoObject.h"
#include "Player.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Structure.h"
#include "GameObjects/Unit.h"
#include "Screens/ScreenGame.h"
#include "Widgets/ButtonMinimap.h"
#include "Widgets/ButtonQuickUnitSelection.h"
#include "Widgets/CountdownLabel.h"
#include "Widgets/DialogEndMission.h"
#include "Widgets/DialogExit.h"
#include "Widgets/DialogNewElement.h"
#include "Widgets/MiniMap.h"
#include "Widgets/PanelObjectActions.h"
#include "Widgets/PanelResources.h"

#include <sgl/graphic/Renderer.h>
#include <sgl/sgui/ButtonsGroup.h>

namespace game
{

GameHUD::GameHUD(CameraMapController * camController, IsoMap * isoMap,
                 ScreenGame * screen, GameMap * gameMap)
    : mGame(screen->GetGame())
    , mGameMap(gameMap)
    , mScreen(screen)
{
    using namespace sgl;

    const int rendW = graphic::Renderer::Instance()->GetWidth();
    const int rendH = graphic::Renderer::Instance()->GetHeight();

    SetSize(rendW, rendH);

    // LOCAL PLAYER
    Player * player = mGame->GetLocalPlayer();

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

    mMiniMap = new MiniMap(camController, isoMap, this);
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
    Player * player = mGame->GetLocalPlayer();

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
    // stats
    const PlayerFaction pf = mGame->GetLocalPlayerFaction();
    const int territory = mGameMap->GetControlMap()->GetPercentageControlledByFaction(pf);
    const unsigned int killed = mGameMap->GetEnemiesKilled(pf);
    const unsigned int casualties = mGameMap->GetCasualties(pf);
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
    auto renderer = sgl::graphic::Renderer::Instance();
    const int rendW = renderer->GetWidth();
    const int rendH = renderer->GetHeight();
    const int posX = (rendW - dialog->GetWidth()) / 2;
    const int posY = (rendH - dialog->GetHeight()) / 2;
    dialog->SetPosition(posX, posY);
}

void GameHUD::ShowDialogExit()
{
    if(mDialogExit != nullptr)
        return ;

    mDialogExit = new DialogExit(mGame, mScreen);
    mDialogExit->SetFocus();

    mDialogExit->SetFunctionOnClose([this]
                                    {
                                        // schedule dialog deletion
                                        mDialogExit->DeleteLater();
                                        mDialogExit = nullptr;
                                    });

    // position dialog
    auto renderer = sgl::graphic::Renderer::Instance();
    const int rendW = renderer->GetWidth();
    const int rendH = renderer->GetHeight();
    const int posX = (rendW - mDialogExit->GetWidth()) / 2;
    const int posY = (rendH - mDialogExit->GetHeight()) / 2;
    mDialogExit->SetPosition(posX, posY);
}

void GameHUD::ShowDialogNewElement(unsigned int type)
{
    if(mDialogNewElement != nullptr)
        return;

    Player * player = mGame->GetLocalPlayer();

    auto t = static_cast<DialogNewElement::ElemType>(type);
    mDialogNewElement = new DialogNewElement(t, player, mGame->GetObjectsRegistry());
    mDialogNewElement->SetFocus();

    if(DialogNewElement::ETYPE_STRUCTURES == t)
    {
        // set unit's action to idle while dialog is open
        auto unit = static_cast<Unit *>(player->GetSelectedObject());
        unit->SetActiveAction(GameObjectActionId::IDLE);

        mDialogNewElement->SetFunctionOnBuild([this, unit]
        {
            unit->SetActiveAction(GameObjectActionId::BUILD_STRUCTURE);

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
    auto renderer = sgl::graphic::Renderer::Instance();
    const int posX = (renderer->GetWidth() - mDialogNewElement->GetWidth()) / 2;
    const int posY = (renderer->GetHeight() - mDialogNewElement->GetHeight()) / 2;
    mDialogNewElement->SetPosition(posX, posY);
}

void GameHUD::HideDialogNewElement()
{
    // no dialog -> nothing to do
    if(nullptr == mDialogNewElement)
        return ;

    // schedule dialog deletion
    mDialogNewElement->DeleteLater();
    mDialogNewElement = nullptr;
}

void GameHUD::ShowMissionCountdown(int secs)
{
    const Player * p = mGame->GetLocalPlayer();
    const PlayerFaction pf = p->GetFaction();
    const auto bases = p->GetStructuresByType(GameObject::TYPE_BASE);

    // this shouldn't happen
    if(bases.empty())
        return ;

    const Structure * base = bases[0];
    const IsoObject * isoObj = base->GetIsoObject();

    mCountdownLabel = new CountdownLabel(pf, secs);

    const int x0 = isoObj->GetX() + (isoObj->GetWidth() - mCountdownLabel->GetWidth()) / 2;
    const int y0 = isoObj->GetY() - mCountdownLabel->GetHeight();

    mCountdownLabel->SetPosition(x0, y0);
}

void GameHUD::HideMissionCountdown()
{
    delete mCountdownLabel;
    mCountdownLabel = nullptr;
}

} // namespace game
