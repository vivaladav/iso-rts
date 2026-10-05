#include "Tutorial/StepGameEnergyRegeneration.h"

#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameEnergyRegeneration::StepGameEnergyRegeneration()
    : TutorialInfoStep(700, 175)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // INFO
    auto info = GetPanelInfo();

    info->SetPosition(1100, 600);

    info->AddInfoEntry(sm->GetCString("TUT_GAME_ENERGY_REGENERATION_2"), 14.f, true, false);

    info->SetFunctionOnFinished([this]
    {
        SetDone();
    });
}

} // namespace game
