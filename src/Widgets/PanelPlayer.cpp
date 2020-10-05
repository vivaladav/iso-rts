#include "Widgets/PanelPlayer.h"

#include "GameConstants.h"
#include "GameMapCell.h"
#include "Player.h"
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

PanelPlayer::PanelPlayer(Player * player, lib::sgui::Widget * parent)
    : lib::sgui::Widget(parent)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    FontManager * fm = FontManager::Instance();

    Font * fontName = fm->GetFont("data/fonts/OpenSans.ttf", 48);
    fontName->SetStyle(Font::BOLD);

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 28);

    Font * fontData = fm->GetFont("data/fonts/OpenSans.ttf", 40);

    Label * labelName = new Label(player->GetName().c_str(), fontName, this);
    labelName->SetColor(PLAYER_COLOR[player->GetPlayerId()]);

    const int marginX0 = 280;
    const int marginX1 = 50;

    const int Y0 = 10;

    Label * labelHeaderCoins = new Label("COINS", fontHeader, this);
    labelHeaderCoins->SetPosition(labelName->GetX() + labelName->GetWidth() + marginX0, Y0);
    labelHeaderCoins->SetColor(0x212121FF);

    const int coins = player->GetMoney();
    mLabelCoins = new Label(MakeStrCoins(coins).c_str(), fontData, this);
    mLabelCoins->SetPosition(labelHeaderCoins->GetX() + (labelHeaderCoins->GetWidth() - mLabelCoins->GetWidth()) * 0.5f,
                             labelHeaderCoins->GetY() + labelHeaderCoins->GetHeight());
    mLabelCoins->SetColor(0x212121FF);

    Label * labelHeaderCells = new Label("CELLS", fontHeader, this);
    labelHeaderCells->SetPosition(labelHeaderCoins->GetX() + labelHeaderCoins->GetWidth() + marginX1, Y0);
    labelHeaderCells->SetColor(0x212121FF);

    const int cells = player->GetNumCells();
    mLabelCells = new Label(MakeStrCells(cells).c_str(), fontData, this);
    mLabelCells->SetPosition(labelHeaderCells->GetX() + (labelHeaderCells->GetWidth() - mLabelCells->GetWidth()) * 0.5f,
                             labelHeaderCells->GetY() + labelHeaderCells->GetHeight());
    mLabelCells->SetColor(0x212121FF);

    Label * labelHeaderUnits = new Label("UNITS", fontHeader, this);
    labelHeaderUnits->SetPosition(labelHeaderCells->GetX() + labelHeaderCells->GetWidth() + marginX1, Y0);
    labelHeaderUnits->SetColor(0x212121FF);

    const int units = player->GetNumUnits();
    mLabelUnits = new Label(MakeStrUnits(units).c_str(), fontData, this);
    mLabelUnits->SetPosition(labelHeaderUnits->GetX() + (labelHeaderUnits->GetWidth() - mLabelUnits->GetWidth()) * 0.5f,
                             labelHeaderUnits->GetY() + labelHeaderUnits->GetHeight());
    mLabelUnits->SetColor(0x212121FF);

    // -- subpanels --
    const int marginTopRow = 10;
    const int marginRightPanels = 50;

    CreatePanelCell();

    const int panelY = labelName->GetY() + labelName->GetHeight() + marginTopRow;
    mPanelCell->SetY(panelY);
    mPanelCell->SetVisible(false);

    CreatePanelUnits();

    const int panelX = mPanelCell->GetX() + mPanelCell->GetWidth() + marginRightPanels;
    mPanelUnits->SetPosition(panelX, panelY);
    mPanelUnits->SetVisible(false);
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

    mButtonUnitsMove->SetChecked(false);
}

void PanelPlayer::SetSelectedCell(const GameMapCell & cell)
{
    UpdateButtonCellFortify(cell.fortLevel);
    UpdateButtonCellUpgrade(cell.level);
    UpdateButtonNewUnit(cell.units, cell.unitsLevel);
    UpdateButtonUnitUpgrade(cell.units, cell.unitsLevel);
    UpdateButtonUnitsMove(cell.units);

    mPanelCell->SetVisible(true);
}

void PanelPlayer::UpdateButtonCellFortify(int fortLevel)
{
    if(fortLevel >= MAX_CELL_FORT_LEVEL)
    {
        mButtonCellFortify->SetLabel("FORTIFY");
        mButtonCellFortify->SetEnabled(false);
    }
    else
    {
        std::ostringstream s;
        s << "FORTIFY (" << COST_CELL_FORT[fortLevel] << ")";

        mButtonCellFortify->SetLabel(s.str().c_str());
        mButtonCellFortify->SetEnabled(true);
    }
}

void PanelPlayer::UpdateButtonCellUpgrade(int cellLevel)
{
    if(cellLevel >= MAX_CELL_LEVEL)
    {
        mButtonCellUpgrade->SetLabel("UPGRADE");
        mButtonCellUpgrade->SetEnabled(false);
    }
    else
    {
        std::ostringstream s;
        s << "UPGRADE (" << COST_CELL_UPGRADE[cellLevel] << ")";

        mButtonCellUpgrade->SetLabel(s.str().c_str());
        mButtonCellUpgrade->SetEnabled(true);
    }
}

void PanelPlayer::UpdateButtonNewUnit(int num, int level)
{

    if(num >= MAX_CELL_UNITS)
    {
        mButtonNewUnit->SetLabel("NEW UNIT");
        mButtonNewUnit->SetEnabled(false);
    }
    else
    {
        std::ostringstream s;
        s << "NEW UNIT (" << COST_NEW_UNIT[level] << ")";

        mButtonNewUnit->SetLabel(s.str().c_str());
        mButtonNewUnit->SetEnabled(true);
    }

    const bool hasUnits = num > 0;
    mPanelUnits->SetVisible(hasUnits);
}

void PanelPlayer::UpdateButtonUnitUpgrade(int num, int level)
{
    if(level >= MAX_UNITS_LEVEL)
    {
        mButtonUnitsUpgrade->SetLabel("UPGRADE");
        mButtonUnitsUpgrade->SetEnabled(false);
    }
    else
    {
        std::ostringstream s;
        s << "UPGRADE (" << COST_UNIT_UPGRADE[level] * num << ")";

        mButtonUnitsUpgrade->SetLabel(s.str().c_str());
        mButtonUnitsUpgrade->SetEnabled(true);
    }
}

void PanelPlayer::UpdateButtonUnitsMove(int num)
{
    mButtonUnitsMove->SetChecked(false);

    if(num < 1)
        return ;

    for(int i = 0; i < num; ++i)
        mUnitsSelector->SetButtonEnabled(i, true);

    for(int i = num; i < MAX_CELL_UNITS; ++i)
        mUnitsSelector->SetButtonEnabled(i, false);

    mUnitsSelector->SetButtonChecked(num - 1, true);
}

void PanelPlayer::SetFunctionCellFortify(const std::function<void()> & f)
{
    mButtonCellFortify->SetOnClickFunction(f);
}

void PanelPlayer::SetFunctionCellUpgrade(const std::function<void()> & f)
{
    mButtonCellUpgrade->SetOnClickFunction(f);
}

void PanelPlayer::SetFunctionNewUnit(const std::function<void()> & f)
{
    mButtonNewUnit->SetOnClickFunction(f);
}

void PanelPlayer::SetFunctionUnitsMove(const std::function<void()> & f)
{
}

void PanelPlayer::SetFunctionUnitsUpgrade(const std::function<void()> & f)
{
    mButtonUnitsUpgrade->SetOnClickFunction(f);
}

int PanelPlayer::GetNumUnitsToMove() const
{
    if(mButtonUnitsMove->IsChecked())
        return mUnitsSelector->GetIndexChecked() + 1;
    else
        return 0;
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

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 32);
    fontHeader->SetStyle(Font::BOLD);

    Label * labelHeader = new Label("CELL", fontHeader, mPanelCell);
    labelHeader->SetColor(0x212121FF);

    int buttonY = labelHeader->GetY() + labelHeader->GetHeight() + 10;

    const int marginY = 30;

    mButtonNewUnit = new ButtonPanelPlayer(mPanelCell);
    mButtonNewUnit->SetY(buttonY);

    buttonY += mButtonNewUnit->GetHeight() + marginY;

    mButtonCellFortify = new ButtonPanelPlayer(mPanelCell);
    mButtonCellFortify->SetY(buttonY);

    buttonY += mButtonCellFortify->GetHeight() + marginY;

    mButtonCellUpgrade = new ButtonPanelPlayer(mPanelCell);
    mButtonCellUpgrade->SetY(buttonY);
}

void PanelPlayer::CreatePanelUnits()
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    mPanelUnits = new ButtonsPanel(this);

    FontManager * fm = FontManager::Instance();

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 32);
    fontHeader->SetStyle(Font::BOLD);

    Label * labelHeader = new Label("UNITS", fontHeader, mPanelUnits);
    labelHeader->SetColor(0x212121FF);

    const int marginY = 30;

    int buttonY = labelHeader->GetY() + labelHeader->GetHeight() + 10;

    // button UPGRADE
    mButtonUnitsUpgrade = new ButtonPanelPlayer(mPanelUnits);
    mButtonUnitsUpgrade->SetY(buttonY);

    buttonY += mButtonUnitsUpgrade->GetHeight() + marginY;

    // button MOVE
    mButtonUnitsMove = new ButtonPanelPlayer("MOVE", mPanelUnits);
    mButtonUnitsMove->SetCheckable(true);
    mButtonUnitsMove->SetY(buttonY);

    mButtonUnitsMove->SetOnToggleFunction([this](bool checked)
    {
        mUnitsSelector->SetVisible(checked);
    });

    // units selector
    mUnitsSelector = new UnitsSelector(mPanelUnits);
    mUnitsSelector->SetPosition((mButtonUnitsMove->GetWidth() - mUnitsSelector->GetWidth()) * 0.5f,
                                mButtonUnitsMove->GetY() + mButtonUnitsMove->GetHeight() + 10);

    mUnitsSelector->SetVisible(false);
}

} // namespace game
