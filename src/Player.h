#pragma once

#include <string>

namespace game
{

class Player
{
public:
    Player(const char * name, int pid);

    const std::string & GetName() const;

    int GetPlayerId() const;

    int GetNumCells() const;
    void SumCells(int val);

    int GetTotalCellsLevel() const;
    void SumTotalCellsLevel(int val);

    int GetMoney() const;
    void SumMoney(int val);

    int GetNumUnits() const;
    void SumUnits(int val);

private:
    std::string mName;

    int mPlayerId;

    int mNumCells = 0;
    int mTotCellsLevel = 0;
    int mMoney = 0;
    int mNumUnits = 0;
};

inline const std::string & Player::GetName() const { return mName; }

inline int Player::GetPlayerId() const { return mPlayerId; }

inline int Player::GetNumCells() const { return mNumCells; }
inline void Player::SumCells(int val) { mNumCells += val; }

inline int Player::GetTotalCellsLevel() const { return mTotCellsLevel; }
inline void Player::SumTotalCellsLevel(int val) { mTotCellsLevel += val; }

inline int Player::GetMoney() const { return mMoney; }
inline void Player::SumMoney(int val) { mMoney += val; }

inline int Player::GetNumUnits() const { return  mNumUnits; }
inline void Player::SumUnits(int val) { mNumUnits += val; }

} // namespace game
