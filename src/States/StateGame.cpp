#include "States/StateGame.h"

#include "Game.h"
#include "Screens/ScreenGame.h"
#include "States/StatesIds.h"

namespace game
{

StateGame::StateGame(Game * game)
    : BaseGameState(StateId::GAME, game)
{
}

void StateGame::OnActive()
{
    mScreen = new ScreenGame(mGame);
    mGame->AddMouseListener(mScreen);
}

void StateGame::OnInactive()
{
    delete mScreen;
    mScreen = nullptr;
}

} // namespace game
