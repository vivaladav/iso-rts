#pragma once

#include "GameObject.h"

namespace game
{

class Unit : public GameObject
{
public:
    Unit(int owner, unsigned int elements = 1);

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
    unsigned int mElements;
};

inline int Unit::GetUnitLevel() const { return mLevel; }

inline int Unit::GetNumElements() const { return mElements; }

} // namespace game

