#include "Player.h"

#include "Cell2D.h"

namespace game
{

Player::Player(const char * name, int pid)
    : mName(name)
    , mOnNumCellsChanged([](int){})
    , mOnMoneyChanged([](int){})
    , mOnNumUnitsChanged(([](int){}))
    , mSelectedCell(new Cell2D(-1, -1))
    , mPlayerId(pid)
{
}

Player::~Player()
{
    delete mSelectedCell;
}

void Player::SumCells(int val)
{
    mNumCells += val;

    mOnNumCellsChanged(mNumCells);
}

void Player::SumMoney(int val)
{
    mMoney += val;

    mOnMoneyChanged(mMoney);
}

void Player::SumUnits(int val)
{
    mNumUnits += val;

    mOnNumUnitsChanged(mNumUnits);
}

void Player::SetSelectedCell(const Cell2D & cell)
{
    *mSelectedCell = cell;

    mCurrSelectedCell = mSelectedCell;
}

} // namespace game
