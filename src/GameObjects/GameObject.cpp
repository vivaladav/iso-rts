#include "GameObject.h"

#include "GameMapCell.h"
#include "IsoObject.h"

namespace game
{

GameObject::GameObject(GameObjectType type, int owner, int rows, int cols)
    : mIsoObj(new IsoObject(rows, cols))
    , mOwner(owner)
    , mType(type)
{
}

GameObject::~GameObject() { delete mIsoObj; }

void GameObject::SetSelected(bool val)
{
    // same value -> nothing to do
    if(val == mSelected)
        return ;

    mSelected = val;

    UpdateImage();
}

void GameObject::SetCell(GameMapCell * cell)
{
    mCell = cell;

    mIsoObj->SetRow(cell->row);
    mIsoObj->SetCol(cell->col);

    UpdateImage();
}

int GameObject::GetRow0() const { return mCell->row; }
int GameObject::GetCol0() const { return mCell->col; }
int GameObject::GetRow1() const { return 1 + mCell->row - mRows; }
int GameObject::GetCol1() const { return 1 + mCell->col - mCols; }

void GameObject::SetOwner(int owner)
{
    mOwner = owner;

    UpdateImage();
}

} // namespace game
