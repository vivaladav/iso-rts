#pragma once

#include "Tutorial/TutorialStep.h"

#include <vector>

namespace game
{

class GameObject;

class StepGameSetObjectIndestructible : public TutorialStep
{
public:
    StepGameSetObjectIndestructible(GameObject * obj, bool val);
    StepGameSetObjectIndestructible(const std::vector<GameObject *> && objs, bool val);

    void OnStart() override;

private:
    std::vector<GameObject *> mObjs;
    bool mValue;
};

} // namespace game
