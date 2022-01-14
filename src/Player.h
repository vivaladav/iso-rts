#pragma once

#include "Cell2D.h"
#include "StatValue.h"
#include "GameObjects/ObjectData.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace game
{

class GameObject;
class PlayerAI;
class ResourceGenerator;

enum ResourceType : unsigned int;
enum PlayerFaction : unsigned int;
enum UnitType : unsigned int;

class Player
{
public:
    enum Stat : unsigned int
    {
        BLOBS,
        DIAMONDS,
        ENERGY,
        MATERIAL,
        MONEY,

        NUM_PSTATS,

        INVALID_STAT
    };

public:
    Player(const char * name, int pid);
    ~Player();

    PlayerFaction GetFaction() const;
    void SetFaction(PlayerFaction faction);

    // visibility map
    void InitVisibility(int rows, int cols);
    bool IsCellVisible(unsigned int ind) const;
    void AddVisibility(unsigned int ind);
    void RemVisibility(unsigned int ind);
    void AddVisibilityToAll();
    void RemVisibilityToAll();

    const std::string & GetName() const;

    int GetPlayerId() const;

    const Cell2D & GetBaseCell() const;
    void SetBaseCell(const Cell2D & cell);

    // stats
    StatValue & GetStat(Stat sid);
    void SetOnResourcesChanged(const std::function<void()> & f);

    int GetNumCells() const;
    void SumCells(int val);
    void SetOnNumCellsChanged(const std::function<void(int)> & f);

    int GetEnergyUse() const;

    int GetNumUnits() const;
    void SumUnits(int val);
    void SetOnNumUnitsChanged(const std::function<void(int)> & f);

    int GetTotalUnitsLevel() const;
    void SumTotalUnitsLevel(int val);

    // available units
    void AddAvailableUnit(const ObjectData & data);
    const std::vector<ObjectData> & GetAvailableUnits() const;
    const ObjectData & GetAvailableUnit(UnitType type) const;

    void ClearSelectedObject();
    GameObject * GetSelectedObject() const;
    void SetSelectedObject(GameObject * obj);
    bool HasSelectedObject() const;

    void AddResourceGenerator(unsigned int cellId, ResourceGenerator * gen);
    void RemoveResourceGenerator(unsigned int cellId);

    int GetResourceProduction(ResourceType type) const;
    void UpdateResources();

    void HandleCollectable(GameObject * obj);

    // -- AI --
    bool IsAI() const;
    PlayerAI * GetAI();
    void SetAI(PlayerAI * ai);

    bool IsLocal() const;
    void SetLocal(bool val);

private:
    std::vector<int> mVisMap;

    std::vector<StatValue> mStats;
    StatValue mDummyStat;

    std::vector<ObjectData> mAvailableUnits;

    std::string mName;

    std::function<void(int)> mOnNumCellsChanged;
    std::function<void(int)> mOnNumUnitsChanged;
    std::function<void()> mOnResourcesChanged;

    std::unordered_map<unsigned int, ResourceGenerator *> mResGenerators;

    PlayerAI * mAI = nullptr;

    GameObject * mSelObj = nullptr;

    Cell2D mHomeCell;

    int mPlayerId;

    PlayerFaction mFaction;

    int mNumCells = 0;
    int mTotCellsLevel = 0;
    int mEnergy = 0;
    int mMaterial1 = 0;
    int mDiamonds = 0;
    int mNumUnits = 0;
    int mTotUnitsLevel = 0;

    bool mLocal = false;
};

inline PlayerFaction Player::GetFaction() const { return mFaction; }
inline void Player::SetFaction(PlayerFaction faction) { mFaction = faction; }

inline bool Player::IsCellVisible(unsigned int ind) const
{
    return mVisMap[ind] > 0;
}

inline void Player::AddVisibility(unsigned int ind)
{
    ++mVisMap[ind];
}

inline void Player::RemVisibility(unsigned int ind)
{
    if(mVisMap[ind] > 0)
        --mVisMap[ind];
}

inline const std::string & Player::GetName() const { return mName; }

inline int Player::GetPlayerId() const { return mPlayerId; }

inline const Cell2D & Player::GetBaseCell() const { return mHomeCell; }
inline void Player::SetBaseCell(const Cell2D & cell) { mHomeCell = cell; }

inline StatValue & Player::GetStat(Stat sid)
{
    if(sid < NUM_PSTATS)
        return mStats[sid];
    else
        return mDummyStat;
}

inline void Player::SetOnResourcesChanged(const std::function<void()> & f) { mOnResourcesChanged = f; }

inline int Player::GetNumCells() const { return mNumCells; }
inline void Player::SetOnNumCellsChanged(const std::function<void(int)> & f)
{
    mOnNumCellsChanged = f;
}

inline int Player::GetNumUnits() const { return  mNumUnits; }
inline void Player::SetOnNumUnitsChanged(const std::function<void(int)> &f)
{
    mOnNumUnitsChanged = f;
}

inline int Player::GetTotalUnitsLevel() const { return mTotUnitsLevel; }
inline void Player::SumTotalUnitsLevel(int val) { mTotUnitsLevel += val; }

inline const std::vector<ObjectData> & Player::GetAvailableUnits() const { return mAvailableUnits; }
inline const ObjectData & Player::GetAvailableUnit(UnitType type) const { return mAvailableUnits[type]; }

inline GameObject * Player::GetSelectedObject() const { return mSelObj; }
inline bool Player::HasSelectedObject() const { return mSelObj != nullptr; }

inline bool Player::IsAI() const { return mAI != nullptr; }
inline PlayerAI * Player::GetAI() { return mAI; }
inline void Player::SetAI(PlayerAI * ai) { mAI = ai; }

inline bool Player::IsLocal() const { return mLocal; }
inline void Player::SetLocal(bool val) { mLocal = val; }

} // namespace game
