#include "States/BaseGameState.h"

#include "Screens/Screen.h"

namespace game
{

BaseGameState::~BaseGameState()
{
    delete mScreen;
}

void BaseGameState::Update(float delta)
{
    mScreen->Update(delta);
}

void BaseGameState::Render()
{
    mScreen->Render();
}

} // namespace game
