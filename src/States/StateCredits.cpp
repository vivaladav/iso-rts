#include "States/StateCredits.h"

#include "Screens/ScreenCredits.h"
#include "States/StatesIds.h"

namespace game
{

StateCredits::StateCredits(Game * game)
    : BaseGameState(StateId::CREDITS, game)
{
}

void StateCredits::OnActive()
{
    mScreen = new ScreenCredits(mGame);
}

void StateCredits::OnInactive()
{
    delete mScreen;
    mScreen = nullptr;
}

} // namespace game
