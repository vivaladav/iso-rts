#include "Player.h"

#ifdef DEV_MODE
#include "Game.h"
#endif

#include "GameConstants.h"
#include "GameMapCell.h"
#include "AI/PlayerAI.h"
#include "GameObjects/Blobs.h"
#include "GameObjects/Diamonds.h"
#include "GameObjects/GameObjectsGroup.h"
#include "GameObjects/ResourceStorage.h"
#include "GameObjects/Structure.h"
#include "GameObjects/Unit.h"

#include <sgl/utilities/BinaryFile.h>

#include <algorithm>
#include <iostream>

#include <cassert>
#include <cmath>

namespace
{
    using namespace game;

    const int RESOURCES_MAX[Player::NUM_PSTATS] =
    {
        200,
        200,
        2250,
        1800,
        99999999,
        999999,
    };
}

namespace game
{

Player::Player()
    : Player(-1)
{
}

Player::Player(int pid)
    : mDummyStat(INVALID_STAT, 0)
    , mOnNumUnitsChanged([](){})
    , mPlayerId(pid)
    , mFaction(NO_FACTION)
{
    InitResources();

    InitUpgrades();
}

Player::~Player()
{
    delete mAI;
}

bool Player::Load(sgl::utilities::BinaryFile & bf)
{
    // player ID
    mPlayerId = bf.ReadInt();
    // faction
    mFaction = static_cast<PlayerFaction>(bf.ReadUint());
    // name
    bf.ReadString(mName);

    // upgrades status - read only unlocked ones
    const unsigned int numUnlocked = bf.ReadUint();

    for(unsigned int i = 0; i < numUnlocked; ++i)
    {
        const auto upgrade = static_cast<TechUpgradeId>(bf.ReadUint());
        mUpgrades[upgrade] = true;
    }

    // upgrades available - read only available ones
    const unsigned int numAvailable = bf.ReadUint();

    for(unsigned int i = 0; i < numAvailable; ++i)
    {
        const auto upgrade = static_cast<TechUpgradeId>(bf.ReadUint());
        mUpgradesAvailable[upgrade] = true;
    }

    // global resources
    for(unsigned int i = 0; i < NUM_PSTATS; ++i)
    {
        const unsigned int statId = bf.ReadUint();
        const int val = bf.ReadInt();
        const int min = bf.ReadInt();
        const int max = bf.ReadInt();

        mGlobalResources[statId].SetMin(min);
        mGlobalResources[statId].SetMax(max);
        mGlobalResources[statId].SetValue(val);
    }

    // local resources
    for(unsigned int i = 0; i < NUM_PSTATS; ++i)
    {
        const unsigned int statId = bf.ReadUint();
        const int val = bf.ReadInt();
        const int min = bf.ReadInt();
        const int max = bf.ReadInt();

        mLocalResources[statId].SetMin(min);
        mLocalResources[statId].SetMax(max);
        mLocalResources[statId].SetValue(val);
    }

    // global/local resources flag
    mUseGlobalRes = bf.ReadBool();

    // available mini-units
    const unsigned int numAvMiniUnits = bf.ReadUint();
    mAvailableMiniUnits.reserve(numAvMiniUnits);

    for(unsigned int i = 0; i < numAvMiniUnits; ++i)
        mAvailableMiniUnits.emplace_back(bf.ReadSizeT());

    // available structures
    const unsigned int numAvStructures = bf.ReadUint();
    mAvailableStructures.reserve(numAvStructures);

    for(unsigned int i = 0; i < numAvStructures; ++i)
        mAvailableStructures.emplace_back(bf.ReadSizeT());

    // available units
    const unsigned int numAvUnits = bf.ReadUint();
    mAvailableUnits.reserve(numAvUnits);

    for(unsigned int i = 0; i < numAvUnits; ++i)
        mAvailableUnits.emplace_back(bf.ReadSizeT());

    // stats
    mTurnsPlayed = bf.ReadUint();
    mNumCells = bf.ReadUint();
    mNumLinkedCells = bf.ReadUint();
    mMaxUnits = bf.ReadUint();
    mEnemiesKilled = bf.ReadUint();
    mCasualties = bf.ReadUint();

    // upgrade multiplayers
    mBaseProdMult = bf.ReadFloat();
    mStorageEnergyMult = bf.ReadFloat();
    mStorageMaterialMult = bf.ReadFloat();
    mStorageDiamondsMult = bf.ReadFloat();
    mStorageBlobsMult = bf.ReadFloat();

    return true;
}

bool Player::Save(sgl::utilities::BinaryFile & bf) const
{
    // player ID
    bf.WriteInt(mPlayerId);
    // faction
    bf.WriteUint(mFaction);
    // name
    bf.WriteString(mName);

    // upgrades status - write only unlocked ones
    const unsigned int unlocked = std::count_if(mUpgrades.begin(), mUpgrades.end(),
                                                [](const std::pair<TechUpgradeId, bool> & it)
                                                {
                                                    return it.second == true;
                                                });
    bf.WriteUint(unlocked);

    for(auto it : mUpgrades)
    {
        if(it.second)
            bf.WriteUint(it.first);
    }

    // upgrades available - write only available ones
    const unsigned int available = std::count_if(mUpgradesAvailable.begin(), mUpgradesAvailable.end(),
                                                [](const std::pair<TechUpgradeId, bool> & it)
                                                {
                                                    return it.second == true;
                                                });
    bf.WriteUint(available);

    for(auto it : mUpgradesAvailable)
    {
        if(it.second)
            bf.WriteUint(it.first);
    }

    // global resources
    for(const StatValue & s : mGlobalResources)
    {
        bf.WriteUint(s.GetId());
        bf.WriteInt(s.GetValue());
        bf.WriteInt(s.GetMin());
        bf.WriteInt(s.GetMax());
    }

    // local resources
    for(const StatValue & s : mLocalResources)
    {
        bf.WriteUint(s.GetId());
        bf.WriteInt(s.GetValue());
        bf.WriteInt(s.GetMin());
        bf.WriteInt(s.GetMax());
    }

    // global/local resources flag
    bf.WriteBool(mUseGlobalRes);

    // available mini-units
    const unsigned int numAvMiniUnits = mAvailableMiniUnits.size();
    bf.WriteUint(numAvMiniUnits);

    for(const GameObjectTypeId type : mAvailableMiniUnits)
        bf.WriteSizeT(type);

    // available structures
    const unsigned int numAvStructures = mAvailableStructures.size();
    bf.WriteUint(numAvStructures);

    for(const GameObjectTypeId type : mAvailableStructures)
        bf.WriteSizeT(type);

    // available units
    const unsigned int numAvUnits = mAvailableUnits.size();
    bf.WriteUint(numAvUnits);

    for(const GameObjectTypeId type : mAvailableUnits)
        bf.WriteSizeT(type);

    // stats
    bf.WriteUint(mTurnsPlayed);
    bf.WriteUint(mNumCells);
    bf.WriteUint(mNumLinkedCells);
    bf.WriteUint(mMaxUnits);
    bf.WriteUint(mEnemiesKilled);
    bf.WriteUint(mCasualties);

    // upgrade multiplayers
    bf.WriteFloat(mBaseProdMult);
    bf.WriteFloat(mStorageEnergyMult);
    bf.WriteFloat(mStorageMaterialMult);
    bf.WriteFloat(mStorageDiamondsMult);
    bf.WriteFloat(mStorageBlobsMult);

    return true;
}

unsigned int Player::GetNumUnitsByType(GameObjectTypeId type) const
{
    unsigned int num = 0;

    for(Unit * u : mUnits)
    {
        if(u->GetObjectType() == type)
            ++num;
    }

    return num;
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

Unit * Player::GetUnit(unsigned int index) const
{
    if(index < mUnits.size())
        return mUnits[index];
    else
        return nullptr;
}

std::vector<Unit *> Player::GetUnitsByType(GameObjectTypeId type) const
{
    std::vector<Unit *> units;

    for(auto u : mUnits)
    {
        if(u->GetObjectType() == type)
            units.emplace_back(u);
    }

    return units;
}

unsigned int Player::GetNumStructuresByType(GameObjectTypeId type) const
{
    unsigned int num = 0;

    for(Structure * s : mStructures)
    {
        if(s->GetObjectType() == type)
            ++num;
    }

    return num;
}

void Player::AddStructure(Structure * s)
{
    mStructures.push_back(s);
}

void Player::RemoveStructure(Structure * s)
{
    auto it = mStructures.begin();

    while(it != mStructures.end())
    {
        if(*it == s)
        {
            mStructures.erase(it);

            return ;
        }
        else
            ++it;
    }
}

Structure * Player::GetStructure(unsigned int index)
{
    if(index < mStructures.size())
        return mStructures[index];
    else
        return nullptr;
}

bool Player::HasStructure(GameObjectTypeId type) const
{
    for(Structure * s : mStructures)
    {
        if(s->GetObjectType() == type)
            return true;
    }

    return false;
}

std::vector<Structure *> Player::GetStructuresByType(GameObjectTypeId type) const
{
    std::vector<Structure *> structures;

    for(Structure * s : mStructures)
    {
        if(s->GetObjectType() == type)
            structures.push_back(s);
    }

    return structures;
}

void Player::InitMaps(int rows, int cols)
{
    const unsigned int size = rows * cols;

    // visibility map
    mVisMap.resize(size);
    mVisMap.assign(size, 0);

    // walkable map
    mWalkableOverrideMap.resize(size, false);

    // size of map
    mMapRows = rows;
    mMapCols = cols;
}

bool Player::IsObjectVisible(const GameObject * obj) const
{
    const unsigned int tlR = obj->GetRow1();
    const unsigned int tlC = obj->GetCol1();
    const unsigned int brR = obj->GetRow0();
    const unsigned int brC = obj->GetCol0();

    for(unsigned int r = tlR; r <= brR; ++r)
    {
        const unsigned int ind0 = r * mMapCols;

        for(unsigned int c = tlC; c <= brC; ++c)
        {
            const unsigned int ind = ind0 + c;

            if(mVisMap[ind])
                return true;
        }
    }

    return false;
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

void Player::SetResource(Stat sid, int val)
{
    if(sid >= NUM_PSTATS)
        return ;

    GetCurrentResource(sid).SetValue(val);

    NotifyResourcesChanged();
}

void Player::SumResource(Stat sid, int val)
{
    if(sid >= NUM_PSTATS || 0 == val)
        return ;

    GetCurrentResource(sid).SumValue(val);

    NotifyResourcesChanged();
}

void Player::SetResourceMax(Stat sid, int max)
{
    if(sid >= NUM_PSTATS)
        return ;

    const int oldVal = mLocalResources[sid].GetValue();

    GetCurrentResource(sid).SetMax(max);

    const int newVal = mLocalResources[sid].GetValue();

    if(oldVal != newVal)
        NotifyResourcesChanged();
}

void Player::SumResourceMax(Stat sid, int sum)
{
    if(sid >= NUM_PSTATS)
        return ;

    const int oldVal = mLocalResources[sid].GetValue();
    const int max = mLocalResources[sid].GetMax();
    int newMax = max + sum;

    if(newMax < 0)
        newMax = 0;

    GetCurrentResource(sid).SetMax(newMax);

    const int newVal = mLocalResources[sid].GetValue();

    if(oldVal != newVal)
        NotifyResourcesChanged();
}

unsigned int Player::AddOnResourcesChanged(const std::function<void()> & f)
{
    static unsigned int num = 0;

    mOnResourcesChanged.emplace(++num, f);

    return num;
}

void Player::RemoveOnResourcesChanged(unsigned int funId)
{
    auto it = mOnResourcesChanged.find(funId);

    if(it != mOnResourcesChanged.end())
        mOnResourcesChanged.erase(it);
}

unsigned int Player::AddOnResourceChanged(Stat sid, const std::function<void (const StatValue *,
                                                                              int, int)> & f)
{
    if(sid >= NUM_PSTATS)
        return 0;

    // NOTE callback is set to both types of resources, so it doesn't matter what happens in
    //      ScreenGame and there's no need to handle multiple cases (init vs loading)
    const unsigned int ret1 = mGlobalResources[sid].AddOnValueChanged(f);
    const unsigned int ret2 = mLocalResources[sid].AddOnValueChanged(f);

    assert(ret2 == (ret1 + 1));

    return ret1;
}

unsigned int Player::AddOnResourceRangeChanged(Stat sid, const std::function<void(const StatValue *)> & f)
{
    if(sid >= NUM_PSTATS)
        return 0;

    // NOTE callback is set to both types of resources, so it doesn't matter what happens in
    //      ScreenGame and there's no need to handle multiple cases (init vs loading)
    const unsigned int ret1 = mGlobalResources[sid].AddOnRangeChanged(f);
    const unsigned int ret2 = mLocalResources[sid].AddOnRangeChanged(f);

    assert(ret2 == (ret1 + 1));

    return ret1;
}

void Player::RemoveOnResourceChanged(Stat sid, unsigned int funId)
{
    if(sid < NUM_PSTATS)
    {
        mGlobalResources[sid].RemoveOnValueChanged(funId);
        mLocalResources[sid].RemoveOnValueChanged(funId + 1);
    }
}

void Player::RemoveOnResourceRangeChanged(Stat sid, unsigned int funId)
{
    if(sid < NUM_PSTATS)
    {
        mGlobalResources[sid].RemoveOnRangeChanged(funId);
        mLocalResources[sid].RemoveOnRangeChanged(funId + 1);
    }
}

void Player::ResetMissionData()
{
    mNumCells = 0;
    mEnemiesKilled = 0;
    mCasualties = 0;
}

void Player::HandleCollectable(GameObject * collected, GameObject * collector)
{
    const GameObjectTypeId type = collected->GetObjectType();

    // DIAMONDS
    if(type == ObjectData::TYPE_DIAMONDS)
    {
        auto d = static_cast<Diamonds *>(collected);
        mLocalResources[Stat::DIAMONDS].SumValue(d->GetNumUnits());
    }
    else if(type == ObjectData::TYPE_BLOBS)
    {
        auto d = static_cast<Blobs *>(collected);
        mLocalResources[Stat::BLOBS].SumValue(d->GetNumUnits());
    }
    else
    {
        std::cerr << "Player::HandleCollectable | don't know how to handle this object type: "
                  << type << std::endl;
        return ;
    }

    NotifyResourcesChanged();

    // notify collection
    static_cast<Collectable *>(collected)->Collected(this);
}

void Player::ClearUpgrades()
{
    for(auto it : mUpgrades)
        it.second = false;

    for(auto it : mUpgradesAvailable)
        it.second = false;

    InitUpgrades();
}

void Player::SetUpgradeAvailable(TechUpgradeId upgrade)
{
    auto it = mUpgradesAvailable.find(upgrade);

    if(it != mUpgradesAvailable.end())
        it->second = true;
}

void Player::UnlockUpgrade(TechUpgradeId upgrade)
{
    auto it = mUpgrades.find(upgrade);

    if(it == mUpgrades.end())
        return ;

    it->second = true;

    switch(upgrade)
    {
        case TECH_UP_BASE_IMPROVE_1:
            mBaseProdMult *= 1.05f;
        break;

        case TECH_UP_BASE_IMPROVE_2:
            mBaseProdMult *= 1.10f;
        break;

        case TECH_UP_BASE_IMPROVE_3:
            mBaseProdMult *= 1.15f;
        break;

        case TECH_UP_BASE_IMPROVE_4:
            mBaseProdMult *= 1.20f;
        break;

        case TECH_UP_BASE_IMPROVE_5:
            mBaseProdMult *= 1.25f;
        break;

        case TECH_UP_RADAR_STATION:
            AddAvailableStructure(ObjectData::TYPE_RADAR_STATION);
        break;

        case TECH_UP_RADAR_TOWER:
            AddAvailableStructure(ObjectData::TYPE_RADAR_TOWER);
        break;

        case TECH_UP_STORAGE_STRUCTS:
            AddAvailableStructure(ObjectData::TYPE_RES_STORAGE_BLOBS);
            AddAvailableStructure(ObjectData::TYPE_RES_STORAGE_DIAMONDS);
            AddAvailableStructure(ObjectData::TYPE_RES_STORAGE_ENERGY);
            AddAvailableStructure(ObjectData::TYPE_RES_STORAGE_MATERIAL);
        break;

        case TECH_UP_STORAGE_ENERGY_1:
            UpgradeResourceStorage(ER_ENERGY, 1.25f);
        break;

        case TECH_UP_STORAGE_ENERGY_2:
            UpgradeResourceStorage(ER_ENERGY, 1.5f);
        break;

        case TECH_UP_STORAGE_MATERIAL_1:
            UpgradeResourceStorage(ER_MATERIAL, 1.25f);
        break;

        case TECH_UP_STORAGE_MATERIAL_2:
            UpgradeResourceStorage(ER_MATERIAL, 1.5f);
        break;

        case TECH_UP_STORAGE_DIAMONDS_1:
            UpgradeResourceStorage(ER_DIAMONDS, 1.25f);
        break;

        case TECH_UP_STORAGE_DIAMONDS_2:
            UpgradeResourceStorage(ER_DIAMONDS, 1.5f);
        break;

        case TECH_UP_STORAGE_BLOBS_1:
            UpgradeResourceStorage(ER_BLOBS, 1.25f);
            break;

        case TECH_UP_STORAGE_BLOBS_2:
            UpgradeResourceStorage(ER_BLOBS, 1.5f);
        break;

        case TECH_UP_PRACTICE_TARGET:
            AddAvailableStructure(ObjectData::TYPE_PRACTICE_TARGET);
        break;

        case TECH_UP_TRADING_POST:
            AddAvailableStructure(ObjectData::TYPE_TRADING_POST);
        break;


        case TECH_UP_UNIT_SLOTS_1:
        case TECH_UP_UNIT_SLOTS_2:
        case TECH_UP_UNIT_SLOTS_3:
        case TECH_UP_UNIT_SLOTS_4:
        case TECH_UP_UNIT_SLOTS_5:
            SetMaxUnits(GetMaxUnits() + 1);
        break;

        default:
        break;
    }

    NotifyUpgradeUnlock(upgrade);
}

unsigned int Player::AddOnUpgradeUnlocked(const std::function<void(TechUpgradeId)> & f)
{
    static unsigned int num = 0;

    mOnUpgradeUnlocked.emplace(++num, f);

    return num;
}

void Player::RemoveOnUpgradeUnlocked(unsigned int funId)
{
    auto it = mOnUpgradeUnlocked.find(funId);

    if(it != mOnUpgradeUnlocked.end())
        mOnUpgradeUnlocked.erase(it);
}

void Player::OnNewTurn()
{
    // update turns counter
    ++mTurnsPlayed;

    // UPDATE RESOURCES
    const Player::Stat statIds[] =
    {
        ENERGY,
        MATERIAL,
        DIAMONDS,
        BLOBS,
        MONEY,
        RESEARCH
    };

    for(unsigned int r = 0; r < NUM_EXT_RESOURCES; ++r)
    {
        const auto er = static_cast<ExtendedResource>(r);
        const int delta = GetResourceDelta(er);

        mLocalResources[statIds[r]].SumValue(delta);
    }

    NotifyResourcesChanged();
}

void Player::AddAvailableStructure(GameObjectTypeId type)
{
    mAvailableStructures.emplace_back(type);
}

bool Player::IsStructureAvailable(GameObjectTypeId type) const
{
    for(const GameObjectTypeId t : mAvailableStructures)
    {
        if(t == type)
            return true;
    }

    return false;
}

void Player::AddAvailableUnit(GameObjectTypeId type)
{
    mAvailableUnits.emplace_back(type);
}

bool Player::IsUnitAvailable(GameObjectTypeId type) const
{
    for(const GameObjectTypeId t : mAvailableUnits)
    {
        if(t == type)
            return true;
    }

    return false;
}

void Player::AddAvailableMiniUnit(GameObjectTypeId type)
{
    mAvailableMiniUnits.emplace_back(type);
}

bool Player::IsMiniUnitAvailable(GameObjectTypeId type) const
{
    for(const GameObjectTypeId t : mAvailableMiniUnits)
    {
        if(t == type)
            return true;
    }

    return false;
}

void Player::ClearSelectedObject()
{
    if(nullptr == mSelObj)
        return ;

    auto og = mSelObj->GetGroup();

    // in case object is part of a group -> deselect all members
    if(og != nullptr)
        og->SetSelected(false, nullptr);
    // standard single object -> deselect
    else
        mSelObj->SetSelected(false, nullptr);

    mSelObj = nullptr;
}

void Player::SetSelectedObject(GameObject * obj)
{
    if(mSelObj != nullptr)
        mSelObj->SetSelected(false, nullptr);

    mSelObj = obj;
}

int Player::GetResourceProduction(ExtendedResource type) const
{
    int tot = 0;

    for(const auto s : mStructures)
    {
        if(s->IsLinked())
            tot += s->GetResourceProduction(type);
    }

    return tot;
}

int Player::GetResourceConsumption(ExtendedResource type) const
{
    int tot = 0;

    // consider usage from structures
    for(const auto s : mStructures)
    {
        if(s->IsLinked())
            tot += s->GetResourceUsage(type);
    }

    // energy used by cells too
    if(ER_ENERGY == type)
        tot += GetCellsEnergyUsed();

    return tot;
}

int Player::GetResourceDelta(ExtendedResource type) const
{
    int delta = 0;

    // consider usage and production  from structures
    for(const auto s : mStructures)
    {
        if(s->IsLinked())
            delta += s->GetResourceProduction(type) - s->GetResourceUsage(type);
    }

    // energy used by cells too
    if(ER_ENERGY == type)
        delta -= GetCellsEnergyUsed();

    return delta;
}

void Player::OnNewMission()
{
    ResetMissionData();

    // reset turns played, local always starts from 1
    mTurnsPlayed = IsLocal() ? 1 : 0;

    // set local resources min & max
    // NOTE this needs to be done before switching to local for capping
    for(unsigned int i = 0; i < NUM_PSTATS; ++i)
    {
        mLocalResources[i].SetMin(0);
        mLocalResources[i].SetMax(RESOURCES_MAX[i]);
    }

    SwitchFromGlobalToLocalResources();
}

void Player::OnMissionOver()
{
    mUnits.clear();
    mStructures.clear();

    mSelObj = nullptr;

    SwitchFromLocalToGlobalResources();
}

void Player::InitResources()
{
    // LOCAL RESOURCES
    mLocalResources.emplace_back(Stat::BLOBS, 0);
    mLocalResources.emplace_back(Stat::DIAMONDS, 0);
    mLocalResources.emplace_back(Stat::ENERGY, 0);
    mLocalResources.emplace_back(Stat::MATERIAL, 0);
    mLocalResources.emplace_back(Stat::MONEY, 0);
    mLocalResources.emplace_back(Stat::RESEARCH, 0);

    // set local resources min & max
    for(unsigned int i = 0; i < NUM_PSTATS; ++i)
    {
        mLocalResources[i].SetMin(0);
        mLocalResources[i].SetMax(RESOURCES_MAX[i]);
    }

    // GLOBAL RESOURCES
    mGlobalResources.emplace_back(Stat::BLOBS, 0);
    mGlobalResources.emplace_back(Stat::DIAMONDS, 0);
    mGlobalResources.emplace_back(Stat::ENERGY, 0);
    mGlobalResources.emplace_back(Stat::MATERIAL, 0);
    mGlobalResources.emplace_back(Stat::MONEY, 0);
    mGlobalResources.emplace_back(Stat::RESEARCH, 0);

    // set global resources min
    for(StatValue & val : mGlobalResources)
        val.SetMin(0);
}

void Player::InitUpgrades()
{
    for(unsigned int i = 0; i < NUM_TECH_UPGRADES; ++i)
    {
        const auto tu = static_cast<TechUpgradeId>(i);

        mUpgrades.emplace(tu, false);
        mUpgradesAvailable.emplace(tu, false);
    }

    mUpgrades.emplace(TECH_UP_NULL, false);

    mUpgradesAvailable[TECH_UP_BASE_IMPROVE_1] = true;
    mUpgradesAvailable[TECH_UP_UNIT_SLOTS_1] = true;
}

int Player::GetCellsEnergyUsed() const
{
    const int energyPerCell = 1;
    return mNumLinkedCells * energyPerCell;
}

void Player::NotifyResourcesChanged()
{
    for(auto & it : mOnResourcesChanged)
        it.second();
}

void Player::NotifyUpgradeUnlock(TechUpgradeId upgrade)
{
    for(auto & it : mOnUpgradeUnlocked)
        it.second(upgrade);
}

void Player::UpgradeResourceStorage(ExtendedResource res, float mult)
{
    const GameObjectTypeId objectTypes[] =
    {
        ObjectData::TYPE_RES_STORAGE_ENERGY,
        ObjectData::TYPE_RES_STORAGE_MATERIAL,
        ObjectData::TYPE_RES_STORAGE_DIAMONDS,
        ObjectData::TYPE_RES_STORAGE_BLOBS,
    };

    if(res == ER_ENERGY)
        mStorageEnergyMult *= mult;
    else if(res == ER_MATERIAL)
        mStorageMaterialMult *= mult;
    else if(res == ER_DIAMONDS)
        mStorageDiamondsMult *= mult;
    else if(res == ER_BLOBS)
        mStorageBlobsMult *= mult;

    // notify existing structures
    for(auto s : mStructures)
    {
        if(s->GetObjectType() == objectTypes[res])
            static_cast<ResourceStorage *>(s)->OnCapacityUpgraded();
    }
}

void Player::SwitchFromGlobalToLocalResources()
{
    // copy global resources to local to cap them
    for(unsigned int i = 0; i < NUM_PSTATS; ++i)
        mLocalResources[i].SetValue(mGlobalResources[i].GetValue());

    // subtract local from global
    for(unsigned int i = 0; i < NUM_PSTATS; ++i)
        mGlobalResources[i].SumValue(-mLocalResources[i].GetValue());

    // switch to using local resources
    mUseGlobalRes = false;
}

void Player::SwitchFromLocalToGlobalResources()
{
    // sum local to global
    for(unsigned int i = 0; i < NUM_PSTATS; ++i)
        mGlobalResources[i].SumValue(mLocalResources[i].GetValue());

    // switch to using global resources
    mUseGlobalRes = true;
}

StatValue & Player::GetCurrentResource(Stat sid)
{
    return mUseGlobalRes ? mGlobalResources[sid] : mLocalResources[sid];
}

const StatValue & Player::GetCurrentResource(Stat sid) const
{
    return mUseGlobalRes ? mGlobalResources[sid] : mLocalResources[sid];
}

} // namespace game
