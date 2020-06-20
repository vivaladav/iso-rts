#pragma once

#include <string>

namespace game
{

class Player
{
public:
    Player(const char * name, int pid);

    const std::string & GetName() const;

    int GetNumCells() const;

    int GetMoney() const;

    int GetNumUnits() const;

private:
    std::string mName;

    int mPlayerId;

    int mNumCells = 0;
    int mMoney = 0;
    int mNumUnits = 0;
};

inline const std::string & Player::GetName() const { return mName; }

inline int Player::GetNumCells() const { return mNumCells; }

inline int Player::GetMoney() const { return mMoney; }

inline int Player::GetNumUnits() const { return  mNumUnits; }

} // namespace game
