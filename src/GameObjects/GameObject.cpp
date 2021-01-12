#include "GameObject.h"

#include "GameMapCell.h"

namespace game
{

GameObject::GameObject(GameObjectType type, int owner)
    : mOwner(owner)
    , mType(type)
{
}

GameObject::~GameObject() { }

void GameObject::SetSelected(bool val)
{
    // same value -> nothing to do
    if(val == mSelected)
        return ;

    mSelected = val;

    UpdateImageId();
}

void GameObject::SetCell(GameMapCell * cell)
{
    mCell = cell;

    UpdateImageId();
}

int GameObject::GetRow0() const { return mCell->row; }
int GameObject::GetCol0() const { return mCell->col; }
int GameObject::GetRow1() const { return 1 + mCell->row - mRows; }
int GameObject::GetCol1() const { return 1 + mCell->col - mCols; }

void GameObject::SetOwner(int owner)
{
    mOwner = owner;

    UpdateImageId();
}

} // namespace game
