#include "States/StateInitGame.h"

#include "Game.h"
#include "ResourceLoader.h"
#include "Screens/ScreenInitGame.h"
#include "States/StatesIds.h"

namespace game
{

StateInitGame::StateInitGame(Game * game)
    : BaseGameState(StateId::INIT_GAME, game)
{
}

void StateInitGame::OnActive()
{
    // destroy setup textures
    auto loader = mGame->GetResourceLoader();
    loader->DestroyGameSetupTextures();

    mScreen = new ScreenInitGame(mGame);
}

void StateInitGame::OnInactive()
{
    delete mScreen;
    mScreen = nullptr;
}

} // namespace game
