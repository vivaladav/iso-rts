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

    void SetAttackTarget(GameObject * obj);

    void Update(float delta) override;

private:
    void UpdateImage() override;

    void SetImage();

    void Shoot();

private:
    int mLevel = 0;

    // stats
    std::vector<int> mStats;

    // weapon
    float mTimeAttack = 0.25f;
    float mTimerAttack = 0.f;
    float mWeaponDamage = 40.f;

    GameObject * mTarget = nullptr;

    UnitType mUnitType;
};

inline UnitType Unit::GetUnitType() const { return mUnitType; }

inline int Unit::GetUnitLevel() const { return mLevel; }

inline void Unit::SetAttackTarget(GameObject * obj) { mTarget = obj; }

} // namespace game

