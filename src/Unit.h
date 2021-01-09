#pragma once

#include "GameObject.h"

namespace game
{

class Unit : public GameObject
{
public:
    Unit(int owner);

    int GetUnitLevel() const;
    void IncreaseUnitLevel();

protected:
    void UpdateImageId() override;

private:
    void SetImageId();

private:
    int mLevel = 0;
};

inline int Unit::GetUnitLevel() const { return mLevel; }

} // namespace game

