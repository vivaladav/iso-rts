#include "Screens/ScreenNewGame.h"

#include "Game.h"
#include "Planet.h"
#include "GameObjects/GameObject.h"
#include "States/StatesIds.h"

#include <sgl/sgui/Stage.h>

#include <chrono>

namespace game
{

ScreenNewGame::ScreenNewGame(Game * game)
    : Screen(game)
    , mGame(game)
{
    // init game data
    mGame->InitNewGameData();

    // set seed for random values
    const auto t = std::chrono::system_clock::now().time_since_epoch().count();
    game->SetRandSeed(t);

    // start game from first mission
    game->SetCurrentPlanet(PLANET_1);
    game->SetCurrentTerritory(0);

    // add AI opponent
    game->AddToActivePlayersRandomAI();

    // move to GAME screen
    game->RequestNextActiveState(StateId::GAME);
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
