#include "Tutorial/StepGameTurnEnergy.h"

#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameTurnEnergy::StepGameTurnEnergy()
    : TutorialInfoStep(650, 200)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // INFO
    auto info = GetPanelInfo();

    info->SetPosition(1200, 450);

    info->AddInfoEntry(sm->GetCString("TUT_GAME_TURN_ENERGY_2"), 10.f, true, false);

    info->SetFunctionOnFinished([this]
                                {
                                    SetDone();
                                });
}

} // namespace game
