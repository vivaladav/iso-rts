#include "Widgets/PanelPlayer.h"

#include "GameConstants.h"
#include "GameMapCell.h"
#include "Player.h"
#include "Unit.h"
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
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    FontManager * fm = FontManager::Instance();
    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 28, Font::BOLD);
    Font * fontData = fm->GetFont("data/fonts/OpenSans.ttf", 32, Font::NORMAL);

    Label * labelHeaderCoins = new Label("ENERGY", fontHeader, this);
    labelHeaderCoins->SetColor(colorHeader);

    const int coins = player->GetMoney();
    mLabelCoins = new Label(MakeStrCoins(coins).c_str(), fontData, this);
    mLabelCoins->SetColor(colorData);

    Label * labelHeaderCells = new Label("CELLS", fontHeader, this);
    labelHeaderCells->SetColor(colorHeader);

    const int cells = player->GetNumCells();
    mLabelCells = new Label(MakeStrCells(cells).c_str(), fontData, this);
    mLabelCells->SetColor(colorData);

    Label * labelHeaderUnits = new Label("UNITS", fontHeader, this);
    labelHeaderUnits->SetColor(colorHeader);

    const int units = player->GetNumUnits();
    mLabelUnits = new Label(MakeStrUnits(units).c_str(), fontData, this);
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
    const int marginX1 = 50;

    // coins
    labelHeaderCoins->SetX(marginX0);

    mLabelCoins->SetPosition(labelHeaderCoins->GetX() +
                             (labelHeaderCoins->GetWidth() - mLabelCoins->GetWidth()) * 0.5f,
                             labelHeaderCoins->GetY() + labelHeaderCoins->GetHeight());

    // cells
    labelHeaderCells->SetX(labelHeaderCoins->GetX() + labelHeaderCoins->GetWidth() + marginX1);

    mLabelCells->SetPosition(labelHeaderCells->GetX() + (labelHeaderCells->GetWidth() - mLabelCells->GetWidth()) * 0.5f,
                             labelHeaderCells->GetY() + labelHeaderCells->GetHeight());

    // units
    labelHeaderUnits->SetX(labelHeaderCells->GetX() + labelHeaderCells->GetWidth() + marginX1);

    mLabelUnits->SetPosition(labelHeaderUnits->GetX() +
                             (labelHeaderUnits->GetWidth() - mLabelUnits->GetWidth()) * 0.5f,
                             labelHeaderUnits->GetY() + labelHeaderUnits->GetHeight());

    // sub-panel cell buttons
    const int panelY = mLabelCoins->GetY() + mLabelCoins->GetHeight() + marginTopRow;
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

    mLabelCells->SetText(MakeStrCells(cells).c_str());

    mLabelCells->SetPosition(cX - mLabelCells->GetWidth() * 0.5f, cY - mLabelCells->GetHeight() * 0.5f);
}

void PanelPlayer::UpdateCoins(int coins)
{
    const float cX = mLabelCoins->GetX() + mLabelCoins->GetWidth() * 0.5f;
    const float cY = mLabelCoins->GetY() + mLabelCoins->GetHeight() * 0.5f;

    mLabelCoins->SetText(MakeStrCoins(coins).c_str());

    mLabelCoins->SetPosition(cX - mLabelCoins->GetWidth() * 0.5f, cY - mLabelCoins->GetHeight() * 0.5f);
}

void PanelPlayer::UpdateUnits(int units)
{
    const float cX = mLabelUnits->GetX() + mLabelUnits->GetWidth() * 0.5f;
    const float cY = mLabelUnits->GetY() + mLabelUnits->GetHeight() * 0.5f;

    mLabelUnits->SetText(MakeStrUnits(units).c_str());

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

    mPanelCell->SetVisible(true);
}

void PanelPlayer::UpdateButtonNewUnit(const GameMapCell & cell)
{
    const bool noUnit = cell.GetUnit() == nullptr;

    std::ostringstream s;

    if(noUnit)
        s << "NEW UNIT (" << COST_NEW_UNIT << ")";
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

void PanelPlayer::SetFunctionCellConquest(const std::function<void()> & f)
{
    mButtonCellConquest->SetOnClickFunction(f);
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

std::string PanelPlayer::MakeStrCells(int cells)
{
    return std::to_string(cells);
}

std::string PanelPlayer::MakeStrCoins(int coins)
{
    // convert 1000 and bigger to 1k
    if(coins > 1000)
    {
        float val = coins / 1000.f;

        std::ostringstream s;
        s << std::setprecision(2) << val << "k";

        return s.str();
    }
    else
        return std::to_string(coins);
}

std::string PanelPlayer::MakeStrUnits(int units)
{
    return std::to_string(units);
}

void PanelPlayer::CreatePanelCell()
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    mPanelCell = new ButtonsPanel(this);

    FontManager * fm = FontManager::Instance();

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 32, Font::BOLD);

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

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 32, Font::BOLD);

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
    mButtonCellConquest = new ButtonPanelPlayer("CONQUEST", mPanelUnits);

    // -- position elements --
    mButtonUnitsDestroy->SetX(mButtonUnitsUpgrade->GetX() + marginX + mButtonUnitsUpgrade->GetWidth());

    int buttonY = labelHeader->GetY() + labelHeader->GetHeight() + marginY0;
    mButtonUnitsUpgrade->SetY(buttonY);

    buttonY += mButtonUnitsUpgrade->GetHeight() + marginY;
    mButtonCellConquest->SetY(buttonY);

    mButtonUnitsDestroy->SetY(mButtonUnitsUpgrade->GetY());
    mButtonUnitsDestroyConf->SetPosition(mButtonUnitsDestroy->GetX(), mButtonUnitsDestroy->GetY());

    mButtonCellConquest->SetX(mButtonUnitsUpgrade->GetX());
}

} // namespace game
