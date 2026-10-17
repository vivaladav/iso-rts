#pragma once

#include "BaseGameState.h"

namespace game
{

class StateInitGameSetup : public BaseGameState
{
public:
    StateInitGameSetup(Game * game);

private:
    void OnActive() override;
    void OnInactive() override;
};

} // namespace game
