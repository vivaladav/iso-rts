#include "GameObject.h"

#include "GameMapCell.h"
#include "IsoObject.h"

namespace game
{

const unsigned int GameObject::COLOR_FOW = 0x555555FF;
const unsigned int GameObject::COLOR_VIS = 0xFFFFFFFF;

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

void GameObject::SetVisible(bool val)
{
    mVisible = val;

    UpdateImage();
}

void GameObject::SetCell(const GameMapCell * cell)
{
    mCell = cell;

    mIsoObj->SetRow(cell->row);
    mIsoObj->SetCol(cell->col);
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

void GameObject::Update(float) { }

} // namespace game
