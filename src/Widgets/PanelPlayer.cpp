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

    Label * labelHeaderEnergy = new Label("ENERGY", fontHeader, this);
    labelHeaderEnergy->SetColor(colorHeader);

    const int energy = player->GetEnergy();
    mLabelEnergy = new Label(MakeStrValue(energy).c_str(), fontData, this);
    mLabelEnergy->SetColor(colorData);

    Label * labelHeaderMaterial = new Label("MATERIAL", fontHeader, this);
    labelHeaderMaterial->SetColor(colorHeader);

    const int material = player->GetMaterial();
    mLabelMaterial = new Label(MakeStrValue(material).c_str(), fontData, this);
    mLabelMaterial->SetColor(colorData);

    Label * labelHeaderCells = new Label("CELLS", fontHeader, this);
    labelHeaderCells->SetColor(colorHeader);

    const int cells = player->GetNumCells();
    mLabelCells = new Label(MakeStrValue(cells).c_str(), fontData, this);
    mLabelCells->SetColor(colorData);

    Label * labelHeaderUnits = new Label("UNITS", fontHeader, this);
    labelHeaderUnits->SetColor(colorHeader);

    const int units = player->GetNumUnits();
    mLabelUnits = new Label(MakeStrValue(units).c_str(), fontData, this);
    mLabelUnits->SetColor(colorData);

    // -- subpanels --
    const int marginTopRow = 10;
    const int marginRightPanels = 50;

    CreatePanelCell();
    mPanelCell->SetVisible(false);

    CreatePanelUnits();
    mPanelUnits->SetVisible(false);

    // -- position elements --
    const int marginX0 = 10;
    const int marginX1 = 10;
    const int marginX2 = 50;

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

    // cells
    labelHeaderCells->SetX(widgetX);
    widgetX += labelHeaderCells->GetWidth() + marginX1;

    mLabelCells->SetPosition(widgetX, labelHeaderCells->GetY());
    widgetX += mLabelCells->GetWidth() + marginX2;

    // units
    labelHeaderUnits->SetX(widgetX);
    widgetX += labelHeaderUnits->GetWidth() + marginX1;

    mLabelUnits->SetPosition(widgetX, labelHeaderUnits->GetY());

    // sub-panel cell buttons
    const int panelY = mLabelEnergy->GetY() + mLabelEnergy->GetHeight() + marginTopRow;
    mPanelCell->SetPosition(marginX0, panelY);

    // sub-panel unit buttons
    const int panelX = mPanelCell->GetX() + mPanelCell->GetWidth() + marginRightPanels;
    mPanelUnits->SetPosition(panelX, panelY);
}

void PanelPlayer::SetPanelCellVisible(bool val) { mPanelCell->SetVisible(val); }
void PanelPlayer::SetPanelUnitsVisible(bool val) { mPanelUnits->SetVisible(val); }

void PanelPlayer::UpdateCells(int cells)
{
    const float cX = mLabelCells->GetX() + mLabelCells->GetWidth() * 0.5f;
    const float cY = mLabelCells->GetY() + mLabelCells->GetHeight() * 0.5f;

    mLabelCells->SetText(MakeStrValue(cells).c_str());

    mLabelCells->SetPosition(cX - mLabelCells->GetWidth() * 0.5f,
                             cY - mLabelCells->GetHeight() * 0.5f);
}

void PanelPlayer::UpdateEnergy(int energy)
{
    const float cX = mLabelEnergy->GetX() + mLabelEnergy->GetWidth() * 0.5f;
    const float cY = mLabelEnergy->GetY() + mLabelEnergy->GetHeight() * 0.5f;

    mLabelEnergy->SetText(MakeStrValue(energy).c_str());

    mLabelEnergy->SetPosition(cX - mLabelEnergy->GetWidth() * 0.5f,
                              cY - mLabelEnergy->GetHeight() * 0.5f);
}

void PanelPlayer::UpdateMaterial(int material)
{
    const float cX = mLabelMaterial->GetX() + mLabelMaterial->GetWidth() * 0.5f;
    const float cY = mLabelMaterial->GetY() + mLabelMaterial->GetHeight() * 0.5f;

    mLabelMaterial->SetText(MakeStrValue(material).c_str());

    mLabelMaterial->SetPosition(cX - mLabelMaterial->GetWidth() * 0.5f,
                                cY - mLabelMaterial->GetHeight() * 0.5f);
}

void PanelPlayer::UpdateUnits(int units)
{
    const float cX = mLabelUnits->GetX() + mLabelUnits->GetWidth() * 0.5f;
    const float cY = mLabelUnits->GetY() + mLabelUnits->GetHeight() * 0.5f;

    mLabelUnits->SetText(MakeStrValue(units).c_str());

    mLabelUnits->SetPosition(cX - mLabelUnits->GetWidth() * 0.5f, cY - mLabelUnits->GetHeight() * 0.5f);
}

void PanelPlayer::ClearSelectedCell()
{
    mPanelCell->SetVisible(false);
    mPanelUnits->SetVisible(false);
}

void PanelPlayer::SetSelectedCell(const GameMapCell & cell)
{
    UpdateButtonNewUnit(cell);
    UpdateButtonUnitDestroy();
    UpdateButtonUnitUpgrade();
    UpdateButtonConquer(cell);

    mPanelCell->SetVisible(true);
}

void PanelPlayer::UpdateButtonNewUnit(const GameMapCell & cell)
{
    const bool noUnit = cell.GetUnit() == nullptr;

    std::ostringstream s;

    if(noUnit)
        s << "NEW UNIT (" << ENERGY_NEW_UNIT << ")";
    else
        s << "NEW UNIT";

    mButtonNewUnit->SetLabel(s.str().c_str());
    mButtonNewUnit->SetEnabled(noUnit);

    mPanelUnits->SetVisible(!noUnit);
}

void PanelPlayer::UpdateButtonUnitDestroy()
{
    mButtonUnitsDestroy->SetVisible(true);
    mButtonUnitsDestroyConf->SetVisible(false);
}

void PanelPlayer::UpdateButtonUnitUpgrade()
{
    mButtonUnitsUpgrade->SetLabel("UPGRADE");
    mButtonUnitsUpgrade->SetEnabled(false);
}

void PanelPlayer::UpdateButtonConquer(const GameMapCell & cell)
{
    mButtonCellConquer->SetEnabled(cell.owner != mPlayer);
}

void PanelPlayer::SetFunctionCellConquest(const std::function<void()> & f)
{
    mButtonCellConquer->SetOnClickFunction(f);
}

void PanelPlayer::SetFunctionNewUnit(const std::function<void()> & f)
{
    mButtonNewUnit->SetOnClickFunction(f);
}

void PanelPlayer::SetFunctionUnitsDestroy(const std::function<void()> & f)
{
    mButtonUnitsDestroyConf->SetOnClickFunction(f);
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

void PanelPlayer::CreatePanelCell()
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    mPanelCell = new ButtonsPanel(this);

    FontManager * fm = FontManager::Instance();

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 28, Font::BOLD);

    Label * labelHeader = new Label("CELL", fontHeader, mPanelCell);
    labelHeader->SetColor(colorHeader);

    mButtonNewUnit = new ButtonPanelPlayer(mPanelCell);

    const int marginY0 = 10;
    const int marginY = 30;

    int buttonY = labelHeader->GetHeight() + marginY0;

    mButtonNewUnit->SetY(buttonY);
    buttonY += mButtonNewUnit->GetHeight() + marginY;
}

void PanelPlayer::CreatePanelUnits()
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    mPanelUnits = new ButtonsPanel(this);

    FontManager * fm = FontManager::Instance();

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 28, Font::BOLD);

    Label * labelHeader = new Label("UNITS", fontHeader, mPanelUnits);
    labelHeader->SetColor(colorHeader);

    const int marginX = 30;
    const int marginY0 = 10;
    const int marginY = 30;

    // button UPGRADE
    mButtonUnitsUpgrade = new ButtonPanelPlayer(mPanelUnits);

    // button DESTROY
    mButtonUnitsDestroy = new ButtonPanelPlayer("DESTROY", mPanelUnits);

    mButtonUnitsDestroy->SetOnClickFunction([this]
    {
        mButtonUnitsDestroy->SetVisible(false);
        mButtonUnitsDestroyConf->SetVisible(true);
    });

    // button DESTROY CONFIRM
    mButtonUnitsDestroyConf = new ButtonPanelPlayer("CONFIRM", mPanelUnits);
    mButtonUnitsDestroyConf->SetVisible(false);

    // button CONQUEST
    mButtonCellConquer = new ButtonPanelPlayer("CONQUER", mPanelUnits);

    // -- position elements --
    mButtonUnitsDestroy->SetX(mButtonUnitsUpgrade->GetX() + marginX + mButtonUnitsUpgrade->GetWidth());

    int buttonY = labelHeader->GetY() + labelHeader->GetHeight() + marginY0;
    mButtonUnitsUpgrade->SetY(buttonY);

    buttonY += mButtonUnitsUpgrade->GetHeight() + marginY;
    mButtonCellConquer->SetY(buttonY);

    mButtonUnitsDestroy->SetY(mButtonUnitsUpgrade->GetY());
    mButtonUnitsDestroyConf->SetPosition(mButtonUnitsDestroy->GetX(), mButtonUnitsDestroy->GetY());

    mButtonCellConquer->SetX(mButtonUnitsUpgrade->GetX());
}

} // namespace game
