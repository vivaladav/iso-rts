#include "Widgets/PanelPlayer.h"

#include "GameConstants.h"
#include "GameMapCell.h"
#include "Player.h"
#include "GameObjects/Unit.h"
#include "Widgets/ButtonPanelPlayer.h"
#include "Widgets/ButtonsPanel.h"
#include "Widgets/UnitsSelector.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <sgui/Label.h>

#include <iomanip>
#include <iostream>
#include <sstream>

namespace game
{

const unsigned int colorHeader = 0x888888FF;

PanelPlayer::PanelPlayer(Player * player, lib::sgui::Widget * parent)
    : lib::sgui::Widget(parent)
    , mPlayer(player)
{
    // -- subpanels --
    CreatePanelBase();
    mPanelCell->SetVisible(false);

    CreatePanelUnit();
    mPanelUnits->SetVisible(false);

    // -- position elements --
    const int panelX = 10;
    const int panelY = 0;

    // sub-panel cell buttons
    mPanelCell->SetPosition(panelX, panelY);

    // sub-panel unit buttons
    mPanelUnits->SetPosition(panelX, panelY);
}

void PanelPlayer::SetPanelCellVisible(bool val) { mPanelCell->SetVisible(val); }
void PanelPlayer::SetPanelUnitsVisible(bool val) { mPanelUnits->SetVisible(val); }

void PanelPlayer::ClearSelectedCell()
{
    mPanelCell->SetVisible(false);
    mPanelUnits->SetVisible(false);
}

void PanelPlayer::SetSelectedObject(GameObject * obj)
{
    const GameObjectType objType = obj->GetObjectType();

    if(objType == OBJ_BASE)
    {
        mPanelCell->SetVisible(true);
        mPanelUnits->SetVisible(false);

        UpdateButtonNewUnit(obj);
    }
    else if(objType == OBJ_UNIT)
    {
        mPanelCell->SetVisible(false);
        mPanelUnits->SetVisible(true);

        UpdateButtonUnitUpgrade(obj);
        UpdateButtonConquer(obj);
    }
    else
    {
        mPanelCell->SetVisible(false);
        mPanelUnits->SetVisible(false);
    }
}

void PanelPlayer::UpdateButtonNewUnit(const GameObject * obj)
{
    mButtonNewUnit->SetLabel("NEW UNIT");
    mButtonNewUnit->SetEnabled(true);
}

void PanelPlayer::UpdateButtonUnitUpgrade(const GameObject * obj)
{
    mButtonUnitsUpgrade->SetLabel("UPGRADE");
    mButtonUnitsUpgrade->SetEnabled(false);
}

void PanelPlayer::UpdateButtonConquer(const GameObject * obj)
{
    mButtonCellConquer->SetEnabled(true);
}

void PanelPlayer::SetFunctionCellConquest(const std::function<void()> & f)
{
    mButtonCellConquer->SetOnClickFunction(f);
}

void PanelPlayer::SetFunctionUnitMove(const std::function<void()> & f)
{
    mButtonUnitMove->SetOnClickFunction(f);
}

void PanelPlayer::SetFunctionNewUnit(const std::function<void()> & f)
{
    mButtonNewUnit->SetOnClickFunction(f);
}

void PanelPlayer::SetFunctionBuildWall(const std::function<void()> & f)
{
    mButtonBuildWall->SetOnClickFunction(f);
}

void PanelPlayer::SetFunctionUnitsUpgrade(const std::function<void()> & f)
{
    mButtonUnitsUpgrade->SetOnClickFunction(f);
}

std::string PanelPlayer::MakeStrValue(int value)
{
    // convert 1000 and bigger to 1k
    if(value > 1000)
    {
        float val = value / 1000.f;

        std::ostringstream s;
        s << std::setprecision(2) << val << "k";

        return s.str();
    }
    else
        return std::to_string(value);
}

void PanelPlayer::CreatePanelBase()
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    mPanelCell = new ButtonsPanel(this);

    FontManager * fm = FontManager::Instance();

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 28, Font::BOLD);

    Label * labelHeader = new Label("BASE", fontHeader, mPanelCell);
    labelHeader->SetColor(colorHeader);

    mButtonNewUnit = new ButtonPanelPlayer(mPanelCell);

    const int marginY0 = 10;
    const int marginY = 30;

    int buttonY = labelHeader->GetHeight() + marginY0;

    mButtonNewUnit->SetY(buttonY);
    buttonY += mButtonNewUnit->GetHeight() + marginY;
}

void PanelPlayer::CreatePanelUnit()
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    mPanelUnits = new ButtonsPanel(this);

    FontManager * fm = FontManager::Instance();

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 28, Font::BOLD);

    Label * labelHeader = new Label("UNIT", fontHeader, mPanelUnits);
    labelHeader->SetColor(colorHeader);

    const int marginY0 = 10;
    const int marginY = 20;

    // button UPGRADE
    mButtonUnitsUpgrade = new ButtonPanelPlayer(mPanelUnits);

    // button CONQUER
    mButtonCellConquer = new ButtonPanelPlayer("CONQUER", mPanelUnits);

    // button MOVE
    mButtonUnitMove = new ButtonPanelPlayer("MOVE", mPanelUnits);

    // button BUILD WALL
    mButtonBuildWall = new ButtonPanelPlayer("BUILD WALL", mPanelUnits);

    // -- position elements --
    int buttonY = labelHeader->GetY() + labelHeader->GetHeight() + marginY0;
    mButtonUnitsUpgrade->SetY(buttonY);
    buttonY += mButtonUnitsUpgrade->GetHeight() + marginY;

    mButtonCellConquer->SetY(buttonY);
    buttonY += mButtonCellConquer->GetHeight() + marginY;

    mButtonUnitMove->SetY(buttonY);
    buttonY += mButtonUnitMove->GetHeight() + marginY;

    mButtonBuildWall->SetY(buttonY);

}

} // namespace game
