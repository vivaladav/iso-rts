#pragma once

#include "BaseGameState.h"

namespace game
{

class StatePlanetMap : public BaseGameState
{
public:
    StatePlanetMap(Game * game);

private:
    void OnActive() override;
    void OnInactive() override;
};

} // namespace game
