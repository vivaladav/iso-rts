#include "DiamondsGenerator.h"

#include "GameMap.h"

#include <utilities/UniformDistribution.h>

namespace game
{

DiamondsGenerator::DiamondsGenerator(GameMap * gm)
    : mGameMap(gm)
{
    // time range, in ms
    const int minTime = 15000;
    const int maxTime = 45000;

    lib::utilities::UniformDistribution ran(minTime, maxTime);

    mTimeRegen = ran.GetNextValue();
    mTimerRegen = mTimeRegen;
}

void DiamondsGenerator::Update(float delta)
{
    mTimerRegen -= delta;

    // nothing to do until timer is 0
    if(mTimerRegen > 0.f)
        return ;

    // try to generate some diamonds
    // TODO

    // reset timer
    mTimerRegen = mTimeRegen;
}

} // namespace game
