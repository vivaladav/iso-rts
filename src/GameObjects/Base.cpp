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
            img = IsSelected() ? BASE_F1_SEL :BASE_F1;
        break;

        case 1:
            img = IsSelected() ? BASE_F2_SEL :BASE_F2;
        break;

        case 2:
            img = IsSelected() ? BASE_F3_SEL :BASE_F3;
        break;

        default:
        break;
    }

    mImageId = static_cast<GameObjectImageId>(img);
}

} // namespace game
