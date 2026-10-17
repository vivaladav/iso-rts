#pragma once

#include "Tutorial/TutorialInfoStep.h"

#include <sgl/core/Point.h>

namespace game
{

class Game;

class StepGameEnding : public TutorialInfoStep
{
public:
    StepGameEnding(const Game * game, const sgl::core::Pointd2D & p0);
};

} // namespace game
