#include "Screens/ScreenFactionSelection.h"

#include "Game.h"
#include "Player.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonDialogSelect.h"
#include "Widgets/GameUIData.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/Renderer.h>
#include <graphic/Texture.h>
#include <graphic/TextureManager.h>
#include <sgui/Image.h>
#include <sgui/Label.h>
#include <sgui/Stage.h>

#include <cmath>
#include <iostream>
#include <string>

namespace game
{

ScreenFactionSelection::ScreenFactionSelection(Game * game)
    : Screen(game)
    , mFaction(NO_FACTION)
{
    using namespace lib;

    InitSprites();

    const int screenW = graphic::Renderer::Instance()->GetWidth();

    mBg = new graphic::Image("data/img/space_bg.jpg");

    auto tm = graphic::TextureManager::Instance();
    auto fm = graphic::FontManager::Instance();
    graphic::Texture * tex = nullptr;
    graphic::Font * fnt = nullptr;

    // MAIN PANEL
    tex = tm->GetSprite(SpriteFileFactionSelection, IND_FSEL_MAIN_BG);
    sgui::Image * panelMain = new sgui::Image(tex);

    const int pmX = (screenW - panelMain->GetWidth()) * 0.5f;
    const int pmY = 30;

    panelMain->SetPosition(pmX, pmY);

    const unsigned int colorHeader = 0xdbebf0ff;
    const unsigned int colorText = 0xb8d3e0ff;

    const int marginL = 50;
    const int marginPanelsH = 15;
    const int marginTxt = 10;
    const int marginFaction = 50;

    const int y0 = 5;
    const int contentY0 = 100;

    int x = marginL;
    int y = y0;

    // LABEL "SELECT YOUR FACTION"
    fnt = fm->GetFont("data/fonts/Lato-Regular.ttf", 44, graphic::Font::NORMAL);
    sgui::Label * labelTitle = new sgui::Label("SELECT YOUR FACTION", fnt, panelMain);
    labelTitle->SetPosition(x, y);
    labelTitle->SetColor(colorHeader);

    graphic::Font * fntFaction = fm->GetFont("data/fonts/Lato-Regular.ttf", 32, graphic::Font::NORMAL);
    graphic::Font * fntTxt = fm->GetFont("data/fonts/Lato-Regular.ttf", 20, graphic::Font::NORMAL);

    // -- FACTION 1 --
    // PANEL FACTION LOGO
    y = contentY0;

    tex = tm->GetSprite(SpriteFileFactionSelection, IND_FSEL_PANEL_F1);
    auto panelFaction = new sgui::Image(tex, panelMain);
    panelFaction->SetPosition(x, y);

    auto labelFactionName = new sgui::Label("Zulox", fntFaction, panelFaction);
    int labelX = (panelFaction->GetWidth() - labelFactionName->GetWidth()) * 0.5f;
    labelFactionName->SetPosition(labelX, 10);
    labelFactionName->SetColor(colorHeader);

    y += panelFaction->GetHeight() + marginPanelsH;

    // PANEL STORY
    tex = tm->GetSprite(SpriteFileFactionSelection, IND_FSEL_PANEL_TXT);
    auto panelTxt = new sgui::Image(tex, panelMain);
    panelTxt->SetPosition(x, y);

    int txtX = marginTxt;
    int txtY = marginTxt;

    auto labelStory = new sgui::Label("Trained warriors and expert",
                                      fntTxt, panelTxt);
    labelStory->SetPosition(txtX, txtY);
    labelStory->SetColor(colorText);

    txtY += labelStory->GetHeight();

    labelStory = new sgui::Label("weapon makers. They hate the",
                                 fntTxt, panelTxt);
    labelStory->SetPosition(txtX, txtY);
    labelStory->SetColor(colorText);

    txtY += labelStory->GetHeight();

    labelStory = new sgui::Label("Domens.", fntTxt, panelTxt);
    labelStory->SetPosition(txtX, txtY);
    labelStory->SetColor(colorText);

    y += panelTxt->GetHeight() + marginPanelsH;

    // PANEL ATTRIBUTES

    int panelStatsH = AddPanelStats(x, y, { 8, 6, 10, 3 }, panelMain);

    y += panelStatsH + marginPanelsH;

    // BUTTON SELECT
    auto btnSel = new ButtonDialogSelect(panelMain);
    btnSel->SetCheckable(true);

    int btnX = x + (panelFaction->GetWidth() - btnSel->GetWidth()) * 0.5f;
    btnSel->SetPosition(btnX, y);

    x += panelFaction->GetWidth() + marginFaction;

    // -- FACTION 2 --
    // PANEL FACTION LOGO
    y = contentY0;

    tex = tm->GetSprite(SpriteFileFactionSelection, IND_FSEL_PANEL_F2);
    panelFaction = new sgui::Image(tex, panelMain);
    panelFaction->SetPosition(x, y);

    labelFactionName = new sgui::Label("Domens", fntFaction, panelFaction);
    labelX = (panelFaction->GetWidth() - labelFactionName->GetWidth()) * 0.5f;
    labelFactionName->SetPosition(labelX, 10);
    labelFactionName->SetColor(colorHeader);

    y += panelFaction->GetHeight() + marginPanelsH;

    // PANEL STORY
    tex = tm->GetSprite(SpriteFileFactionSelection, IND_FSEL_PANEL_TXT);
    panelTxt = new sgui::Image(tex, panelMain);
    panelTxt->SetPosition(x, y);

    txtX = marginTxt;
    txtY = marginTxt;

    labelStory = new sgui::Label("Masters of engineering and",
                                 fntTxt, panelTxt);
    labelStory->SetPosition(txtX, txtY);
    labelStory->SetColor(colorText);

    txtY += labelStory->GetHeight();

    labelStory = new sgui::Label("science. Long time enemies of",
                                 fntTxt, panelTxt);
    labelStory->SetPosition(txtX, txtY);
    labelStory->SetColor(colorText);

    txtY += labelStory->GetHeight();

    labelStory = new sgui::Label("the Zulox.", fntTxt, panelTxt);
    labelStory->SetPosition(txtX, txtY);
    labelStory->SetColor(colorText);

    y += panelTxt->GetHeight() + marginPanelsH;

    // PANEL ATTRIBUTES

    panelStatsH = AddPanelStats(x, y, { 6, 9, 3, 8 }, panelMain);

    y += panelStatsH + marginPanelsH;

    // BUTTON SELECT
    btnSel = new ButtonDialogSelect(panelMain);
    btnSel->SetCheckable(true);

    btnX = x + (panelFaction->GetWidth() - btnSel->GetWidth()) * 0.5f;
    btnSel->SetPosition(btnX, y);

    x += panelFaction->GetWidth() + marginFaction;

    // -- FACTION 3 --
    // PANEL FACTION LOGO
    y = contentY0;

    tex = tm->GetSprite(SpriteFileFactionSelection, IND_FSEL_PANEL_F3);
    panelFaction = new sgui::Image(tex, panelMain);
    panelFaction->SetPosition(x, y);

    fnt = fm->GetFont("data/fonts/Lato-Regular.ttf", 32, graphic::Font::NORMAL);
    labelFactionName = new sgui::Label("Alphen", fnt, panelFaction);
    labelX = (panelFaction->GetWidth() - labelFactionName->GetWidth()) * 0.5f;
    labelFactionName->SetPosition(labelX, 10);
    labelFactionName->SetColor(colorHeader);

    y += panelFaction->GetHeight() + marginPanelsH;

    // PANEL STORY
    tex = tm->GetSprite(SpriteFileFactionSelection, IND_FSEL_PANEL_TXT);
    panelTxt = new sgui::Image(tex, panelMain);
    panelTxt->SetPosition(x, y);

    txtX = marginTxt;
    txtY = marginTxt;

    labelStory = new sgui::Label("Great explorers and very",
                                 fntTxt, panelTxt);
    labelStory->SetPosition(txtX, txtY);
    labelStory->SetColor(colorText);

    txtY += labelStory->GetHeight();

    labelStory = new sgui::Label("versatile. They have never",
                                 fntTxt, panelTxt);
    labelStory->SetPosition(txtX, txtY);
    labelStory->SetColor(colorText);

    txtY += labelStory->GetHeight();

    labelStory = new sgui::Label("fought other factions until now.",
                                 fntTxt, panelTxt);
    labelStory->SetPosition(txtX, txtY);
    labelStory->SetColor(colorText);

    y += panelTxt->GetHeight() + marginPanelsH;

    // PANEL ATTRIBUTES

    panelStatsH = AddPanelStats(x, y, { 9, 6, 5, 6 }, panelMain);

    y += panelStatsH + marginPanelsH;

    // BUTTON SELECT
    btnSel = new ButtonDialogSelect(panelMain);
    btnSel->SetCheckable(true);

    btnX = x + (panelFaction->GetWidth() - btnSel->GetWidth()) * 0.5f;
    btnSel->SetPosition(btnX, y);

    x += panelFaction->GetWidth() + marginFaction;
}

ScreenFactionSelection::~ScreenFactionSelection()
{
    lib::sgui::Stage::Instance()->ClearWidgets();

    delete mBg;
}

void ScreenFactionSelection::Update(float update)
{
}

void ScreenFactionSelection::Render()
{
    mBg->Render();
}

void ScreenFactionSelection::InitSprites()
{
    // NOTE this code will need to be moved to a dedicated screen to init data on start
    auto tm = lib::graphic::TextureManager::Instance();

    // CELLS
    std::vector<lib::core::Rectd> rects
    {
        // PANELS
        { 0, 0, 1102, 970 },
        { 1103, 0, 300, 260 },
        { 1103, 261, 300, 260 },
        { 1103, 522, 300, 260 },
        { 1103, 783, 300, 110 },
        { 1404, 0, 300, 370 },

        // ATTRIBUTES BAR
        { 1103, 894, 10, 12 },
        { 1114, 894, 10, 14 },
        { 1125, 894, 10, 16 },
        { 1136, 894, 10, 18 },
        { 1147, 894, 10, 20 },
        { 1158, 894, 10, 22 },
        { 1169, 894, 10, 24 },
        { 1180, 894, 10, 26 },
        { 1191, 894, 10, 28 },
        { 1202, 894, 10, 30 },

        // BACK BUTTON
        { 0, 971, 320, 50 },
        { 321, 971, 320, 50 },
        { 642, 971, 320, 50 },

        // SELECT BUTTON
        { 1404, 554, 250, 50 },
        { 1404, 605, 250, 50 },
        { 1404, 656, 250, 50 },
        { 1404, 707, 250, 50 },

        // CONTINUE BUTTON
        { 1404, 371, 300, 60 },
        { 1404, 432, 300, 60 },
        { 1404, 493, 300, 60 }
    };

    tm->RegisterSprite(SpriteFileFactionSelection, rects);
}

int ScreenFactionSelection::AddPanelStats(int x, int y, const std::array<int, NUM_FACTION_STATS> & stats,
                                           lib::sgui::Widget * parent)
{
    using namespace lib;

    const unsigned int colorHeader = 0xdbebf0ff;

    auto tm = graphic::TextureManager::Instance();
    auto fm = graphic::FontManager::Instance();

    graphic::Texture * tex = tm->GetSprite(SpriteFileFactionSelection, IND_FSEL_PANEL_ATTR);
    auto panel = new sgui::Image(tex, parent);
    panel->SetPosition(x, y);

    graphic::Font * fnt = fm->GetFont("data/fonts/Lato-Regular.ttf", 20, graphic::Font::NORMAL);

    const int marginHeaderB = 5;
    const int marginBlockB = 36;

    int contX = 0;
    int contY = 0;

    // CONTENT PANEL
    auto panelContent = new sgui::Widget(panel);

    const char * strHeader[NUM_FACTION_STATS] =
    {
        "EXPLORATION",
        "CONSTRUCTION",
        "COMBAT",
        "TECHNOLOGY"
    };

    const int attLevels = 4;
    const unsigned int colorPip[attLevels] =
    {
        0xf5a3a3ff,
        0xf5daa3ff,
        0xa6f2a6ff,
        0xa3daf5ff
    };
    const int treshPip[attLevels] = { 4, 7, 9, 11 };

    const unsigned int colorPipOff = 0x365463ff;

    for(unsigned int i = 0; i < NUM_FACTION_STATS; ++i)
    {
        const unsigned int stat = stats[i];

        // header label
        auto header0 = new sgui::Label(strHeader[i], fnt, panelContent);
        header0->SetPosition(contX, contY);
        header0->SetColor(colorHeader);

        contY += header0->GetHeight() + marginHeaderB;

        // stat bar
        const int maxH = tm->GetSprite(SpriteFileFactionSelection, IND_FSEL_PIP9)->GetHeight();

        int colorInd = attLevels - 1;

        if(stat < treshPip[0])
            colorInd = 0;
        else if(stat < treshPip[1])
            colorInd = 1;
        else if(stat < treshPip[2])
            colorInd = 2;

        for(unsigned int p = 0; p < NUM_FSEL_PIPS; ++p)
        {
            tex = tm->GetSprite(SpriteFileFactionSelection, IND_FSEL_PIP0 + p);
            auto pip = new sgui::Image(tex, panelContent);
            pip->SetPosition(contX, contY + maxH - pip->GetHeight());

            if(p < stat)
                pip->SetColor(colorPip[colorInd]);
            else
                pip->SetColor(colorPipOff);

            contX += pip->GetWidth() * 2;
        }

        contX = 0;
        contY += maxH + marginBlockB;
    }

    // position content panel
    const int posContX = (panel->GetWidth() - panelContent->GetWidth()) * 0.5f;
    const int posContY = (panel->GetHeight() - panelContent->GetHeight()) * 0.5f;

    panelContent->SetPosition(posContX, posContY);

    return panel->GetHeight();
}

} // namespace game
