#include "Screens/ScreenNewGame.h"

#include "Game.h"
#include "GameConstants.h"
#include "Player.h"
#include "GameObjects/ObjectsDataRegistry.h"
#include "GameObjects/Structure.h"
#include "GameObjects/Unit.h"
#include "AI/PlayerAI.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"
#include "Widgets/ButtonUnitsSelector.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/sgui/ButtonsGroup.h>
#include <sgl/sgui/Label.h>
#include <sgl/sgui/PushButton.h>
#include <sgl/sgui/Stage.h>

#include <iostream>
#include <string>

namespace game
{

// margins
const int marginHeaderB = 20;
const int marginMapH = 174;
const int marginMapV = 60;

// maps
const int mapX0 = 125;
const int rows = 2;
const int cols = 3;
const int mapsPerScreen = rows * cols;

ScreenNewGame::ScreenNewGame(Game * game)
    : Screen(game)
    , mGame(game)
    , mDiff(Difficulty::EASY)
{
    using namespace sgl::graphic;
    using namespace sgl::sgui;

    game->SetClearColor(0x0F, 0x0F, 0x0F, 0x0F);

    const int marginL = 25;
    const int marginT = 10;
    const int marginTitleB = 50;
    const int marginWidgetsH = 100;
    const int marginWidgetsV = 50;
    int widgetY = marginT;

    // -- TITLE --
    const unsigned int colorTitle = 0x888888FF;
    FontManager * fm = FontManager::Instance();

    Font * fontTitle = fm->GetFont("data/fonts/OpenSans.ttf", 48, Font::BOLD);

    auto * title = new Label("SINGLE PLAYER GAME", fontTitle);
    title->SetColor(colorTitle);
    title->SetPosition(marginL, widgetY);

    widgetY += title->GetHeight() + marginTitleB;

    // -- CPU PLAYERS --
    const unsigned int colorHeader = 0x777777FF;
    Font * fontHeader = fm->GetFont("data/fonts/OpenSans.ttf", 32, Font::BOLD);

    Label * headerPlayers = new Label("CPU PLAYERS", fontHeader);
    headerPlayers->SetColor(colorHeader);
    headerPlayers->SetPosition(marginL, widgetY);

    widgetY += headerPlayers->GetHeight() + marginHeaderB;

    // buttons
    auto * bgPlayers = new ButtonsGroup(ButtonsGroup::HORIZONTAL);
    bgPlayers->SetPosition(headerPlayers->GetX(), widgetY);

    bgPlayers->AddButton(new ButtonUnitsSelector("1"));
    bgPlayers->AddButton(new ButtonUnitsSelector("2"));

    bgPlayers->SetButtonChecked(0, true);

    bgPlayers->SetFunctionOnToggle([this](unsigned int ind, bool checked)
    {
        if(checked)
            mCpuPlayers = ind + 1;
    });

    // -- DIFFICULTY --
    auto * headerDiff = new Label("DIFFICULTY", fontHeader);
    headerDiff->SetColor(colorHeader);
    headerDiff->SetPosition(headerPlayers->GetX() + headerPlayers->GetWidth() + marginWidgetsH, headerPlayers->GetY());

    // buttons
    auto * bgDiff = new ButtonsGroup(ButtonsGroup::HORIZONTAL);
    bgDiff->SetPosition(headerDiff->GetX(), widgetY);

    bgDiff->AddButton(new ButtonUnitsSelector("1"));
    bgDiff->AddButton(new ButtonUnitsSelector("2"));
    bgDiff->AddButton(new ButtonUnitsSelector("3"));

    bgDiff->SetButtonChecked(0, true);

    bgDiff->SetFunctionOnToggle([this](unsigned int ind, bool checked)
    {
        if(checked)
            mDiff = static_cast<Difficulty>(ind);
    });

    // move down
    widgetY += bgDiff->GetHeight() + marginWidgetsV;
    // -- NAVIGATION PANEL --
    const int marginButtonsH = 100;
    const int marginButtonsB = 25;

    Widget * panel = new Widget;

    // BUTTON BACK
    ButtonMainMenu * button = new ButtonMainMenu("BACK", panel);

    button->AddOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::FACTION_SEL);
    });

    const int buttonBackW = button->GetWidth();

    // -- BUTTON NEW GAME --
    mButtonStart = new ButtonMainMenu("START", panel);
    mButtonStart->SetX(buttonBackW + marginButtonsH);

    mButtonStart->AddOnClickFunction([this, game]
    {
        const ObjectsDataRegistry * dataReg = mGame->GetObjectsRegistry();

        const int MAX_UNITS0 = 5;
        const int startEnergy = 400;
        const int startMaterial = 50;
        const int startMoney = 500;
        const int startDiamonds = 10;
        const int startBlobs = 5;

        // create human player
        const PlayerFaction pf = game->GetLocalPlayerFaction();
        Player * p = game->AddPlayer("PLAYER 1", 0);
        p->SetLocal(true);
        p->SetFaction(pf);
        p->SetMaxUnits(MAX_UNITS0);

        // assign initial available structures
        p->AddAvailableStructure(dataReg->GetStructure(pf, STRUCT_RES_STO_ENERGY));
        p->AddAvailableStructure(dataReg->GetStructure(pf, STRUCT_RES_STO_MATERIAL));
        p->AddAvailableStructure(dataReg->GetStructure(pf, STRUCT_RES_STO_DIAMONDS));
        p->AddAvailableStructure(dataReg->GetStructure(pf, STRUCT_RES_STO_BLOBS));
        p->AddAvailableStructure(dataReg->GetStructure(pf, STRUCT_WALL_GATE));
        p->AddAvailableStructure(dataReg->GetStructure(pf, STRUCT_DEF_TOWER));
        p->AddAvailableStructure(dataReg->GetStructure(pf, STRUCT_RES_GEN_MATERIAL_EXTRACTOR));
        p->AddAvailableStructure(dataReg->GetStructure(pf, STRUCT_RES_GEN_SOLAR));
        p->AddAvailableStructure(dataReg->GetStructure(pf, STRUCT_RADAR));
        p->AddAvailableStructure(dataReg->GetStructure(pf, STRUCT_TARGET));

        // assign initial available units
        p->AddAvailableUnit(dataReg->GetUnit(pf, UNIT_1));
        p->AddAvailableUnit(dataReg->GetUnit(pf, UNIT_2));
        // TODO temporary for testing. In the future start only with 2 types
        p->AddAvailableUnit(dataReg->GetUnit(pf, UNIT_3));
        p->AddAvailableUnit(dataReg->GetUnit(pf, UNIT_4));

        // assign initial resources
        p->SumResource(Player::Stat::BLOBS, startBlobs);
        p->SumResource(Player::Stat::DIAMONDS, startDiamonds);
        p->SumResource(Player::Stat::ENERGY, startEnergy);
        p->SumResource(Player::Stat::MATERIAL, startMaterial);
        p->SumResource(Player::Stat::MONEY, startMoney);

        // create AI players
        const char * strPlayers[] =
        {
            "PLAYER 2",
            "PLAYER 3"
        };

        const PlayerFaction factions[] =
        {
            FACTION_1,
            FACTION_2,
            FACTION_3
        };

        int indFaction = (FACTION_1 == pf) ? 1 : 0;

        for(int i = 0; i < mCpuPlayers; ++i)
        {
            const int playerId = i + 1;
            const PlayerFaction facAI = factions[indFaction];

            p = game->AddPlayer(strPlayers[i], playerId);
            p->SetFaction(facAI);
            p->SetMaxUnits(MAX_UNITS0);
            auto * ai = new PlayerAI(p);
            p->SetAI(ai);

            // assign initial available structures
            p->AddAvailableStructure(dataReg->GetStructure(facAI, STRUCT_RES_STO_ENERGY));
            p->AddAvailableStructure(dataReg->GetStructure(facAI, STRUCT_RES_STO_MATERIAL));
            p->AddAvailableStructure(dataReg->GetStructure(facAI, STRUCT_RES_STO_DIAMONDS));
            p->AddAvailableStructure(dataReg->GetStructure(facAI, STRUCT_RES_STO_BLOBS));
            p->AddAvailableStructure(dataReg->GetStructure(facAI, STRUCT_WALL_GATE));
            p->AddAvailableStructure(dataReg->GetStructure(facAI, STRUCT_DEF_TOWER));
            p->AddAvailableStructure(dataReg->GetStructure(facAI, STRUCT_RADAR));
            p->AddAvailableStructure(dataReg->GetStructure(facAI, STRUCT_RES_GEN_MATERIAL_EXTRACTOR));
            p->AddAvailableStructure(dataReg->GetStructure(facAI, STRUCT_RES_GEN_SOLAR));
            p->AddAvailableStructure(dataReg->GetStructure(facAI, STRUCT_TARGET));

            // assign initial available units
            p->AddAvailableUnit(dataReg->GetUnit(facAI, UNIT_1));
            p->AddAvailableUnit(dataReg->GetUnit(facAI, UNIT_2));
            // TODO temporary for testing. In the future start only with 2 types
            p->AddAvailableUnit(dataReg->GetUnit(facAI, UNIT_3));
            p->AddAvailableUnit(dataReg->GetUnit(facAI, UNIT_4));

            // assign initial resources
            p->SumResource(Player::Stat::BLOBS, startBlobs);
            p->SumResource(Player::Stat::DIAMONDS, startDiamonds);
            p->SumResource(Player::Stat::ENERGY, startEnergy);
            p->SumResource(Player::Stat::MATERIAL, startMaterial);
            p->SumResource(Player::Stat::MONEY, startMoney);

            ++indFaction;

            if(factions[indFaction] == pf)
                ++indFaction;
        }

        // set game difficulty
        game->SetDifficulty(mDiff);

        // move to game
        game->RequestNextActiveState(StateId::PLANET_MAP);
    });

    // position buttons panel
    const int containerX = (Renderer::Instance()->GetWidth() - panel->GetWidth()) * 0.5f;
    const int containerY = Renderer::Instance()->GetHeight() - panel->GetHeight() - marginButtonsB;
    panel->SetPosition(containerX, containerY);
}

ScreenNewGame::~ScreenNewGame()
{
    sgl::sgui::Stage::Instance()->ClearWidgets();
}

void ScreenNewGame::Update(float update)
{
}

void ScreenNewGame::Render()
{
}

} // namespace game
