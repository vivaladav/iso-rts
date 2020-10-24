#pragma once

#include <functional>
#include <string>

namespace game
{

class PlayerAI;

struct Cell2D;

class Player
{
public:
    Player(const char * name, int pid);
    ~Player();

    const std::string & GetName() const;

    int GetPlayerId() const;

    int GetNumCells() const;
    void SumCells(int val);
    void SetOnNumCellsChanged(const std::function<void(int)> & f);

    int GetTotalCellsLevel() const;
    void SumTotalCellsLevel(int val);

    int GetMoney() const;
    void SumMoney(int val);
    void SetOnMoneyChanged(const std::function<void(int)> & f);

    int GetNumUnits() const;
    void SumUnits(int val);
    void SetOnNumUnitsChanged(const std::function<void(int)> & f);

    void ClearSelectedCell();
    const Cell2D * GetSelectedCell() const;
    void SetSelectedCell(const Cell2D & cell);

    // -- AI --
    bool IsAI() const;
    PlayerAI * GetAI();
    void SetAI(PlayerAI * ai);

    bool IsLocal() const;
    void SetLocal(bool val);

private:
    std::string mName;

    std::function<void(int)> mOnNumCellsChanged;
    std::function<void(int)> mOnMoneyChanged;
    std::function<void(int)> mOnNumUnitsChanged;

    PlayerAI * mAI = nullptr;

    Cell2D * mCurrSelectedCell = nullptr;
    Cell2D * mSelectedCell = nullptr;

    int mPlayerId;

    int mNumCells = 0;
    int mTotCellsLevel = 0;
    int mMoney = 0;
    int mNumUnits = 0;

    bool mLocal = false;
};

inline const std::string & Player::GetName() const { return mName; }

inline int Player::GetPlayerId() const { return mPlayerId; }

inline int Player::GetNumCells() const { return mNumCells; }
inline void Player::SetOnNumCellsChanged(const std::function<void(int)> & f)
{
    mOnNumCellsChanged = f;
}

inline int Player::GetTotalCellsLevel() const { return mTotCellsLevel; }
inline void Player::SumTotalCellsLevel(int val) { mTotCellsLevel += val; }

inline int Player::GetMoney() const { return mMoney; }
inline void Player::SetOnMoneyChanged(const std::function<void(int)> & f)
{
    mOnMoneyChanged = f;
}

inline int Player::GetNumUnits() const { return  mNumUnits; }
inline void Player::SetOnNumUnitsChanged(const std::function<void(int)> &f)
{
    mOnNumUnitsChanged = f;
}

inline void Player::ClearSelectedCell() { mCurrSelectedCell = nullptr; }
inline const Cell2D * Player::GetSelectedCell() const { return mCurrSelectedCell; }

inline bool Player::IsAI() const { return mAI != nullptr; }
inline PlayerAI * Player::GetAI() { return mAI; }
inline void Player::SetAI(PlayerAI * ai) { mAI = ai; }

inline bool Player::IsLocal() const { return mLocal; }
inline void Player::SetLocal(bool val) { mLocal = val; }

} // namespace game
