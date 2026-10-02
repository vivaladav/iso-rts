#pragma once

#include "Tutorial/TutorialInfoStep.h"

#include <sgl/core/Point.h>

namespace game
{

class Game;
class GameMap;
class GameObject;
class IsoFocusArea;
class IsoMap;
class Unit;

class StepGameUnitAttackFree : public TutorialInfoStep
{
public:
    StepGameUnitAttackFree(const Game * game, const Unit * unit, const GameObject * target,
                           const IsoMap * isoMap, const GameMap * gm, const char * text);
    ~StepGameUnitAttackFree();

    void Update(float delta) override;

private:
    IsoFocusArea * mFocusArea = nullptr;

    const GameMap * mGameMap = nullptr;
    const Unit * mUnit = nullptr;
    const GameObject * mTarget = nullptr;
};

} // namespace game
