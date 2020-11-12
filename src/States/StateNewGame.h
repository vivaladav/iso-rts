#pragma once

#include "BaseGameState.h"

namespace game
{

class StateNewGame : public BaseGameState
{
public:
    StateNewGame(Game * game);

private:
    void OnActive() override;
    void OnInactive() override;
};

} // namespace game
