#pragma once

#include "Tutorial/TutorialInfoStep.h"

#include "GameObjects/GameObjectTypes.h"

namespace game
{

class Player;

class StepGameWaitSelectedHasEnergy : public TutorialInfoStep
{
public:
    StepGameWaitSelectedHasEnergy(const Player * p, GameObjectTypeId objType, float minEnergy,
                                  const char * text);

    void OnStart() override;

    void Update(float delta) override;

private:
    const Player * mPlayer = nullptr;
    GameObjectTypeId mWantedType;
    float mMinEnergy = 0.f;
};

} // namespace game
