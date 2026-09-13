#include "States/StateInitGameSetup.h"

#include "Game.h"
#include "ResourceLoader.h"
#include "Screens/ScreenInitGameSetup.h"
#include "States/StatesIds.h"

namespace game
{

StateInitGameSetup::StateInitGameSetup(Game * game)
    : BaseGameState(StateId::INIT_GAME_SETUP, game)
{
}

void StateInitGameSetup::OnActive()
{
    // destroy textures
    auto loader = mGame->GetResourceLoader();
    loader->DestroyPreGameTextures();

    // create new screen
    mScreen = new ScreenInitGameSetup(mGame);
}

void StateInitGameSetup::OnInactive()
{
    delete mScreen;
    mScreen = nullptr;
}

} // namespace game
