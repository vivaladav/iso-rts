#pragma once

#include "Tutorial/TutorialInfoStep.h"

#include <sgl/core/Point.h>

namespace game
{

class StepGameWallBuildIntro : public TutorialInfoStep
{
public:
    StepGameWallBuildIntro(const sgl::core::Pointd2D & p0 = sgl::core::Pointd2D(1250, 150));
};

} // namespace game
