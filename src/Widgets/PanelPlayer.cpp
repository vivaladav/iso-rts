#include "PanelPlayer.h"

#include "Player.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <sgui/Label.h>
#include <sgui/PushButton.h>

#include <iomanip>
#include <iostream>
#include <sstream>

namespace game
{

PanelPlayer::PanelPlayer(Player * p, lib::sgui::Widget * parent)
    : lib::sgui::Widget(parent)
    , mPlayer(p)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    FontManager * fm = FontManager::Instance();

    Font * fontName = fm->GetFont("data/fonts/OpenSans.ttf", 48);
    fontName->SetStyle(Font::BOLD);

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 28);

    Font * fontData = fm->GetFont("data/fonts/OpenSans.ttf", 40);

    Label * labelName = new Label(p->GetName().c_str(), fontName, this);
    labelName->SetColor(0x212121FF);

    const int marginX0 = 280;
    const int marginX1 = 50;

    const int Y0 = 10;

    Label * labelHeaderCoins = new Label("COINS", fontHeader, this);
    labelHeaderCoins->SetPosition(labelName->GetX() + labelName->GetWidth() + marginX0, Y0);
    labelHeaderCoins->SetColor(0x212121FF);

    mLabelCoins = new Label(MakeStrCoins().c_str(), fontData, this);
    mLabelCoins->SetPosition(labelHeaderCoins->GetX() + (labelHeaderCoins->GetWidth() - mLabelCoins->GetWidth()) * 0.5f,
                             labelHeaderCoins->GetY() + labelHeaderCoins->GetHeight());
    mLabelCoins->SetColor(0x212121FF);

    Label * labelHeaderCells = new Label("CELLS", fontHeader, this);
    labelHeaderCells->SetPosition(labelHeaderCoins->GetX() + labelHeaderCoins->GetWidth() + marginX1, Y0);
    labelHeaderCells->SetColor(0x212121FF);

    mLabelCells = new Label(MakeStrCells().c_str(), fontData, this);
    mLabelCells->SetPosition(labelHeaderCells->GetX() + (labelHeaderCells->GetWidth() - mLabelCells->GetWidth()) * 0.5f,
                             labelHeaderCells->GetY() + labelHeaderCells->GetHeight());
    mLabelCells->SetColor(0x212121FF);

    Label * labelHeaderUnits = new Label("UNITS", fontHeader, this);
    labelHeaderUnits->SetPosition(labelHeaderCells->GetX() + labelHeaderCells->GetWidth() + marginX1, Y0);
    labelHeaderUnits->SetColor(0x212121FF);

    mLabelUnits = new Label(MakeStrUnits().c_str(), fontData, this);
    mLabelUnits->SetPosition(labelHeaderUnits->GetX() + (labelHeaderUnits->GetWidth() - mLabelUnits->GetWidth()) * 0.5f,
                             labelHeaderUnits->GetY() + labelHeaderUnits->GetHeight());
    mLabelUnits->SetColor(0x212121FF);

    // -- subpanels --
    const int marginTopRow = 10;
    const int marginRightPanels = 50;

    CreatePanelCell();

    const int panelY = labelName->GetY() + labelName->GetHeight() + marginTopRow;
    mPanelCell->SetY(panelY);

    CreatePanelUnits();

    const int panelX = mPanelCell->GetX() + mPanelCell->GetWidth() + marginRightPanels;
    mPanelUnits->SetPosition(panelX, panelY);
}

void PanelPlayer::UpdateCells()
{
    const float cX = mLabelCells->GetX() + mLabelCells->GetWidth() * 0.5f;
    const float cY = mLabelCells->GetY() + mLabelCells->GetWidth() * 0.5f;

    mLabelCells->SetText(MakeStrCells().c_str());

    mLabelCells->SetPosition(cX - mLabelCells->GetWidth() * 0.5f, cY - mLabelCells->GetWidth() * 0.5f);
}

void PanelPlayer::UpdateCoins()
{
    const float cX = mLabelCoins->GetX() + mLabelCoins->GetWidth() * 0.5f;
    const float cY = mLabelCoins->GetY() + mLabelCoins->GetWidth() * 0.5f;

    mLabelCoins->SetText(MakeStrCoins().c_str());

    mLabelCoins->SetPosition(cX - mLabelCoins->GetWidth() * 0.5f, cY - mLabelCoins->GetWidth() * 0.5f);
}

void PanelPlayer::UpdateUnits()
{
    const float cX = mLabelUnits->GetX() + mLabelUnits->GetWidth() * 0.5f;
    const float cY = mLabelUnits->GetY() + mLabelUnits->GetWidth() * 0.5f;

    mLabelUnits->SetText(MakeStrUnits().c_str());

    mLabelUnits->SetPosition(cX - mLabelUnits->GetWidth() * 0.5f, cY - mLabelUnits->GetWidth() * 0.5f);
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

std::string PanelPlayer::MakeStrCells()
{
    return std::to_string(mPlayer->GetNumCells());
}

std::string PanelPlayer::MakeStrCoins()
{
    const int coins = mPlayer->GetMoney();

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

std::string PanelPlayer::MakeStrUnits()
{
    return std::to_string(mPlayer->GetNumUnits());
}

void PanelPlayer::CreatePanelCell()
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    mPanelCell = new Widget(this);

    FontManager * fm = FontManager::Instance();

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 32);
    fontHeader->SetStyle(Font::BOLD);

    Label * labelHeader = new Label("CELL", fontHeader, mPanelCell);
    labelHeader->SetColor(0x212121FF);

    int buttonY = labelHeader->GetY() + labelHeader->GetHeight() + 10;

    const int marginY = 30;

    Font * fontButton = fm->GetFont("data/fonts/OpenSans.ttf", 18);
    fontButton->SetStyle(Font::BOLD);

    mButtonNewUnit = new PushButton(mPanelCell);
    mButtonNewUnit->SetBackground("data/img/buttons/player_ui-01.png");
    mButtonNewUnit->SetLabel("NEW UNIT (10)", fontButton);
    mButtonNewUnit->SetLabelColor(0xF0F0F0FF);
    mButtonNewUnit->SetY(buttonY);

    buttonY += mButtonNewUnit->GetHeight() + marginY;

    mButtonCellFortify = new PushButton(mPanelCell);
    mButtonCellFortify->SetBackground("data/img/buttons/player_ui-01.png");
    mButtonCellFortify->SetLabel("FORTIFY (20)", fontButton);
    mButtonCellFortify->SetLabelColor(0xF0F0F0FF);
    mButtonCellFortify->SetY(buttonY);

    buttonY += mButtonCellFortify->GetHeight() + marginY;

    mButtonCellUpgrade = new PushButton(mPanelCell);
    mButtonCellUpgrade->SetBackground("data/img/buttons/player_ui-01.png");
    mButtonCellUpgrade->SetLabel("UPGRADE (50)", fontButton);
    mButtonCellUpgrade->SetLabelColor(0xF0F0F0FF);
    mButtonCellUpgrade->SetY(buttonY);
}

void PanelPlayer::CreatePanelUnits()
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    mPanelUnits = new Widget(this);

    FontManager * fm = FontManager::Instance();

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 32);
    fontHeader->SetStyle(Font::BOLD);

    Label * labelHeader = new Label("UNITS", fontHeader, mPanelUnits);
    labelHeader->SetColor(0x212121FF);

    int buttonY = labelHeader->GetY() + labelHeader->GetHeight() + 10;;

    const int marginY = 30;

    Font * fontButton = fm->GetFont("data/fonts/OpenSans.ttf", 18);
    fontButton->SetStyle(Font::BOLD);

    PushButton * buttonMove = new PushButton(mPanelUnits);
    buttonMove->SetBackground("data/img/buttons/player_ui-01.png");
    buttonMove->SetLabel("MOVE", fontButton);
    buttonMove->SetLabelColor(0xF0F0F0FF);
    buttonMove->SetY(buttonY);

    buttonY += buttonMove->GetHeight() + marginY;

    PushButton * buttonUpgrade = new PushButton(mPanelUnits);
    buttonUpgrade->SetBackground("data/img/buttons/player_ui-01.png");
    buttonUpgrade->SetLabel("UPGRADE (10)", fontButton);
    buttonUpgrade->SetLabelColor(0xF0F0F0FF);
    buttonUpgrade->SetY(buttonY);
}

} // namespace game
