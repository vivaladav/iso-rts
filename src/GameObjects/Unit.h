#pragma once

#include "GameObject.h"

namespace game
{

enum UnitAction : unsigned int
{
    IDLE,
    MOVE,
    CONQUER,
    ATTACK,

    NUM_UNIT_ACTIONS
};

class Unit : public GameObject
{
public:
    Unit(int rows, int cols);

    int GetUnitLevel() const;
    void IncreaseUnitLevel();

    UnitAction GetActiveAction() const;
    void SetActiveAction(UnitAction action);

protected:
    void UpdateImage() override;

private:
    void SetImage();

private:
    int mLevel = 0;

    UnitAction mActiveAction = IDLE;
};

inline int Unit::GetUnitLevel() const { return mLevel; }

inline UnitAction Unit::GetActiveAction() const { return mActiveAction; }
inline void Unit::SetActiveAction(UnitAction action) { mActiveAction = action; }

} // namespace game

