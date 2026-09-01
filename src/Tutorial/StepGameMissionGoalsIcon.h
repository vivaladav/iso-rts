#pragma once

#include "Tutorial/TutorialInfoStep.h"

namespace game
{

class FocusArea;
class Game;
class PanelObjectActions;

class StepGameMissionGoalsIcon : public TutorialInfoStep
{
public:
    StepGameMissionGoalsIcon(const Game * game, Screen * screen, PanelObjectActions * panel,
                             bool showIntro);
    ~StepGameMissionGoalsIcon();

    void SetPause(bool paused) override;

private:
    FocusArea * mFocusArea = nullptr;
    PanelObjectActions * mPanelActions = nullptr;

    unsigned int mClickId = 0;

    bool mShowingFocus = false;
};

} // namespace game
