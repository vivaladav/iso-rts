#pragma once

#include "Cell2D.h"
#include "Tutorial/TutorialInfoStep.h"

#include <sgl/core/Point.h>

namespace game
{

class IsoFocusArea;
class IsoMap;

class StepGameInfoArea : public TutorialInfoStep
{
public:
    StepGameInfoArea(const IsoMap * isoMap, const Cell2D & areaTL, const Cell2D & areaBR,
                     const char * text1, const char * text2, const sgl::core::Pointd2D & p0);
    ~StepGameInfoArea();

    void OnStart() override;

private:
    IsoFocusArea * mFocusArea = nullptr;

    const Cell2D mTarget;
};

} // namespace game
