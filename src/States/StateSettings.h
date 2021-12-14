#pragma once

#include "BaseGameState.h"

namespace game
{

class StateSettings : public BaseGameState
{
public:
    StateSettings(Game * game);

private:
    void OnActive() override;
    void OnInactive() override;
};

} // namespace game
