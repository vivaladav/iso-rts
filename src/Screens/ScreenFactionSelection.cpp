#include "Screens/ScreenFactionSelection.h"

#include "Game.h"
#include "Player.h"
#include "States/StatesIds.h"
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

    const unsigned int colorHeader = 0xdbebf0ff;

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

    const int marginL = 50;
    const int marginPanelsH = 15;

    int y = 5;

    // LABEL "SELECT YOUR FACTION"
    fnt = fm->GetFont("data/fonts/Lato-Regular.ttf", 44, graphic::Font::NORMAL);
    sgui::Label * labelTitle = new sgui::Label("SELECT YOUR FACTION", fnt, panelMain);
    labelTitle->SetPosition(marginL, y);
    labelTitle->SetColor(colorHeader);

    // PANEL FACTION LOGO 1
    y = 100;

    tex = tm->GetSprite(SpriteFileFactionSelection, IND_FSEL_PANEL_F1);
    auto panelFaction = new sgui::Image(tex, panelMain);
    panelFaction->SetPosition(marginL, y);

    fnt = fm->GetFont("data/fonts/Lato-Regular.ttf", 32, graphic::Font::NORMAL);
    auto labelFactionName = new sgui::Label("Zulox", fnt, panelFaction);
    const int labelX = (panelFaction->GetWidth() - labelFactionName->GetWidth()) * 0.5f;
    labelFactionName->SetPosition(labelX, 10);
    labelFactionName->SetColor(colorHeader);

    // PANEL STORY 1
    y += panelFaction->GetHeight() + marginPanelsH;

    tex = tm->GetSprite(SpriteFileFactionSelection, IND_FSEL_PANEL_TXT);
    sgui::Image * panelTxt = new sgui::Image(tex, panelMain);
    panelTxt->SetPosition(marginL, y);

    // PANEL ATTRIBUTES 1
    y += panelTxt->GetHeight() + marginPanelsH;

    tex = tm->GetSprite(SpriteFileFactionSelection, IND_FSEL_PANEL_ATTR);
    sgui::Image * panelAttr = new sgui::Image(tex, panelMain);
    panelAttr->SetPosition(marginL, y);
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
        { 1103, 0, 300, 270 },
        { 1103, 271, 300, 270 },
        { 1103, 542, 300, 270 },
        { 1103, 813, 300, 100 },
        { 1404, 0, 300, 370 },

        // ATTRIBUTES BAR
        { 1103, 914, 10, 12 },
        { 1114, 914, 10, 14 },
        { 1125, 914, 10, 16 },
        { 1136, 914, 10, 18 },
        { 1147, 914, 10, 20 },
        { 1158, 914, 10, 22 },
        { 1169, 914, 10, 24 },
        { 1180, 914, 10, 26 },
        { 1191, 914, 10, 28 },
        { 1202, 914, 10, 30 },

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

} // namespace game
