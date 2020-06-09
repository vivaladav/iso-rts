#pragma once

#include "BaseGameState.h"

namespace game
{

class StateGame : public BaseGameState
{
public:
    StateGame(Game * game);

private:
    void OnActive() override;
    void OnInactive() override;
};

} // namespace game
