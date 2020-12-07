#include "GameObject.h"

namespace game
{

GameObject::~GameObject() { }

GameObject::GameObject(GameObjectType type, int owner)
    : mOwner(owner)
    , mType(type)
{

}

void GameObject::SetOwner(int owner)
{
    mOwner = owner;

    UpdateImageId();
}


} // namespace game
