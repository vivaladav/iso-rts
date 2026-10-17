#pragma once

#include "Tutorial/TutorialInfoStep.h"

#include <sgl/core/Point.h>

namespace game
{

class Game;
class GameObject;
class IsoFocusArea;
class IsoMap;

class StepGameConquerStructFree : public TutorialInfoStep
{
public:
    StepGameConquerStructFree(const Game * game, const GameObject * obj, const IsoMap * isoMap,
                              const char * text, const sgl::core::Pointd2D & p0);
    ~StepGameConquerStructFree();

    void Update(float delta) override;

private:
    IsoFocusArea * mFocusArea = nullptr;

    const GameObject * mStruct = nullptr;
};

} // namespace game
