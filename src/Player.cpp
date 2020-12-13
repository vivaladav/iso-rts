#include "Player.h"

#include "Cell2D.h"
#include "AI/PlayerAI.h"

namespace game
{

Player::Player(const char * name, int pid)
    : mName(name)
    , mOnNumCellsChanged([](int){})
    , mOnMoneyChanged([](int){})
    , mOnNumUnitsChanged(([](int){}))
    , mSelectedCell({-1, -1})
    , mPlayerId(pid)
{
}

Player::~Player()
{
    delete mAI;
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
    mSelectedCell = cell;
}

} // namespace game
