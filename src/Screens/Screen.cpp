#include "Screens/Screen.h"

#include "Game.h"

void Screen::OnActive()
{
    mApp->SetEventListener(this);
}
