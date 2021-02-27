#include "Player.h"

#include "Cell2D.h"
#include "GameMapCell.h"
#include "AI/PlayerAI.h"
#include "GameObjects/Diamonds.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/ResourceGenerator.h"
#include "GameObjects/Unit.h"

#include <cassert>
#include <iostream>

namespace game
{

// NOTE these will be replaced by dynamic values soon
constexpr int ENERGY_PER_CELL = 1;
constexpr int ENERGY_PER_UNIT = 1;

Player::Player(const char * name, int pid)
    : mName(name)
    , mOnNumCellsChanged([](int){})
    , mOnEnergyChanged([](int){})
    , mOnMaterialChanged([](int){})
    , mOnDiamondsChanged([](int){})
    , mOnNumUnitsChanged(([](int){}))
    , mPlayerId(pid)
{
}

Player::~Player()
{
    delete mAI;
}

void Player::InitVisibility(int rows, int cols)
{
    const unsigned int size = rows * cols;

    mVisMap.resize(size);
    mVisMap.assign(size, 0);
}

void Player::AddVisibilityToAll()
{
    for(unsigned int i = 0; i < mVisMap.size(); ++i)
        ++mVisMap[i];
}

void Player::RemVisibilityToAll()
{
    for(unsigned int i = 0; i < mVisMap.size(); ++i)
    {
        if(mVisMap[i] > 0)
            --mVisMap[i];
    }
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

void Player::HandleCollectable(GameObject * obj)
{
    // DIAMONDS
    if(obj->GetObjectType() == OBJ_DIAMONDS)
    {
        auto d = static_cast<Diamonds *>(obj);

        const int diamondsMult = 5;
        SumDiamonds(d->GetNum() * diamondsMult);
    }
}

void Player::SumUnits(int val)
{
    mNumUnits += val;

    mOnNumUnitsChanged(mNumUnits);
}

void Player::ClearSelectedObject()
{
    if(nullptr == mSelObj)
        return ;

    mSelObj->SetSelected(false);

    if(mSelObj->GetObjectType() == OBJ_UNIT)
        static_cast<Unit *>(mSelObj)->SetActiveAction(IDLE);

    mSelObj = nullptr;
}

void Player::SetSelectedObject(GameObject * obj)
{
    if(mSelObj != nullptr)
        mSelObj->SetSelected(false);

    mSelObj = obj;

    if(mSelObj->GetObjectType() == OBJ_UNIT)
        static_cast<Unit *>(mSelObj)->SetActiveAction(MOVE);

    mSelObj->SetSelected(true);
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
