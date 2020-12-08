#include "Unit.h"

#include "GameConstants.h"
#include "GameObjectEnums.h"

#include "GameConstants.h"

namespace game
{

Unit::Unit(int owner, unsigned int elements)
    : GameObject(GameObjectType::OBJ_UNIT, owner)
    , mElements(elements)
{
    SetImageId();

    // cap number of elements
    if(mElements > MAX_CELL_UNITS)
        mElements = MAX_CELL_UNITS;
}

void Unit::IncreaseUnitLevel()
{
    if(mLevel >= MAX_UNITS_LEVEL)
        return ;

    ++mLevel;
    SetImageId();
}

void Unit::SumElements(int num)
{
    // cap up to MAX_CELL_UNITS
    if(mElements + num >= MAX_CELL_UNITS)
        num = MAX_CELL_UNITS + mElements;
    // cap down to 1
    else if(mElements + num < 1)
        num = 1 - mElements;

    // do something only if num is not 0
    if(0 == num)
        return ;

    mElements += num;

    SetImageId();
}

void Unit::UpdateImageId()
{
    SetImageId();
}

void Unit::SetImageId()
{
    int img = (mElements - 1) + (mLevel * MAX_CELL_UNITS);

    switch(GetOwner())
    {
        case 0:
            img += P1_1UL1;
        break;

        case 1:
            img += P2_1UL1;
        break;

        case 2:
            img += P3_1UL1;
        break;

        case 3:
            img += P4_1UL1;
        break;

        default:
            img = IMG_NULL;
        break;
    }

    mImageId = static_cast<GameObjectImageId>(img);
}

} // namespace game
