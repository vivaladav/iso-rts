#include "Screens/Screen.h"

#include "Game.h"

namespace game
{

void Screen::OnActive()
{
    mGame->AddEventListener(this);
}

void Screen::OnInactive()
{
    mGame->RemoveEventListener(this);
}

} // namespace game
