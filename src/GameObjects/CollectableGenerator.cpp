#include "GameObjects/CollectableGenerator.h"

#include "GameMap.h"

#include <sgl/utilities/UniformDistribution.h>

namespace game
{

CollectableGenerator::CollectableGenerator(GameMap * gm, int timeMin, int timeMax)
    : mGameMap(gm)
    , mTimeMin(timeMin)
    , mTimeMax(timeMax)
{
    ResetTimer();
}

CollectableGenerator::~CollectableGenerator()
{

}

void CollectableGenerator::ResetTimer()
{
    // randomize generation time between min and max
    sgl::utilities::UniformDistribution ran(mTimeMin, mTimeMax);

    mTimeRegen = ran.GetNextValue();
    mTimerRegen = mTimeRegen;
}

void CollectableGenerator::Update(float delta)
{
    // decrease timer
    mTimerRegen -= delta;

    // nothing to do until timer is 0
    if(mTimerRegen > 0.f)
        return ;

    // reset timer
    mTimerRegen = mTimeRegen;

    // generate only if cell is empty
    if(!mGameMap->HasObject(mRow, mCol))
        OnGeneration();
}

} // namespace game
