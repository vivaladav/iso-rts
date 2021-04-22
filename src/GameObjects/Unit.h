#pragma once

#include "GameObject.h"

namespace game
{

class Unit : public GameObject
{
public:
    Unit(int rows, int cols);

    int GetUnitLevel() const;
    void IncreaseUnitLevel();

protected:
    void UpdateImage() override;

private:
    void SetImage();

private:
    int mLevel = 0;
};

inline int Unit::GetUnitLevel() const { return mLevel; }

} // namespace game

