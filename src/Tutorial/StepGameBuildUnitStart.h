#pragma once

#include "Tutorial/TutorialInfoStep.h"

namespace game
{

class FocusArea;
class Game;
class PanelObjectActions;

class StepGameBuildUnitStart : public TutorialInfoStep
{
public:
    StepGameBuildUnitStart(const Game * game, Screen * screen, PanelObjectActions * panel,
                           unsigned int buttonId);
    ~StepGameBuildUnitStart();

    void SetPause(bool paused) override;

private:
    FocusArea * mFocusArea = nullptr;
    PanelObjectActions * mPanelActions = nullptr;

    unsigned int mBtnId;
    unsigned int mClickId = 0;

    bool mShowingFocus = false;
};

} // namespace game
