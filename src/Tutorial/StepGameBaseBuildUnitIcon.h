#pragma once

#include "Tutorial/TutorialInfoStep.h"

namespace game
{

class FocusArea;
class Game;
class PanelObjectActions;

class StepGameBaseBuildUnitIcon : public TutorialInfoStep
{
public:
    StepGameBaseBuildUnitIcon(const Game * game, Screen * screen, PanelObjectActions * panel);
    ~StepGameBaseBuildUnitIcon();

    void SetPause(bool paused) override;

private:
    FocusArea * mFocusArea = nullptr;
    PanelObjectActions * mPanelActions = nullptr;

    unsigned int mClickId = 0;

    bool mShowingFocus = false;
};

} // namespace game
