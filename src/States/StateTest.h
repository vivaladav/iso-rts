#pragma once

#include "BaseGameState.h"

namespace game
{

class StateTest : public BaseGameState
{
public:
    StateTest(Game * game);

private:
    void OnActive() override;
    void OnInactive() override;
};

} // namespace game
