#include "Player.h"

#include "Cell2D.h"
#include "GameConstants.h"
#include "GameMapCell.h"
#include "ResourceGenerator.h"
#include "AI/PlayerAI.h"

#include <cassert>

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

void Player::AddResourceGenerator(unsigned int cellId, ResourceGenerator * gen)
{
    mResGenerators.insert({cellId, gen});
}

void Player::RemoveResourceGenerator(unsigned int cellId)
{
    auto it = mResGenerators.find(cellId);

    if(it != mResGenerators.end())
        mResGenerators.erase(it);
}

int Player::GetEnergyProduction() const
{
    int energy = 0;

    for(auto it : mResGenerators)
    {
        const ResourceGenerator * resGen = it.second;

        assert(resGen->GetCell() != nullptr);

        if(resGen->GetResourceType() == ResourceType::ENERGY &&
           resGen->GetCell()->linked)
            energy += resGen->GetOutput();
    }

    return energy;
}

int Player::GetEnergyUse() const
{
    const int energyCells = mNumCells * ENERGY_PER_CELL;
    const int energyUnits = mTotUnitsLevel * ENERGY_PER_UNIT;

    return energyCells + energyUnits;
}

} // namespace game
