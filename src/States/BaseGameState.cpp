#include "States/BaseGameState.h"

#include "Screens/Screen.h"

namespace game
{

void BaseGameState::Update()
{
    mScreen->Update();
}

void BaseGameState::Render()
{
    mScreen->Render();
}

} // namespace game
