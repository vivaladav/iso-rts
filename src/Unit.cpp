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
    int img = IMG_NULL;

    switch(GetOwner())
    {
        case 0:
            img = F1_U1;
        break;

        case 1:
            img = F2_U1;
        break;

        case 2:
            img = F3_U1;
        break;


        default:
            img = IMG_NULL;
        break;
    }

    mImageId = static_cast<GameObjectImageId>(img);
}

} // namespace game
