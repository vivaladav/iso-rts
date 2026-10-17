#pragma once

#include "Tutorial/TutorialInfoStep.h"

namespace game
{

class ObjectData;
class Player;

class StepGameWaitUnitCanBuild : public TutorialInfoStep
{
public:
    StepGameWaitUnitCanBuild(const Player * p, const ObjectData & buildData, const char * text);

    void OnStart() override;

    void Update(float delta) override;

private:
    const Player * mPlayer = nullptr;

    const ObjectData & mData;
};

} // namespace game
