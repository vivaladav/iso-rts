#pragma once

#include "GameObject.h"

#include <vector>

namespace game
{

struct ObjectData;

enum StructureType : unsigned int;

enum UnitType : unsigned int
{
    UNIT_1,
    UNIT_2,
    UNIT_3,
    UNIT_4,

    NUM_UNIT_TYPES,

    UNIT_NULL
};

class Unit : public GameObject
{
public:
    Unit(const ObjectData & data, int rows, int cols);

    UnitType GetUnitType() const;

    int GetUnitLevel() const;
    void IncreaseUnitLevel();

    int GetAttackRange() const;
    void ClearAttackTarget();
    bool SetAttackTarget(GameObject * obj);

    void SetActiveActionToDefault() override;

    void Update(float delta) override;

    void ClearStructureToBuild();
    void SetStructureToBuild(StructureType type);
    StructureType GetStructureToBuild() const;

    bool HasEnergyForAction(GameObjectActionId action);
    void ConsumeEnergy(GameObjectActionId action);

    int GetStat(unsigned int index) const;

public:
    static const char * TITLES[NUM_UNIT_TYPES];
    static const char * DESCRIPTIONS[NUM_UNIT_TYPES];

private:
    void UpdateGraphics() override;

    bool IsTargetInRange(GameObject * obj) const;

    void SetImage();

    void Shoot();

private:
    int mLevel = 0;

    // stats
    std::vector<int> mStats;

    // weapon
    float mTimeAttack = 0.25f;
    float mTimerAttack = 0.f;
    float mWeaponDamage = 10.f;
    int mAttackRange = 1;

    GameObject * mTarget = nullptr;

    UnitType mUnitType;

    StructureType mStructToBuild;
};

inline UnitType Unit::GetUnitType() const { return mUnitType; }

inline int Unit::GetUnitLevel() const { return mLevel; }

inline int Unit::GetAttackRange() const { return mAttackRange; }
inline void Unit::ClearAttackTarget() { mTarget = nullptr; }
inline void Unit::SetStructureToBuild(StructureType type) { mStructToBuild = type; }
inline StructureType Unit::GetStructureToBuild() const { return mStructToBuild; }

} // namespace game

