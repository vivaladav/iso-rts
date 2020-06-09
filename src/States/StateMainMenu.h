#pragma once

#include "BaseGameState.h"

namespace game
{

class StateMainMenu : public BaseGameState
{
public:
    StateMainMenu(Game * game);

private:
    void OnActive() override;
    void OnInactive() override;
};

} // namespace game
