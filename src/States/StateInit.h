#pragma once

#include "BaseGameState.h"

namespace game
{

class StateInit : public BaseGameState
{
public:
    StateInit(Game * game);

private:
    void OnActive() override;
    void OnInactive() override;
};

} // namespace game
