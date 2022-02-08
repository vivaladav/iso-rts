#include "Screens/Screen.h"

#include "Game.h"
#include "Screens/SharedScreenListener.h"

#include <sgl/graphic/Camera.h>

namespace game
{

Screen::Screen(Game * game)
    : mGame(game)
    , mSharedListener(new SharedScreenListener(this))
{
    mGame->AddApplicationListener(mSharedListener);
    mGame->AddKeyboardListener(mSharedListener);

    lib::graphic::Camera::GetDefaultCamera()->ResetPosition();
}

Screen::~Screen()
{
    delete mSharedListener;
}

} // namespace game
