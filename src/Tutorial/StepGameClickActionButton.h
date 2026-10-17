#pragma once

#include "Tutorial/TutorialInfoStep.h"

namespace game
{

class FocusArea;
class Game;
class PanelObjectActions;

class StepGameClickActionButton : public TutorialInfoStep
{
public:
    StepGameClickActionButton(const Game * game, Screen * screen, PanelObjectActions * panel,
                              const char * text, unsigned int buttonId, float animDelay = 0.5f);
    ~StepGameClickActionButton();

    void SetPause(bool paused) override;

private:
    FocusArea * mFocusArea = nullptr;
    PanelObjectActions * mPanelActions = nullptr;

    unsigned int mBtnId;
    unsigned int mClickId = 0;

    bool mShowingFocus = false;
};

} // namespace game
