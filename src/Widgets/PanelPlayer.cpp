#include "Widgets/PanelPlayer.h"

#include "GameConstants.h"
#include "GameMapCell.h"
#include "Player.h"
#include "GameObjects/Unit.h"
#include "Widgets/ButtonPanelPlayer.h"
#include "Widgets/ButtonsPanel.h"
#include "Widgets/UnitsSelector.h"

#include <core/event/MouseButtonEvent.h>
#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <sgui/Label.h>

#include <iomanip>
#include <iostream>
#include <sstream>

namespace game
{

const unsigned int colorHeader = 0x888888FF;
const unsigned int colorData = 0x777777FF;

PanelPlayer::PanelPlayer(Player * player, lib::sgui::Widget * parent)
    : lib::sgui::Widget(parent)
    , mPlayer(player)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    FontManager * fm = FontManager::Instance();
    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 28, Font::BOLD);
    Font * fontData = fm->GetFont("data/fonts/OpenSans.ttf", 28, Font::NORMAL);

    // ENERGY
    Label * labelHeaderEnergy = new Label("ENERGY", fontHeader, this);
    labelHeaderEnergy->SetColor(colorHeader);

    const int energy = player->GetEnergy();
    mLabelEnergy = new Label(MakeStrValue(energy).c_str(), fontData, this);
    mLabelEnergy->SetColor(colorData);

    // MATERIAL
    Label * labelHeaderMaterial = new Label("MATERIAL", fontHeader, this);
    labelHeaderMaterial->SetColor(colorHeader);

    const int material = player->GetMaterial();
    mLabelMaterial = new Label(MakeStrValue(material).c_str(), fontData, this);
    mLabelMaterial->SetColor(colorData);

    // DIAMONDS
    Label * labelHeaderDiamonds = new Label("DIAMONDS", fontHeader, this);
    labelHeaderDiamonds->SetColor(colorHeader);

    const int diamonds = player->GetDiamonds();
    mLabelDiamonds = new Label(MakeStrValue(diamonds).c_str(), fontData, this);
    mLabelDiamonds->SetColor(colorData);

    // CELLS
    Label * labelHeaderCells = new Label("CELLS", fontHeader, this);
    labelHeaderCells->SetColor(colorHeader);

    const int cells = player->GetNumCells();
    mLabelCells = new Label(MakeStrValue(cells).c_str(), fontData, this);
    mLabelCells->SetColor(colorData);

    // UNITS
    Label * labelHeaderUnits = new Label("UNITS", fontHeader, this);
    labelHeaderUnits->SetColor(colorHeader);

    const int units = player->GetNumUnits();
    mLabelUnits = new Label(MakeStrValue(units).c_str(), fontData, this);
    mLabelUnits->SetColor(colorData);

    // -- subpanels --
    const int marginTopRow = 10;

    CreatePanelBase();
    mPanelCell->SetVisible(false);

    CreatePanelUnit();
    mPanelUnits->SetVisible(false);

    // -- position elements --
    const int marginX0 = 10;
    const int marginX1 = 10;
    const int marginX2 = 100;

    int widgetX = marginX0;

    // energy
    labelHeaderEnergy->SetX(widgetX);
    widgetX += labelHeaderEnergy->GetWidth() + marginX1;

    mLabelEnergy->SetPosition(widgetX, labelHeaderEnergy->GetY());
    widgetX += mLabelEnergy->GetWidth() + marginX2;

    // material
    labelHeaderMaterial->SetX(widgetX);
    widgetX += labelHeaderMaterial->GetWidth() + marginX1;

    mLabelMaterial->SetPosition(widgetX, labelHeaderMaterial->GetY());
    widgetX += mLabelMaterial->GetWidth() + marginX2;

    // diamonds
    labelHeaderDiamonds->SetX(widgetX);
    widgetX += labelHeaderDiamonds->GetWidth() + marginX1;

    mLabelDiamonds->SetPosition(widgetX, labelHeaderDiamonds->GetY());
    widgetX += mLabelDiamonds->GetWidth() + marginX2;

    // cells
    labelHeaderCells->SetX(widgetX);
    widgetX += labelHeaderCells->GetWidth() + marginX1;

    mLabelCells->SetPosition(widgetX, labelHeaderCells->GetY());
    widgetX += mLabelCells->GetWidth() + marginX2;

    // units
    labelHeaderUnits->SetX(widgetX);
    widgetX += labelHeaderUnits->GetWidth() + marginX1;

    mLabelUnits->SetPosition(widgetX, labelHeaderUnits->GetY());

    const int panelY = mLabelEnergy->GetY() + mLabelEnergy->GetHeight() + marginTopRow;

    // sub-panel cell buttons
    mPanelCell->SetPosition(marginX0, panelY);

    // sub-panel unit buttons
    mPanelUnits->SetPosition(marginX0, panelY);
}

void PanelPlayer::SetPanelCellVisible(bool val) { mPanelCell->SetVisible(val); }
void PanelPlayer::SetPanelUnitsVisible(bool val) { mPanelUnits->SetVisible(val); }

void PanelPlayer::UpdateCells(int cells)
{
    mLabelCells->SetText(MakeStrValue(cells).c_str());
}

void PanelPlayer::UpdateEnergy(int energy)
{
    mLabelEnergy->SetText(MakeStrValue(energy).c_str());
}

void PanelPlayer::UpdateMaterial(int material)
{
    mLabelMaterial->SetText(MakeStrValue(material).c_str());
}

void PanelPlayer::UpdateDiamonds(int diamonds)
{
    mLabelDiamonds->SetText(MakeStrValue(diamonds).c_str());
}

void PanelPlayer::UpdateUnits(int units)
{
    mLabelUnits->SetText(MakeStrValue(units).c_str());
}

void PanelPlayer::ClearSelectedCell()
{
    mPanelCell->SetVisible(false);
    mPanelUnits->SetVisible(false);
}

//void PanelPlayer::SetSelectedCell(const GameMapCell & cell)
//{
//    UpdateButtonNewUnit(cell);
//    UpdateButtonUnitUpgrade();
//    UpdateButtonConquer(cell);

//    mPanelCell->SetVisible(true);
//}

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
