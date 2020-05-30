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
    mGame->AddEventListener(mSharedListener);
}

void Screen::OnInactive()
{
    mGame->RemoveEventListener(mSharedListener);
}

} // namespace game
