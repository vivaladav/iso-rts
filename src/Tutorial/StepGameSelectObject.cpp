#include "Tutorial/StepGameSelectObject.h"

#include "Game.h"
#include "GameObjects/GameObject.h"
#include "Widgets/Tutorial/IsoFocusArea.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameSelectObject::StepGameSelectObject(const Game * game, const IsoMap * im, const GameObject * obj,
                                           const char * text, const sgl::core::Pointd2D & p0,
                                           float animDelay)
    : TutorialInfoStep(550, 150)
    , mFocusArea(new IsoFocusArea(im))
    , mObj(obj)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // FOCUS
    mFocusArea->SetVisible(false);

    // INFO
    auto info = GetPanelInfo();

    info->SetPosition(p0.x, p0.y);

    info->AddActionEntry(sm->GetCString(text), 0.f, false, false, [this, game, im, animDelay]
                        {
                            // FOCUS AREA
                            mFocusArea->SetCellArea(mObj->GetRow0(), mObj->GetCol0(),
                                                    mObj->GetRow1(), mObj->GetCol1(), true, animDelay);
                            mFocusArea->SetCornersColorAction();
                            mFocusArea->SetBlinking(true);
                            mFocusArea->SetVisible(true);

                            // CLICK FILTER
                            auto cf = GetClickFilter();
                            cf->AddClickableCells(im, mObj->GetRow1(), mObj->GetCol1(),
                                                  mObj->GetRow0(), mObj->GetCol0());
                            cf->SetButtonToAllow(game->GetButtonSelect());
                        });
}

StepGameSelectObject::~StepGameSelectObject()
{
    delete mFocusArea;
}

void StepGameSelectObject::Update(float)
{
    if(mObj->IsSelected())
        SetDone();
}

} // namespace game
