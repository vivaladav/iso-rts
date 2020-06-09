#include "Screens/Screen.h"

#include "Game.h"
#include "Screens/SharedScreenListener.h"

namespace game
{

Screen::Screen(Game * game)
    : mGame(game)
    , mSharedListener(new SharedScreenListener(this))
{
    mGame->AddApplicationListener(mSharedListener);
    mGame->AddKeyboardListener(mSharedListener);
}

Screen::~Screen()
{
    mGame->RemoveApplicationListener(mSharedListener);
    mGame->RemoveKeyboardListener(mSharedListener);

    delete mSharedListener;
}

} // namespace game
