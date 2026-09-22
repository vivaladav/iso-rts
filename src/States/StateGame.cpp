#include "States/StateGame.h"

#include "Game.h"
#include "GameStats.h"
#include "Planet.h"
#include "Screens/ScreenGame.h"
#include "States/StatesIds.h"

#include <sgl/graphic/Window.h>

namespace game
{

StateGame::StateGame(Game * game)
    : BaseGameState(StateId::GAME, game)
{
}

bool StateGame::Save(sgl::utilities::BinaryFile & bf) const
{
    BaseGameState::Save(bf);

    mScreen->Save(bf);

    return true;
}

void StateGame::OnActive()
{
    // create and init game screen
    auto screen = new ScreenGame(mGame);
    mScreen = screen;

    auto bf = mGame->GetSaveFileForReading();

    auto stats = mGame->GetGameStats();

    // start a new game
    if(bf == nullptr)
    {
        // update stats
        const PlanetId planet = mGame->GetCurrentPlanet()->GetPlanetId();
        const unsigned int map = mGame->GetCurrentTerritory();
        stats->IncMissionPlayed(planet, map);

        // init game
        screen->InitNewGame();

        // auto save, if enabled
        if(mGame->IsAutosaveEnabled())
        {
            mGame->SaveGame();
            screen->ShowLabelGameSaved();

#ifdef DEV_MODE
            // save again with different filename every turn
            mGame->SaveGameTurn();
#endif
        }
    }
    else
    {
        // update stats
        stats->IncLoadsPlayed();

        // load saved game
        screen->Load(*bf);
        screen->InitLoadedGame();

        mGame->CloseSaveFileForReading();
    }

    // setup listeners
    mGame->AddApplicationListener(mScreen);
    mGame->AddKeyboardListener(mScreen);
    mGame->AddMouseListener(mScreen);

    sgl::graphic::Window::Instance()->AddWindowListener(mScreen);
}

void StateGame::OnInactive()
{
    delete mScreen;
    mScreen = nullptr;
}

} // namespace game
