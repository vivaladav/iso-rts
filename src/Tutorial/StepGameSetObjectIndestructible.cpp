#include "Tutorial/StepGameSetObjectIndestructible.h"

#include "GameObjects/GameObject.h"

namespace game
{

StepGameSetObjectIndestructible::StepGameSetObjectIndestructible(GameObject * obj, bool val)
    : mValue(val)
{
    mObjs.emplace_back(obj);
}

StepGameSetObjectIndestructible::StepGameSetObjectIndestructible(const std::vector<GameObject *> && objs,
                                                                 bool val)
    : mObjs(objs)
    , mValue(val)
{
}

void StepGameSetObjectIndestructible::OnStart()
{
    TutorialStep::OnStart();

    for(GameObject * obj : mObjs)
        obj->SetIndestructible(mValue);

    SetDone();
}

} // namespace game
