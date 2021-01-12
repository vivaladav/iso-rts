#include "GameObjects/Base.h"

#include "GameObjects/GameObjectEnums.h"

namespace game
{

Base::Base(int owner)
    : GameObject(GameObjectType::OBJ_BASE, owner)
{
    SetImageId();
}

void Base::UpdateImageId()
{
    SetImageId();
}

void Base::SetImageId()
{
    int img = IMG_NULL;

    switch(GetOwner())
    {
        case 0:
            img = BASE_F1;
        break;

        case 1:
            img = BASE_F2;
        break;

        case 2:
            img = BASE_F3;
        break;

        default:
        break;
    }

    mImageId = static_cast<GameObjectImageId>(img);
}

} // namespace game
