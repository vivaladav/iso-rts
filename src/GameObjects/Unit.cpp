#include "GameObjects/Unit.h"

#include "GameConstants.h"
#include "GameObjects/GameObjectEnums.h"

namespace game
{

Unit::Unit(int owner)
    : GameObject(GameObjectType::OBJ_UNIT, owner)
{
    SetImageId();
}

void Unit::IncreaseUnitLevel()
{
    if(mLevel >= MAX_UNITS_LEVEL)
        return ;

    ++mLevel;
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
        break;
    }

    mImageId = static_cast<GameObjectImageId>(img);
}

} // namespace game
