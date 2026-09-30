#include "Tutorial/StepGameWaitSelectedHasEnergy.h"

#include "Player.h"
#include "GameObjects/GameObject.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameWaitSelectedHasEnergy::StepGameWaitSelectedHasEnergy(const Player * p,
                                                             GameObjectTypeId objType,
                                                             float minEnergy, const char * text)
    : TutorialInfoStep(550, 130)
    , mPlayer(p)
    , mWantedType(objType)
    , mMinEnergy(minEnergy)
{
    // OPTONAL INFO
    if(text != nullptr)
    {
        auto sm = sgl::utilities::StringManager::Instance();

        auto info = GetPanelInfo();
        info->AddActionEntry(sm->GetCString(text), 0.f, false, false);
    }
}

void StepGameWaitSelectedHasEnergy::OnStart()
{
    TutorialInfoStep::OnStart();

    GetClickFilter()->SetEnabled(false);
}

void StepGameWaitSelectedHasEnergy::Update(float delta)
{
    auto obj = mPlayer->GetSelectedObject();

    if(obj != nullptr && obj->GetObjectType() == mWantedType && obj->GetEnergy() >= mMinEnergy)
        SetDone();
}

} // namespace game
