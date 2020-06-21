#include "PanelPlayer.h"

#include "Player.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <sgui/Label.h>
#include <sgui/PushButton.h>

#include <iostream>

namespace game
{

PanelPlayer::PanelPlayer(Player * p, lib::sgui::Widget * parent)
    : lib::sgui::Widget(0, parent)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    int wid = 0;

    FontManager * fm = FontManager::Instance();

    Font * fontName = fm->GetFont("data/fonts/OpenSans.ttf", 48);
    fontName->SetStyle(Font::BOLD);

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 28);

    Font * fontData = fm->GetFont("data/fonts/OpenSans.ttf", 40);

    Label * labelName = new Label(p->GetName().c_str(), wid++, fontName, this);
    labelName->SetColor(0x212121FF);

    const int marginX0 = 80;
    const int marginX1 = 50;

    const int Y0 = 10;

    Label * labelHeader1 = new Label("COINS", wid++, fontHeader, this);
    labelHeader1->SetPosition(labelName->GetX() + labelName->GetWidth() + marginX0, Y0);
    labelHeader1->SetColor(0x212121FF);

    Label * labelData1 = new Label("000", wid++, fontData, this);
    labelData1->SetPosition(labelHeader1->GetX() + (labelHeader1->GetWidth() - labelData1->GetWidth()) * 0.5f,
                            labelHeader1->GetY() + labelHeader1->GetHeight());
    labelData1->SetColor(0x212121FF);

    Label * labelHeader2 = new Label("CELLS", wid++, fontHeader, this);
    labelHeader2->SetPosition(labelHeader1->GetX() + labelHeader1->GetWidth() + marginX1, Y0);
    labelHeader2->SetColor(0x212121FF);

    Label * labelData2 = new Label("000", wid++, fontData, this);
    labelData2->SetPosition(labelHeader2->GetX() + (labelHeader2->GetWidth() - labelData2->GetWidth()) * 0.5f,
                            labelHeader2->GetY() + labelHeader2->GetHeight());
    labelData2->SetColor(0x212121FF);

    Label * labelHeader3 = new Label("UNITS", wid++, fontHeader, this);
    labelHeader3->SetPosition(labelHeader2->GetX() + labelHeader2->GetWidth() + marginX1, Y0);
    labelHeader3->SetColor(0x212121FF);

    Label * labelData3 = new Label("000", wid++, fontData, this);
    labelData3->SetPosition(labelHeader3->GetX() + (labelHeader3->GetWidth() - labelData3->GetWidth()) * 0.5f,
                            labelHeader3->GetY() + labelHeader3->GetHeight());
    labelData3->SetColor(0x212121FF);

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

void PanelPlayer::CreatePanelCell()
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    int wid = 100;

    mPanelCell = new Widget(wid++, this);

    FontManager * fm = FontManager::Instance();

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 32);
    fontHeader->SetStyle(Font::BOLD);

    Label * labelHeader = new Label("CELL", wid++, fontHeader, mPanelCell);
    labelHeader->SetColor(0x212121FF);

    const int buttonsY = labelHeader->GetY() + labelHeader->GetHeight() + 10;
    int buttonX = 0;

    const int marginX = 30;

    PushButton * buttonNewUnit = new PushButton(wid++, mPanelCell);
    buttonNewUnit->SetBackground("data/img/buttons/player_ui-01.png");
    buttonNewUnit->SetLabel("NEW UNIT");
    buttonNewUnit->SetPosition(buttonX, buttonsY);
    buttonNewUnit->SetOnClickFunction([] { std::cout << "OnClick NEW UNIT" << std::endl; });

    buttonX += buttonNewUnit->GetWidth() + marginX;

    PushButton * buttonFortify = new PushButton(wid++, mPanelCell);
    buttonFortify->SetBackground("data/img/buttons/player_ui-01.png");
    buttonFortify->SetLabel("FORTIFY");
    buttonFortify->SetPosition(buttonX, buttonsY);

    buttonX += buttonFortify->GetWidth() + marginX;

    PushButton * buttonUpgrade = new PushButton(wid++, mPanelCell);
    buttonUpgrade->SetBackground("data/img/buttons/player_ui-01.png");
    buttonUpgrade->SetLabel("UPGRADE");
    buttonUpgrade->SetPosition(buttonX, buttonsY);

}

void PanelPlayer::CreatePanelUnits()
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    int wid = 200;

    mPanelUnits = new Widget(wid++, this);

    FontManager * fm = FontManager::Instance();

    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 32);
    fontHeader->SetStyle(Font::BOLD);

    Label * labelHeader = new Label("UNITS", wid++, fontHeader, mPanelUnits);
    labelHeader->SetColor(0x212121FF);

    const int buttonsY = labelHeader->GetY() + labelHeader->GetHeight() + 10;
    int buttonX = 0;

    const int marginX = 30;

    PushButton * buttonMove = new PushButton(wid++, mPanelUnits);
    buttonMove->SetBackground("data/img/buttons/player_ui-01.png");
    buttonMove->SetLabel("MOVE");
    buttonMove->SetPosition(buttonX, buttonsY);

    buttonX += buttonMove->GetWidth() + marginX;

    PushButton * buttonUpgrade = new PushButton(wid++, mPanelUnits);
    buttonUpgrade->SetBackground("data/img/buttons/player_ui-01.png");
    buttonUpgrade->SetLabel("UPGRADE");
    buttonUpgrade->SetPosition(buttonX, buttonsY);
}

} // namespace game
