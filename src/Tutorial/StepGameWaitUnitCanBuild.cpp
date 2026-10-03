#include "Tutorial/StepGameWaitUnitCanBuild.h"

#include "Player.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/ObjectData.h"
#include "GameObjects/Unit.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameWaitUnitCanBuild::StepGameWaitUnitCanBuild(const Player * p, const ObjectData & buildData,
                                                   const char * text)
    : TutorialInfoStep(550, 130)
    , mPlayer(p)
    , mData(buildData)
{
    // OPTONAL INFO
    if(text != nullptr)
    {
        auto sm = sgl::utilities::StringManager::Instance();

        auto info = GetPanelInfo();
        info->AddActionEntry(sm->GetCString(text), 0.f, false, false);
    }
}

void StepGameWaitUnitCanBuild::OnStart()
{
    TutorialInfoStep::OnStart();

    GetClickFilter()->SetEnabled(false);
}

void StepGameWaitUnitCanBuild::Update(float delta)
{
    auto obj = mPlayer->GetSelectedObject();

    // no selected object
    if(obj == nullptr)
        return ;

    const float minEnergy = obj->GetEnergyForActionStep(BUILD_STRUCTURE);

    // selecte object is not an unit OR can't build OR doesn't have enough energy
    if(obj->GetObjectCategory() != ObjectData::CAT_UNIT ||
       !static_cast<Unit *>(obj)->CanBuild() || obj->GetEnergy() < minEnergy)
       return;

    // check if player can afford building
    const auto & costs = mData.GetCosts();
    const bool costOk = mPlayer->HasEnough(Player::ENERGY, costs[RES_ENERGY]) &&
                        mPlayer->HasEnough(Player::MATERIAL, costs[RES_MATERIAL1]) &&
                        mPlayer->HasEnough(Player::DIAMONDS, costs[RES_DIAMONDS]) &&
                        mPlayer->HasEnough(Player::BLOBS, costs[RES_BLOBS]);

    if(costOk)
        SetDone();
}

} // namespace game
