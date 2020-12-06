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

PanelPlayer::PanelPlayer(Player * player, PanelPosition pos, lib::sgui::Widget * parent)
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

    Label * labelHeaderCoins = new Label("COINS", fontHeader, this);
    labelHeaderCoins->SetColor(0x212121FF);

    const int coins = player->GetMoney();
    mLabelCoins = new Label(MakeStrCoins(coins).c_str(), fontData, this);
    mLabelCoins->SetColor(0x212121FF);

    Label * labelHeaderCells = new Label("CELLS", fontHeader, this);
    labelHeaderCells->SetColor(0x212121FF);

    const int cells = player->GetNumCells();
    mLabelCells = new Label(MakeStrCells(cells).c_str(), fontData, this);
    mLabelCells->SetColor(0x212121FF);

    Label * labelHeaderUnits = new Label("UNITS", fontHeader, this);
    labelHeaderUnits->SetColor(0x212121FF);

    const int units = player->GetNumUnits();
    mLabelUnits = new Label(MakeStrUnits(units).c_str(), fontData, this);
    mLabelUnits->SetColor(0x212121FF);

    // -- subpanels --
    const int marginTopRow = 10;
    const int marginRightPanels = 50;

    CreatePanelCell(pos);
    mPanelCell->SetVisible(false);

    CreatePanelUnits(pos);
    mPanelUnits->SetVisible(false);

    // -- position elements --
    const int marginH = 10;

    const int marginX0 = 280;
    const int marginX1 = 50;

    const int panelW = labelName->GetWidth() +
                       labelHeaderCoins->GetWidth() +
                       labelHeaderCells->GetWidth() +
                       labelHeaderUnits->GetWidth() +
                       marginH + marginX0 + (marginX1 * 2);

    switch(pos)
    {
        case PPOS_TL:
        {
            // player name
            labelName->SetX(marginH);

            // coins
            labelHeaderCoins->SetX(labelName->GetX() + labelName->GetWidth() + marginX0);

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
            const int panelY = labelName->GetY() + labelName->GetHeight() + marginTopRow;
            mPanelCell->SetPosition(labelName->GetX(), panelY);

            // sub-panel unit buttons
            const int panelX = mPanelCell->GetX() + mPanelCell->GetWidth() + marginRightPanels;
            mPanelUnits->SetPosition(panelX, panelY);
        }
        break;

        case PPOS_TR:
        {
            // player name
            labelName->SetX(panelW - marginH - labelName->GetWidth());

            // coins
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
            const int panelX = panelW - marginH - mPanelCell->GetWidth();
            const int panelY = labelName->GetY() + labelName->GetHeight() + marginTopRow;
            mPanelCell->SetPosition(panelX, panelY);

            // sub-panel unit buttons
            const int panelX2 = panelX - mPanelUnits->GetWidth() - marginRightPanels;
            mPanelUnits->SetPosition(panelX2, panelY);

            // resize panel to include right margin in its size
            SetSize(GetWidth() + marginH, GetHeight());
        }
        break;

        case PPOS_BL:
        {
            // player name
            labelName->SetPosition(marginH, GetHeight() - labelName->GetHeight());

            // coins
            labelHeaderCoins->SetPosition(labelName->GetX() + labelName->GetWidth() + marginX0,
                                          labelName->GetY() + labelName->GetHeight() - labelHeaderCoins->GetHeight());

            mLabelCoins->SetPosition(labelHeaderCoins->GetX() +
                                     (labelHeaderCoins->GetWidth() - mLabelCoins->GetWidth()) * 0.5f,
                                     labelHeaderCoins->GetY() - mLabelCoins->GetHeight());

            // cells
            labelHeaderCells->SetPosition(labelHeaderCoins->GetX() + labelHeaderCoins->GetWidth() + marginX1,
                                          labelName->GetY() + labelName->GetHeight() - labelHeaderCells->GetHeight());

            mLabelCells->SetPosition(labelHeaderCells->GetX() + (labelHeaderCells->GetWidth() - mLabelCells->GetWidth()) * 0.5f,
                                     labelHeaderCells->GetY() - mLabelCells->GetHeight());

            // units
            labelHeaderUnits->SetPosition(labelHeaderCells->GetX() + labelHeaderCells->GetWidth() + marginX1,
                                          labelName->GetY() + labelName->GetHeight() - labelHeaderUnits->GetHeight());

            mLabelUnits->SetPosition(labelHeaderUnits->GetX() +
                                     (labelHeaderUnits->GetWidth() - mLabelUnits->GetWidth()) * 0.5f,
                                     labelHeaderUnits->GetY() - mLabelUnits->GetHeight());

            // sub-panel cell buttons
            const int panelY = labelName->GetY() - marginTopRow - mPanelCell->GetHeight();
            mPanelCell->SetPosition(labelName->GetX(), panelY);

            // sub-panel unit buttons
            const int panelX = mPanelCell->GetX() + mPanelCell->GetWidth() + marginRightPanels;
            const int panelY2 = labelName->GetY() - marginTopRow - mPanelUnits->GetHeight();
            mPanelUnits->SetPosition(panelX, panelY2);
        }
        break;

        case PPOS_BR:
        {
            // player name
            labelName->SetPosition(panelW - marginH - labelName->GetWidth(),
                                   GetHeight() - labelName->GetHeight());

            // coins
            labelHeaderCoins->SetY(labelName->GetY() + labelName->GetHeight() - labelHeaderCoins->GetHeight());

            mLabelCoins->SetPosition(labelHeaderCoins->GetX() +
                                     (labelHeaderCoins->GetWidth() - mLabelCoins->GetWidth()) * 0.5f,
                                     labelHeaderCoins->GetY() - mLabelCoins->GetHeight());

            // cells
            labelHeaderCells->SetPosition(labelHeaderCoins->GetX() + labelHeaderCoins->GetWidth() + marginX1,
                                          labelName->GetY() + labelName->GetHeight() - labelHeaderCells->GetHeight());

            mLabelCells->SetPosition(labelHeaderCells->GetX() + (labelHeaderCells->GetWidth() - mLabelCells->GetWidth()) * 0.5f,
                                     labelHeaderCells->GetY() - mLabelCells->GetHeight());

            // units
            labelHeaderUnits->SetPosition(labelHeaderCells->GetX() + labelHeaderCells->GetWidth() + marginX1,
                                          labelName->GetY() + labelName->GetHeight() - labelHeaderUnits->GetHeight());

            mLabelUnits->SetPosition(labelHeaderUnits->GetX() +
                                     (labelHeaderUnits->GetWidth() - mLabelUnits->GetWidth()) * 0.5f,
                                     labelHeaderUnits->GetY() - mLabelUnits->GetHeight());

            // sub-panel cell buttons
            const int panelX = panelW - marginH - mPanelCell->GetWidth();
            const int panelY = labelName->GetY() - marginTopRow - mPanelCell->GetHeight();
            mPanelCell->SetPosition(panelX, panelY);

            // sub-panel unit buttons
            const int panelX2 = panelX - mPanelUnits->GetWidth() - marginRightPanels;
            const int panelY2 = labelName->GetY() - marginTopRow - mPanelUnits->GetHeight();
            mPanelUnits->SetPosition(panelX2, panelY2);

            // resize panel to include right margin in its size
            SetSize(GetWidth() + marginH, GetHeight());
        }
        break;

        default:
        break;
    }
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
    const Unit * unit = cell.GetUnit();
    int unitElements = 0;
    int unitLevel = 0;

    if(unit != nullptr)
    {
        unitElements = unit->GetNumElements();
        unitLevel = unit->GetUnitLevel();
    }

    UpdateButtonCellFortify(cell.fortLevel);
    UpdateButtonCellUpgrade(cell.level);
    UpdateButtonNewUnit(unitElements, unitLevel);
    UpdateButtonUnitDestroy();
    UpdateButtonUnitUpgrade(unitElements, unitLevel);

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

void PanelPlayer::UpdateButtonUnitDestroy()
{
    mButtonUnitsDestroy->SetVisible(true);
    mButtonUnitsDestroyConf->SetVisible(false);
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

void PanelPlayer::CreatePanelCell(PanelPosition pos)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    mPanelCell = new ButtonsPanel(this);

    FontManager * fm = FontManager::Instance();

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 32);
    fontHeader->SetStyle(Font::BOLD);

    Label * labelHeader = new Label("CELL", fontHeader, mPanelCell);
    labelHeader->SetColor(0x212121FF);

    mButtonNewUnit = new ButtonPanelPlayer(mPanelCell);
    mButtonCellFortify = new ButtonPanelPlayer(mPanelCell);
    mButtonCellUpgrade = new ButtonPanelPlayer(mPanelCell);

    if(PPOS_TR == pos || PPOS_BR == pos)
        labelHeader->SetX(mPanelCell->GetWidth() - labelHeader->GetWidth());

    const int marginY0 = 10;
    const int marginY = 30;

    if(PPOS_BL == pos || PPOS_BR == pos)
    {
        const int panelH = labelHeader->GetHeight() + marginY0 +
                           mButtonNewUnit->GetHeight() +
                           mButtonCellFortify->GetHeight() +
                           mButtonCellUpgrade->GetHeight() +
                           (marginY * 2);

        int buttonY = panelH - labelHeader->GetHeight();

        labelHeader->SetY(buttonY);

        buttonY -= marginY0 + mButtonCellUpgrade->GetHeight();

        mButtonCellUpgrade->SetY(buttonY);
        buttonY -= marginY + mButtonCellFortify->GetHeight();

        mButtonCellFortify->SetY(buttonY);
        buttonY -= marginY + mButtonNewUnit->GetHeight();

        mButtonNewUnit->SetY(buttonY);
    }
    else
    {
        int buttonY = labelHeader->GetHeight() + marginY0;

        mButtonNewUnit->SetY(buttonY);
        buttonY += mButtonNewUnit->GetHeight() + marginY;

        mButtonCellFortify->SetY(buttonY);
        buttonY += mButtonCellFortify->GetHeight() + marginY;

        mButtonCellUpgrade->SetY(buttonY);
    }
}

void PanelPlayer::CreatePanelUnits(PanelPosition pos)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    mPanelUnits = new ButtonsPanel(this);

    FontManager * fm = FontManager::Instance();

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 32);
    fontHeader->SetStyle(Font::BOLD);

    Label * labelHeader = new Label("UNITS", fontHeader, mPanelUnits);
    labelHeader->SetColor(0x212121FF);

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

    // -- position elements --
    if(PPOS_TL == pos || PPOS_BL == pos)
        mButtonUnitsDestroy->SetX(mButtonUnitsUpgrade->GetX() + marginX + mButtonUnitsUpgrade->GetWidth());
    else
        mButtonUnitsUpgrade->SetX(mButtonUnitsDestroy->GetX() + marginX + mButtonUnitsDestroy->GetWidth());

    if(PPOS_BL == pos || PPOS_BR == pos)
    {
        const int panelH = labelHeader->GetHeight() + marginY0 +
                           mButtonUnitsUpgrade->GetHeight() +
                           marginY;

        int buttonY = panelH - labelHeader->GetHeight();

        labelHeader->SetY(buttonY);

        buttonY -= marginY0 + mButtonUnitsUpgrade->GetHeight();

        mButtonUnitsUpgrade->SetY(buttonY);
    }
    else
    {
        int buttonY = labelHeader->GetY() + labelHeader->GetHeight() + marginY0;

        mButtonUnitsUpgrade->SetY(buttonY);

    }

    mButtonUnitsDestroy->SetY(mButtonUnitsUpgrade->GetY());
    mButtonUnitsDestroyConf->SetPosition(mButtonUnitsDestroy->GetX(), mButtonUnitsDestroy->GetY());

    if(PPOS_TR == pos || PPOS_BR == pos)
        labelHeader->SetX(mPanelUnits->GetWidth() - labelHeader->GetWidth());
}

} // namespace game
