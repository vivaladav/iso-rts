#pragma once

#include "BaseGameState.h"

namespace game
{

class StateFactionSelection : public BaseGameState
{
public:
    StateFactionSelection(Game * game);

private:
    void OnActive() override;
    void OnInactive() override;
};

} // namespace game
