#include "States/StatePlanetMap.h"

#include "Screens/ScreenPlanetMap.h"
#include "States/StatesIds.h"

namespace game
{

StatePlanetMap::StatePlanetMap(Game * game)
    : BaseGameState(StateId::PLANET_MAP, game)
{
}

void StatePlanetMap::OnActive()
{
    mScreen = new ScreenPlanetMap(mGame);
}

void StatePlanetMap::OnInactive()
{
    delete mScreen;
    mScreen = nullptr;
}

} // namespace game
