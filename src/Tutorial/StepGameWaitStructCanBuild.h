#pragma once

#include "GameObjects/GameObjectTypes.h"
#include "Tutorial/TutorialInfoStep.h"

namespace game
{

class ObjectData;
class Player;

class StepGameWaitStructCanBuild : public TutorialInfoStep
{
public:
    StepGameWaitStructCanBuild(const Player * p, const ObjectData & buildData,
                               GameObjectTypeId structType, const char * text);

    void OnStart() override;

    void Update(float delta) override;

private:
    const Player * mPlayer = nullptr;

    const ObjectData & mData;

    GameObjectTypeId mStruct;
};

} // namespace game
