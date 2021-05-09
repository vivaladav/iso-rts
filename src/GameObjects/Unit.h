#pragma once

#include "GameObject.h"

#include <vector>

namespace game
{

struct UnitData;

enum UnitType : unsigned int
{
    UNIT_1,
    UNIT_2,
    UNIT_3,

    NUM_UNIT_TYPES
};

class Unit : public GameObject
{
public:
    Unit(const UnitData & data, int rows, int cols);

    UnitType GetUnitType() const;

    int GetUnitLevel() const;
    void IncreaseUnitLevel();

protected:
    void UpdateImage() override;

private:
    void SetImage();

private:
    int mLevel = 0;

    // stats
    std::vector<int> mStats;

    UnitType mUnitType;
};

inline UnitType Unit::GetUnitType() const { return mUnitType; }

inline int Unit::GetUnitLevel() const { return mLevel; }

} // namespace game

