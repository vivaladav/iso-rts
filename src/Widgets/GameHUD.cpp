#include "GameHUD.h"

#include "Player.h"
#include "Screens/ScreenGame.h"
#include "Widgets/ButtonMinimap.h"
#include "Widgets/ButtonQuickUnitSelection.h"
#include "Widgets/DialogExit.h"
#include "Widgets/MiniMap.h"
#include "Widgets/PanelObjectActions.h"
#include "Widgets/PanelResources.h"

#include <sgl/graphic/Renderer.h>
#include <sgl/sgui/ButtonsGroup.h>

namespace game
{

GameHUD::GameHUD(Player * player, CameraMapController * camController,
                 IsoMap * isoMap, ScreenGame * screen)
    : mGame(screen->GetGame())
    , mScreen(screen)
{
    using namespace sgl;

    const int rendW = graphic::Renderer::Instance()->GetWidth();
    const int rendH = graphic::Renderer::Instance()->GetHeight();

    SetSize(rendW, rendH);

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

void GameHUD::HidePanelObjActions()
{
    mPanelObjActions->ClearObject();
    mPanelObjActions->SetVisible(false);
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

} // namespace game
