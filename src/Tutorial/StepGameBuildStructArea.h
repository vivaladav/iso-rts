#pragma once

#include "Cell2D.h"
#include "Tutorial/TutorialInfoStep.h"

namespace game
{

class Game;
class IsoFocusArea;
class IsoMap;
class Player;

class StepGameBuildStructArea : public TutorialInfoStep
{
public:
    StepGameBuildStructArea(const Game * game, const IsoMap * isoMap, const Cell2D & areaTL,
                            const Cell2D & areaBR, const char * text);
    ~StepGameBuildStructArea();

    void OnStart() override;

    void Update(float delta) override;

private:
    IsoFocusArea * mFocusArea = nullptr;

    const Player * mPlayer = nullptr;

    const Cell2D mTarget;

    bool mBuildStarted = false;
};

} // namespace game
