#include "States/StateSettings.h"

#include "Screens/ScreenSettings.h"
#include "States/StatesIds.h"

namespace game
{

StateSettings::StateSettings(Game * game)
    : BaseGameState(StateId::SETTINGS, game)
{
}

void StateSettings::OnActive()
{
    mScreen = new ScreenSettings(mGame);
}

void StateSettings::OnInactive()
{
    delete mScreen;
    mScreen = nullptr;
}

} // namespace game
