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

class StepGameWaitObjectDestroyed : public TutorialInfoStep
{
public:
    StepGameWaitObjectDestroyed(const Game * game, const GameObject * target, const IsoMap * isoMap,
                                const GameMap * gm, const char * text);
    ~StepGameWaitObjectDestroyed();

    void Update(float delta) override;

private:
    IsoFocusArea * mFocusArea = nullptr;

    const GameMap * mGameMap = nullptr;
    const GameObject * mTarget = nullptr;
};

} // namespace game
