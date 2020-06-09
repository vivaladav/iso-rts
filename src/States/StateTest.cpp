#include "States/StateTest.h"

#include "Game.h"
#include "Screens/ScreenTest.h"
#include "States/StatesIds.h"

namespace game
{

StateTest::StateTest(Game * game)
    : BaseGameState(StateId::TEST, game)
{
}

void StateTest::OnActive()
{
    mScreen = new ScreenTest(mGame);
    mGame->AddMouseListener(mScreen);
}

void StateTest::OnInactive()
{
    mGame->RemoveMouseListener(mScreen);

    delete mScreen;
    mScreen = nullptr;
}

} // namespace game
