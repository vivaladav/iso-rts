#pragma once

#include "Tutorial/TutorialInfoStep.h"

namespace sgl { namespace sgui { class AbstractButton; } }

namespace game
{

class FocusArea;
class PanelPlanetActionConquer;

class StepPlanetMapConquerTerritoryStart : public TutorialInfoStep
{
public:
    StepPlanetMapConquerTerritoryStart(PanelPlanetActionConquer * panel);
    ~StepPlanetMapConquerTerritoryStart();

    void SetPause(bool paused) override;

private:
    FocusArea * mFocusArea = nullptr;

    sgl::sgui::AbstractButton * mButton = nullptr;
    unsigned int mFuncId = 0;

    bool mShowingFocus = false;
};

} // namespace game
