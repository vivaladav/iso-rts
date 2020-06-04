#include "Screens/Screen.h"

#include "Game.h"
#include "Screens/SharedScreenListener.h"

namespace game
{

Screen::Screen(unsigned int screenId, Game * game)
    : lib::utilities::State(screenId)
    , mGame(game)
    , mSharedListener(new SharedScreenListener(this))
{
}

Screen::~Screen() { delete mSharedListener; }

void Screen::OnActive()
{
    mGame->AddApplicationListener(mSharedListener);
    mGame->AddKeyboardListener(mSharedListener);
}

void Screen::OnInactive()
{
    mGame->RemoveApplicationListener(mSharedListener);
    mGame->RemoveKeyboardListener(mSharedListener);
}

} // namespace game
