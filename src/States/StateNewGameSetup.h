#pragma once

#include "BaseGameState.h"

namespace game
{

class StateNewGameSetup : public BaseGameState
{
public:
    StateNewGameSetup(Game * game);

private:
    void OnActive() override;
    void OnInactive() override;
};

} // namespace game
