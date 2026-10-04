#pragma once

#include "Cell2D.h"
#include "Tutorial/TutorialInfoStep.h"

#include <sgl/core/Point.h>

namespace game
{

class GameMap;
class IsoFocusArea;
class IsoMap;

class StepGameBuildWallLine : public TutorialInfoStep
{
public:
    StepGameBuildWallLine(const GameMap * gm, const IsoMap * im,
                      const Cell2D & areaTL, const Cell2D & areaBR,
                      const char * text);
    ~StepGameBuildWallLine();

    void Update(float delta) override;

private:
    IsoFocusArea * mFocusArea = nullptr;

    const GameMap * mGameMap = nullptr;

    const Cell2D mAreaTL;
    const Cell2D mAreaBR;
};

} // namespace game
