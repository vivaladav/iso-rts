#pragma once

#include "Cell2D.h"
#include "Tutorial/TutorialInfoStep.h"

#include <sgl/core/Point.h>

namespace game
{

class GameMap;
class IsoFocusArea;
class IsoMap;

class StepGameMoveToCellFree : public TutorialInfoStep
{
public:
    StepGameMoveToCellFree(const GameMap * gm, const IsoMap * isoMap, const char * text,
                           const Cell2D & target, const sgl::core::Pointd2D & p0);
    ~StepGameMoveToCellFree();

    void Update(float delta) override;

private:
    IsoFocusArea * mFocusArea = nullptr;

    const GameMap * mGameMap = nullptr;

    const Cell2D mTarget;
};

} // namespace game
