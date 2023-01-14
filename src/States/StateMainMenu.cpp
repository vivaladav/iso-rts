#include "States/StateMainMenu.h"

#include "Game.h"
#include "Screens/ScreenMainMenu.h"
#include "States/StatesIds.h"

namespace game
{

StateMainMenu::StateMainMenu(Game * game)
    : BaseGameState(StateId::MAIN_MENU, game)
{
}

void StateMainMenu::OnActive()
{
    mScreen = new ScreenMainMenu(mGame);

    mGame->AddApplicationListener(mScreen);
    mGame->AddKeyboardListener(mScreen);
}

void StateMainMenu::OnInactive()
{
    delete mScreen;
    mScreen = nullptr;
}

} // namespace game
