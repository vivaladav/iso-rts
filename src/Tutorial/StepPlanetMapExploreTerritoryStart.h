#pragma once

#include "Tutorial/TutorialInfoStep.h"

namespace sgl { namespace sgui { class AbstractButton; } }

namespace game
{

class FocusArea;
class PanelPlanetActionExplore;

class StepPlanetMapExploreTerritoryStart : public TutorialInfoStep
{
public:
    StepPlanetMapExploreTerritoryStart(PanelPlanetActionExplore * panelExplore);
    ~StepPlanetMapExploreTerritoryStart();

    void OnStart() override;

    void SetPause(bool paused) override;

private:
    FocusArea * mFocusArea = nullptr;

    sgl::sgui::AbstractButton * mButton = nullptr;
    unsigned int mFuncId = 0;
};

} // namespace game
