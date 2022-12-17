#include "Player.h"

#include "GameConstants.h"
#include "GameMapCell.h"
#include "AI/PlayerAI.h"
#include "GameObjects/Blobs.h"
#include "GameObjects/Diamonds.h"
#include "GameObjects/ObjectData.h"
#include "GameObjects/ResourceGenerator.h"
#include "GameObjects/Unit.h"

#include <cassert>
#include <iostream>

namespace game
{

// NOTE these will be replaced by dynamic values soon
constexpr int ENERGY_PER_CELL = 1;

Player::Player(const char * name, int pid)
    : mDummyStat(INVALID_STAT, 0)
    , mName(name)
    , mOnNumCellsChanged([](int){})
    , mOnNumUnitsChanged([](){})
    , mOnResourcesChanged([](){})
    , mPlayerId(pid)
{
    mStats.emplace_back(Stat::BLOBS, 0);
    mStats.emplace_back(Stat::DIAMONDS, 0);
    mStats.emplace_back(Stat::ENERGY, 0);
    mStats.emplace_back(Stat::MATERIAL, 0);
    mStats.emplace_back(Stat::MONEY, 0);

    for(StatValue & val : mStats)
        val.SetMin(0);

    mStats[Stat::BLOBS].SetMax(100);
    mStats[Stat::DIAMONDS].SetMax(100);
    mStats[Stat::ENERGY].SetMax(1000);
    mStats[Stat::MATERIAL].SetMax(500);
    mStats[Stat::MONEY].SetMax(99999999);

    // init vectors of resource generators
    mResGenerators.insert({ RES_ENERGY, {} });
    mResGenerators.insert({ RES_MATERIAL1, {} });
    mResGenerators.insert({ RES_DIAMONDS, {} });
    mResGenerators.insert({ RES_BLOBS, {} });
}

Player::~Player()
{
    delete mAI;
}

void Player::AddUnit(Unit * unit)
{
    if(mUnits.size() == mMaxUnits)
        return ;

    mUnits.push_back(unit);

    mOnNumUnitsChanged();
}

void Player::RemoveUnit(Unit * unit)
{
    auto it = mUnits.begin();

    while(it != mUnits.end())
    {
        if(*it == unit)
        {
            mUnits.erase(it);

            mOnNumUnitsChanged();

            return ;
        }
        else
            ++it;
    }
}

Unit * Player::GetUnit(unsigned int index)
{
    if(index < mUnits.size())
        return mUnits[index];
    else
        return nullptr;
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

void Player::SumResource(Stat sid, int val)
{
    if(sid >= NUM_PSTATS)
        return ;

    mStats[sid].SumValue(val);

    mOnResourcesChanged();
}

void Player::SumResourceMax(Stat sid, int val)
{
    if(sid >= NUM_PSTATS)
        return ;

    const int max = mStats[sid].GetIntMax();
    int newMax = max + val;

    if(newMax < 0)
        newMax = 0;

    mStats[sid].SetMax(newMax);
}

unsigned int Player::AddOnResourceChanged(Stat sid, const std::function<void(const StatValue *)> & f)
{
    if(sid >= NUM_PSTATS)
        return 0;

    return mStats[sid].AddOnValueChanged(f);
}

unsigned int Player::AddOnResourceRangeChanged(Stat sid, const std::function<void(const StatValue *)> & f)
{
    if(sid >= NUM_PSTATS)
        return 0;

    return mStats[sid].AddOnRangeChanged(f);
}

void Player::RemoveOnResourceChanged(Stat sid, unsigned int funId)
{
    if(sid < NUM_PSTATS)
        mStats[sid].RemoveOnValueChanged(funId);
}

void Player::RemoveOnResourceRangeChanged(Stat sid, unsigned int funId)
{
    if(sid < NUM_PSTATS)
        mStats[sid].RemoveOnRangeChanged(funId);
}

void Player::SumCells(int val)
{
    mNumCells += val;

    mOnNumCellsChanged(mNumCells);
}

int Player::GetEnergyUse() const
{
    const int energyCells = mNumCells * ENERGY_PER_CELL;

    return energyCells;
}

void Player::UpdateResources()
{
    // energy
    const int energyProd = GetResourceProduction(ResourceType::RES_ENERGY);
    const int energyUsed = GetEnergyUse();
    const int energyDiff = energyProd - energyUsed;

    bool changed = false;

    if(energyDiff != 0)
    {
        mStats[Player::Stat::ENERGY].SumValue(energyDiff);
        changed = true;
    }

    // material 1
    const int materialProd = GetResourceProduction(ResourceType::RES_MATERIAL1);

    if(materialProd != 0)
    {
        mStats[Player::Stat::MATERIAL].SumValue(materialProd);
        changed = true;
    }

    if(changed)
        mOnResourcesChanged();
}

void Player::HandleCollectable(GameObject * obj)
{
    const GameObjectType type = obj->GetObjectType();

    // DIAMONDS
    if(type == OBJ_DIAMONDS)
    {
        auto d = static_cast<Diamonds *>(obj);

        const int diamondsMult = 10;
        mStats[Stat::DIAMONDS].SumValue(d->GetNum() * diamondsMult);
    }
    else if(type == OBJ_BLOBS)
    {
        auto d = static_cast<Blobs *>(obj);

        const int blobsMult = 5;
        mStats[Stat::BLOBS].SumValue(d->GetNum() * blobsMult);
    }

    mOnResourcesChanged();

    // notify collection
    static_cast<Collectable *>(obj)->Collected();
}

void Player::AddAvailableStructure(const ObjectData & data)
{
    mAvailableStructures.emplace_back(data);
}

const ObjectData & Player::GetAvailableStructure(StructureType type) const
{
    for(const ObjectData & data : mAvailableStructures)
    {
        if(data.objType == type)
            return data;
    }

    return ObjectData::NullObj;
}

void Player::AddAvailableUnit(const ObjectData & data)
{
    mAvailableUnits.emplace_back(data);
}

const ObjectData & Player::GetAvailableUnit(UnitType type) const
{
    for(const ObjectData & data : mAvailableUnits)
    {
        if(data.objType == type)
            return data;
    }

    return ObjectData::NullObj;
}

void Player::ClearSelectedObject()
{
    if(nullptr == mSelObj)
        return ;

    mSelObj->SetSelected(false);

    if(mSelObj->GetObjectType() == OBJ_UNIT)
        static_cast<Unit *>(mSelObj)->SetActiveAction(GameObjectActionId::IDLE);

    mSelObj = nullptr;
}

void Player::SetSelectedObject(GameObject * obj)
{
    if(mSelObj != nullptr)
        mSelObj->SetSelected(false);

    mSelObj = obj;

    // reset active action to move when unit is selected
    if(mSelObj->GetObjectType() == OBJ_UNIT)
        static_cast<Unit *>(mSelObj)->SetActiveAction(GameObjectActionId::MOVE);

    mSelObj->SetSelected(true);
}

void Player::AddResourceGenerator(ResourceGenerator * gen)
{
    const ResourceType type = gen->GetResourceType();

    mResGenerators[type].push_back(gen);
}

void Player::RemoveResourceGenerator(ResourceGenerator * gen)
{
    const ResourceType type = gen->GetResourceType();
    std::vector<ResourceGenerator *> & generators = mResGenerators[type];

    auto it = generators.begin();

    while(it != generators.end())
    {
        if(*it == gen)
        {
            generators.erase(it);
            return ;
        }
        else
            ++it;
    }
}

int Player::GetResourceProduction(ResourceType type) const
{
    int res = 0;

    auto it = mResGenerators.find(type);

    // can't find resource type
    if(mResGenerators.end() == it)
        return 0;

    const std::vector<ResourceGenerator *> & generators = it->second;

    for(const ResourceGenerator * resGen : generators)
    {
        if(resGen->GetCell()->linked)
              res += resGen->GetOutput();
    }

    return res;
}

} // namespace game
