#include "Player.h"

namespace game
{

Player::Player(const char * name, int pid)
    : mName(name)
    , mOnNumCellsChanged([](int){})
    , mOnMoneyChanged([](int){})
    , mOnNumUnitsChanged(([](int){}))
    , mPlayerId(pid)
{
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

} // namespace game
