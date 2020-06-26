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

    const int marginX0 = 80;
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
    labelHeaderUnits->SetPosition(mLabelCells->GetX() + mLabelCells->GetWidth() + marginX1, Y0);
    labelHeaderUnits->SetColor(0x212121FF);

    mLabelUnits = new Label(MakeStrUnits().c_str(), fontData, this);
    mLabelUnits->SetPosition(labelHeaderUnits->GetX() + (labelHeaderUnits->GetWidth() - mLabelUnits->GetWidth()) * 0.5f,
                             labelHeaderUnits->GetY() + labelHeaderUnits->GetHeight());
    mLabelUnits->SetColor(0x212121FF);

    // -- subpanels --
    const int marginTopRow = 30;
    const int marginTopPanels = 20;

    CreatePanelCell();

    const int panelY1 = labelName->GetY() + labelName->GetHeight() + marginTopRow;
    mPanelCell->SetY(panelY1);

    CreatePanelUnits();

    const int panelY2 = mPanelCell->GetY() + mPanelCell->GetHeight() + marginTopPanels;
    mPanelUnits->SetY(panelY2);
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

    const int buttonsY = labelHeader->GetY() + labelHeader->GetHeight() + 10;
    int buttonX = 0;

    const int marginX = 30;

    Font * fontButton = fm->GetFont("data/fonts/OpenSans.ttf", 22);
    fontButton->SetStyle(Font::BOLD);

    PushButton * buttonNewUnit = new PushButton(mPanelCell);
    buttonNewUnit->SetBackground("data/img/buttons/player_ui-01.png");
    buttonNewUnit->SetLabel("NEW UNIT", fontButton);
    buttonNewUnit->SetLabelColor(0xF0F0F0FF);
    buttonNewUnit->SetPosition(buttonX, buttonsY);
    buttonNewUnit->SetOnClickFunction([] { std::cout << "OnClick NEW UNIT" << std::endl; });

    buttonX += buttonNewUnit->GetWidth() + marginX;

    PushButton * buttonFortify = new PushButton(mPanelCell);
    buttonFortify->SetBackground("data/img/buttons/player_ui-01.png");
    buttonFortify->SetLabel("FORTIFY", fontButton);
    buttonFortify->SetLabelColor(0xF0F0F0FF);
    buttonFortify->SetPosition(buttonX, buttonsY);

    buttonX += buttonFortify->GetWidth() + marginX;

    PushButton * buttonUpgrade = new PushButton(mPanelCell);
    buttonUpgrade->SetBackground("data/img/buttons/player_ui-01.png");
    buttonUpgrade->SetLabel("UPGRADE", fontButton);
    buttonUpgrade->SetLabelColor(0xF0F0F0FF);
    buttonUpgrade->SetPosition(buttonX, buttonsY);
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

    const int buttonsY = labelHeader->GetY() + labelHeader->GetHeight() + 10;
    int buttonX = 0;

    const int marginX = 30;

    Font * fontButton = fm->GetFont("data/fonts/OpenSans.ttf", 22);
    fontButton->SetStyle(Font::BOLD);

    PushButton * buttonMove = new PushButton(mPanelUnits);
    buttonMove->SetBackground("data/img/buttons/player_ui-01.png");
    buttonMove->SetLabel("MOVE", fontButton);
    buttonMove->SetLabelColor(0xF0F0F0FF);
    buttonMove->SetPosition(buttonX, buttonsY);

    buttonX += buttonMove->GetWidth() + marginX;

    PushButton * buttonUpgrade = new PushButton(mPanelUnits);
    buttonUpgrade->SetBackground("data/img/buttons/player_ui-01.png");
    buttonUpgrade->SetLabel("UPGRADE", fontButton);
    buttonUpgrade->SetLabelColor(0xF0F0F0FF);
    buttonUpgrade->SetPosition(buttonX, buttonsY);
}

} // namespace game
