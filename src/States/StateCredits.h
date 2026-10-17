#pragma once

#include "BaseGameState.h"

namespace game
{

class StateCredits : public BaseGameState
{
public:
    StateCredits(Game * game);

private:
    void OnActive() override;
    void OnInactive() override;
};

} // namespace game
