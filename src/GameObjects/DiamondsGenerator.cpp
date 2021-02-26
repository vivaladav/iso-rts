#include "GameObjects/DiamondsGenerator.h"

#include "GameConstants.h"
#include "GameMap.h"
#include "GameObjects/Diamonds.h"

#include <utilities/UniformDistribution.h>

namespace game
{

DiamondsGenerator::DiamondsGenerator(GameMap * gm)
    : mGameMap(gm)
{
    // time range, in ms
    const int minTime = 5;
    const int maxTime = 30;

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

    // reset timer
    mTimerRegen = mTimeRegen;

    // try to generate some diamonds if cell is empty
    if(mGameMap->HasObject(mRow, mCol))
        return ;

    GameObject * obj = mGameMap->CreateObject(MapLayers::OBJECTS,GameObjectType::OBJ_DIAMONDS,
                                              mRow, mCol, 1, 1);

    if(obj != nullptr)
    {
        lib::utilities::UniformDistribution ran(Diamonds::MIN_UNITS, Diamonds::MAX_UNITS);
        const int num = ran.GetNextValue();

        static_cast<Diamonds *>(obj)->SetNum(num);
    }
}

} // namespace game
