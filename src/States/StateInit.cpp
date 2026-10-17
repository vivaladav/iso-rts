#include "States/StateInit.h"

#include "Game.h"
#include "ResourceLoader.h"
#include "Screens/ScreenInit.h"
#include "States/StatesIds.h"

namespace game
{

StateInit::StateInit(Game * game)
    : BaseGameState(StateId::INIT, game)
{
}

void StateInit::OnActive()
{
    // delete game and game setup data when restarting
    if(!mFirstInit)
    {
        auto loader = mGame->GetResourceLoader();
        loader->DestroyGameSetupTextures();
        loader->DestroyGameTextures();
    }

    mScreen = new ScreenInit(mGame, mFirstInit);

    mFirstInit = false;
}

void StateInit::OnInactive()
{
    delete mScreen;
    mScreen = nullptr;
}

} // namespace game
