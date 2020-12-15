#include "GameObject.h"

namespace game
{

GameObject::~GameObject() { }

GameObject::GameObject(GameObjectType type, int owner)
    : mOwner(owner)
    , mType(type)
{

}

void GameObject::SetCell(GameMapCell * cell)
{
    mCell = cell;

    UpdateImageId();
}

void GameObject::SetOwner(int owner)
{
    mOwner = owner;

    UpdateImageId();
}


} // namespace game
