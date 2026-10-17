#include "States/StateNewGameSetup.h"

#include "Screens/ScreenNewGameSetup.h"
#include "States/StatesIds.h"

namespace game
{

StateNewGameSetup::StateNewGameSetup(Game * game)
    : BaseGameState(StateId::NEW_GAME_SETUP, game)
{
}

void StateNewGameSetup::OnActive()
{
    mScreen = new ScreenNewGameSetup(mGame);
}

void StateNewGameSetup::OnInactive()
{
    delete mScreen;
    mScreen = nullptr;
}

} // namespace game
