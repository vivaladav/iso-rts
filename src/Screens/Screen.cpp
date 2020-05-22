#include "Screens/Screen.h"

#include "Game.h"

namespace game
{

void Screen::OnActive()
{
    mGame->SetEventListener(this);
}

} // namespace game
