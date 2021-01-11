#include "Player.h"

#include "Cell2D.h"
#include "GameConstants.h"
#include "GameMapCell.h"
#include "AI/PlayerAI.h"
#include "GameObjects/ResourceGenerator.h"

#include <cassert>

namespace game
{

Player::Player(const char * name, int pid)
    : mName(name)
    , mOnNumCellsChanged([](int){})
    , mOnEnergyChanged([](int){})
    , mOnMaterialChanged([](int){})
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

int Player::GetEnergyUse() const
{
    const int energyCells = mNumCells * ENERGY_PER_CELL;
    const int energyUnits = mTotUnitsLevel * ENERGY_PER_UNIT;

    return energyCells + energyUnits;
}

void Player::SumEnergy(int val)
{
    mEnergy += val;

    mOnEnergyChanged(mEnergy);
}


void Player::SumMaterial(int val)
{
    mMaterial1 += val;

    mOnMaterialChanged(mMaterial1);
}

void Player::UpdateResources()
{
    // energy
    const int energyProd = GetResourceProduction(ResourceType::ENERGY);
    const int energyUsed = GetEnergyUse();
    const int energyDiff = energyProd - energyUsed;

    if(energyDiff != 0)
        SumEnergy(energyDiff);

    // material 1
    const int materialProd = GetResourceProduction(ResourceType::MATERIAL1);

    if(materialProd != 0)
        SumMaterial(materialProd);
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

int Player::GetResourceProduction(ResourceType type) const
{
    int energy = 0;

    for(auto it : mResGenerators)
    {
        const ResourceGenerator * resGen = it.second;

        assert(resGen->GetCell() != nullptr);

        if(resGen->GetResourceType() == type &&
           resGen->GetCell()->linked)
            energy += resGen->GetOutput();
    }

    return energy;
}

} // namespace game
