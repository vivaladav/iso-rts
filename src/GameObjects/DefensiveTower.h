#pragma once

#include "Structure.h"

namespace game
{

class DefensiveTower : public Structure
{
public:
    DefensiveTower(int rows, int cols);

    void Update(float delta) override;

protected:
    void UpdateGraphics() override;

private:
    void SetImage();

    void CheckForEnemies();

    void Shoot();

private:
    GameObject * mTarget = nullptr;

    int mAttackRadius = 4;

    // weapon
    float mTimeAttack = 0.25f;
    float mTimerAttack = 0.f;
    float mWeaponDamage = 15.f;
};

} // namespace game
