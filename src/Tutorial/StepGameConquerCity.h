#pragma once

#include "Tutorial/TutorialInfoStep.h"

#include <sgl/core/Point.h>

namespace game
{

class CityGroup;
class IsoFocusArea;
class IsoMap;

class StepGameConquerCity : public TutorialInfoStep
{
public:
    StepGameConquerCity(const CityGroup * cg, const IsoMap * isoMap,
                        const char * text, const sgl::core::Pointd2D & p0);
    ~StepGameConquerCity();

    void Update(float delta) override;

private:
    IsoFocusArea * mFocusArea = nullptr;

    const CityGroup * mCity = nullptr;
};

} // namespace game
