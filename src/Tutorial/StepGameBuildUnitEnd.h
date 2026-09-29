#pragma once

#include "Tutorial/TutorialInfoStep.h"

namespace game
{

class GameHUD;
class FocusArea;

class StepGameBuildUnitEnd : public TutorialInfoStep
{
public:
    StepGameBuildUnitEnd(GameHUD * HUD, const char * text = "TUT_GAME_BASE_BUILD_UNIT_3",
                         float animDelay = 0.5f);
    ~StepGameBuildUnitEnd();

    void OnStart() override;

private:
    FocusArea * mFocusArea = nullptr;

    GameHUD * mHUD = nullptr;
    unsigned int mButtonActId;
};

} // namespace game
