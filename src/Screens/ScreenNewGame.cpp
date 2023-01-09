#include "Screens/ScreenNewGame.h"

#include "Game.h"
#include "GameConstants.h"
#include "Player.h"
#include "AI/PlayerAI.h"
#include "GameObjects/ObjectsDataRegistry.h"
#include "GameObjects/Structure.h"
#include "GameObjects/Unit.h"
#include "States/StatesIds.h"

#include <sgl/sgui/Stage.h>

namespace game
{

// NOTE for now new game screen is dummy and it goes straight to next screen as
// it's not useful yet for the alpha builds.
// In the future it will allow to set your name, to set the difficulty and to choose play modes.
ScreenNewGame::ScreenNewGame(Game * game)
    : Screen(game)
    , mGame(game)
    , mDiff(Difficulty::EASY)
{
    const ObjectsDataRegistry * dataReg = mGame->GetObjectsRegistry();

    const int MAX_UNITS0 = 5;
    const int startEnergy = 800;
    const int startMaterial = 200;
    const int startMoney = 1000;
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
    p->SetResource(Player::Stat::BLOBS, startBlobs);
    p->SetResource(Player::Stat::DIAMONDS, startDiamonds);
    p->SetResource(Player::Stat::ENERGY, startEnergy);
    p->SetResource(Player::Stat::MATERIAL, startMaterial);
    p->SetResource(Player::Stat::MONEY, startMoney);

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
        p->SetResource(Player::Stat::BLOBS, startBlobs);
        p->SetResource(Player::Stat::DIAMONDS, startDiamonds);
        p->SetResource(Player::Stat::ENERGY, startEnergy);
        p->SetResource(Player::Stat::MATERIAL, startMaterial);
        p->SetResource(Player::Stat::MONEY, startMoney);

        ++indFaction;

        if(factions[indFaction] == pf)
            ++indFaction;
    }

    // set game difficulty
    game->SetDifficulty(mDiff);

    // move to game
    game->RequestNextActiveState(StateId::PLANET_MAP);
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
