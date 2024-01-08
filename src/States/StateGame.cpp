#include "States/StateGame.h"

#include "Game.h"
#include "Screens/ScreenGame.h"
#include "States/StatesIds.h"

#include <sgl/graphic/Window.h>

namespace game
{

StateGame::StateGame(Game * game)
    : BaseGameState(StateId::GAME, game)
{
}

void StateGame::OnActive()
{
    mScreen = new ScreenGame(mGame);

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
