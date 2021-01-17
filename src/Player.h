#pragma once

#include "Cell2D.h"

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

class Player
{
public:
    Player(const char * name, int pid);
    ~Player();

    // visibility map
    void InitVisibility(int rows, int cols);
    bool IsCellVisible(unsigned int ind) const;
    void AddVisibility(unsigned int ind);
    void RemVisibility(unsigned int ind);

    const std::string & GetName() const;

    int GetPlayerId() const;

    const Cell2D & GetBaseCell() const;
    void SetBaseCell(const Cell2D & cell);

    int GetNumCells() const;
    void SumCells(int val);
    void SetOnNumCellsChanged(const std::function<void(int)> & f);

    int GetEnergy() const;
    int GetEnergyUse() const;
    void SumEnergy(int val);
    void SetOnEnergyChanged(const std::function<void(int)> & f);

    int GetMaterial() const;
    void SumMaterial(int val);
    void SetOnMaterialChanged(const std::function<void(int)> & f);

    int GetNumUnits() const;
    void SumUnits(int val);
    void SetOnNumUnitsChanged(const std::function<void(int)> & f);

    int GetTotalUnitsLevel() const;
    void SumTotalUnitsLevel(int val);

    bool HasSelectedCell() const;
    void ClearSelectedCell();
    const Cell2D & GetSelectedCell() const;
    void SetSelectedCell(const Cell2D & cell);

    void ClearSelectedObject();
    GameObject * GetSelectedObject() const;
    void SetSelectedObject(GameObject * obj);

    void AddResourceGenerator(unsigned int cellId, ResourceGenerator * gen);
    void RemoveResourceGenerator(unsigned int cellId);

    int GetResourceProduction(ResourceType type) const;
    void UpdateResources();

    // -- AI --
    bool IsAI() const;
    PlayerAI * GetAI();
    void SetAI(PlayerAI * ai);

    bool IsLocal() const;
    void SetLocal(bool val);

private:
    std::vector<int> mVisMap;

    std::string mName;

    std::function<void(int)> mOnNumCellsChanged;
    std::function<void(int)> mOnEnergyChanged;
    std::function<void(int)> mOnMaterialChanged;
    std::function<void(int)> mOnNumUnitsChanged;

    std::unordered_map<unsigned int, ResourceGenerator *> mResGenerators;

    PlayerAI * mAI = nullptr;

    GameObject * mSelObj = nullptr;

    Cell2D mHomeCell;

    Cell2D mSelectedCell;

    int mPlayerId;

    int mNumCells = 0;
    int mTotCellsLevel = 0;
    int mEnergy = 0;
    int mMaterial1 = 0;
    int mNumUnits = 0;
    int mTotUnitsLevel = 0;

    bool mLocal = false;
};

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

inline int Player::GetNumCells() const { return mNumCells; }
inline void Player::SetOnNumCellsChanged(const std::function<void(int)> & f)
{
    mOnNumCellsChanged = f;
}

inline int Player::GetEnergy() const { return mEnergy; }
inline void Player::SetOnEnergyChanged(const std::function<void(int)> & f)
{
    mOnEnergyChanged = f;
}

inline int Player::GetMaterial() const { return mMaterial1; }
inline void Player::SetOnMaterialChanged(const std::function<void(int)> & f)
{
    mOnMaterialChanged = f;
}

inline int Player::GetNumUnits() const { return  mNumUnits; }
inline void Player::SetOnNumUnitsChanged(const std::function<void(int)> &f)
{
    mOnNumUnitsChanged = f;
}

inline int Player::GetTotalUnitsLevel() const { return mTotUnitsLevel; }
inline void Player::SumTotalUnitsLevel(int val) { mTotUnitsLevel += val; }

inline bool Player::HasSelectedCell() const
{
    return mSelectedCell.row >= 0 && mSelectedCell.col >= 0;
}
inline void Player::ClearSelectedCell()
{
    mSelectedCell.row = -1;
    mSelectedCell.col = -1;
}
inline const Cell2D & Player::GetSelectedCell() const { return mSelectedCell; }

inline GameObject * Player::GetSelectedObject() const { return mSelObj; }

inline bool Player::IsAI() const { return mAI != nullptr; }
inline PlayerAI * Player::GetAI() { return mAI; }
inline void Player::SetAI(PlayerAI * ai) { mAI = ai; }

inline bool Player::IsLocal() const { return mLocal; }
inline void Player::SetLocal(bool val) { mLocal = val; }

} // namespace game
