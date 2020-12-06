#include "Unit.h"

#include "GameConstants.h"
#include "GameObjectEnums.h"

#include "GameConstants.h"

namespace game
{

Unit::Unit(int owner)
    : GameObject(GameObjectType::OBJ_UNIT, owner)
{
    SetImageId();
}

void Unit::AddElement()
{
    if(mElements >= MAX_CELL_UNITS)
        return ;

    ++mElements;
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
