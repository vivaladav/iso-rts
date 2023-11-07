#pragma once

#include "GameObject.h"

#include <vector>

namespace game
{

struct ObjectBasicData;
struct ObjectFactionData;

class Unit : public GameObject
{
public:
    Unit(const ObjectBasicData & objData, const ObjectFactionData & facData);

    int GetUnitLevel() const;
    void IncreaseUnitLevel();

    int GetAttackRange() const;
    void ClearAttackTarget();
    bool SetAttackTarget(GameObject * obj);

    void SetActiveActionToDefault() override;

    void Update(float delta) override;

    void ClearStructureToBuild();
    void SetStructureToBuild(GameObjectTypeId type);
    GameObjectTypeId GetStructureToBuild() const;

    bool HasEnergyForAction(GameObjectActionId action);
    void ConsumeEnergy(GameObjectActionId action);

    int GetStat(unsigned int index) const;

public:
    static unsigned int TypeToIndex(GameObjectTypeId type);
    static GameObjectTypeId IndexToType(unsigned int ind);

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

    GameObjectTypeId mStructToBuild;
};

inline int Unit::GetUnitLevel() const { return mLevel; }

inline int Unit::GetAttackRange() const { return mAttackRange; }
inline void Unit::ClearAttackTarget() { mTarget = nullptr; }
inline void Unit::SetStructureToBuild(GameObjectTypeId type) { mStructToBuild = type; }
inline GameObjectTypeId Unit::GetStructureToBuild() const { return mStructToBuild; }

} // namespace game

