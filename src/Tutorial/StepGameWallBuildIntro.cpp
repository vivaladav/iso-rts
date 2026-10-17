#include "Tutorial/StepGameWallBuildIntro.h"

#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameWallBuildIntro::StepGameWallBuildIntro(const sgl::core::Pointd2D & p0)
    : TutorialInfoStep(550, 150)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // INFO
    auto info = GetPanelInfo();

    info->SetPosition(p0.x, p0.y);

    info->AddInfoEntry(sm->GetCString("TUT_GAME_BUILD_WALL_1"), 7.f, true, false);

    info->SetFunctionOnFinished([this]
                                {
                                    SetDone();
                                });
}

} // namespace game
