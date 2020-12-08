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

    int GetNumElements() const;
    void SumElements(int num);

protected:
    void UpdateImageId() override;

private:
    void SetImageId();

private:
    int mLevel = 0;
    int mElements = 1;
};

inline int Unit::GetUnitLevel() const { return mLevel; }

inline int Unit::GetNumElements() const { return mElements; }

} // namespace game

