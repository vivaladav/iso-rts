#include "GameObject.h"

#include "GameMapCell.h"
#include "IsoObject.h"

namespace game
{

const unsigned int GameObject::COLOR_FOW = 0x555555FF;
const unsigned int GameObject::COLOR_VIS = 0xFFFFFFFF;

unsigned int GameObject::counter = 0;

GameObject::GameObject(GameObjectType type, int rows, int cols)
    : mIsoObj(new IsoObject(rows, cols))
    , mObjId(++counter)
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

void GameObject::SetLinked(bool val)
{
    if(val == mLinked)
        return ;

    mLinked = val;

    OnLinkedChanged();
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

void GameObject::SetOwner(Player * owner)
{
    // setting same owner again -> nothing to do
    if(owner == mOwner)
        return ;

    mOwner = owner;

    UpdateImage();
}

void GameObject::Update(float) { }

void GameObject::OnLinkedChanged() { }

} // namespace game
