#pragma once

#include "Tutorial/TutorialInfoStep.h"

namespace game
{

class ObjectData;
class Player;

class StepGameWaitUnitCanSpawn : public TutorialInfoStep
{
public:
    StepGameWaitUnitCanSpawn(const Player * p, const ObjectData & buildData, unsigned int squads,
                             unsigned int elements, const char * text);

    void OnStart() override;

    void Update(float delta) override;

private:
    const Player * mPlayer = nullptr;

    const ObjectData & mData;

    unsigned int mSquads = 0;
    unsigned int mElements = 0;
};

} // namespace game
