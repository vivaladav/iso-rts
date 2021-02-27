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
    const int maxTime = 10;

    lib::utilities::UniformDistribution ran(minTime, maxTime);

    mTimeRegen = ran.GetNextValue();
    mTimerRegen = mTimeRegen;
}

void DiamondsGenerator::Update(float delta)
{
    // try to generate some diamonds if cell is empty
    if(mGameMap->HasObject(mRow, mCol))
        return ;

    // decrease timer if there's no object on cell
    mTimerRegen -= delta;

    // nothing to do until timer is 0
    if(mTimerRegen > 0.f)
        return ;

    // reset timer
    mTimerRegen = mTimeRegen;

    GameObject * obj = mGameMap->CreateObject(MapLayers::OBJECTS,GameObjectType::OBJ_DIAMONDS,
                                              mRow, mCol, 1, 1);

    if(nullptr == obj)
        return ;

    auto diamonds = static_cast<Diamonds *>(obj);

    // set number of diamonds in cell
    lib::utilities::UniformDistribution ran(Diamonds::MIN_UNITS, Diamonds::MAX_UNITS);
    const int num = ran.GetNextValue();
    diamonds->SetNum(num);

    diamonds->SetGenerator(this);

    // update visibility
    mGameMap->ApplyLocalVisibilityToObject(obj);

    mGameMap->SetCellWalkable(mRow, mCol, true);
}

} // namespace game
