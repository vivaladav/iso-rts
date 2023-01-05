#include "Screens/Screen.h"

#include "Game.h"
#include "Screens/SharedScreenListener.h"

#include <sgl/graphic/Camera.h>

namespace game
{

Screen::Screen(Game * game)
    : mGame(game)
    , mSharedListener(new SharedScreenListener(game, this))
{
    mGame->AddKeyboardListener(mSharedListener);

    auto cam = sgl::graphic::Camera::GetDefaultCamera();
    cam->ClearFunctionOnMove();
    cam->ResetPosition();
}

Screen::~Screen()
{
    delete mSharedListener;
}

} // namespace game
