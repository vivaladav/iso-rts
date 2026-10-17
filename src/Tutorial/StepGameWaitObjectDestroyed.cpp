#include "Tutorial/StepGameWaitObjectDestroyed.h"

#include "Game.h"
#include "GameMap.h"
#include "IsoMap.h"
#include "GameObjects/GameObject.h"
#include "Widgets/Tutorial/IsoFocusArea.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameWaitObjectDestroyed::StepGameWaitObjectDestroyed(const Game * game,
                                                         const GameObject * target,
                                                         const IsoMap * isoMap,
                                                         const GameMap * gm, const char * text)
    : TutorialInfoStep(500, 150)
    , mFocusArea(new IsoFocusArea(isoMap))
    , mGameMap(gm)
    , mTarget(target)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // FOCUS
    mFocusArea->SetVisible(false);

    // INFO
    auto info = GetPanelInfo();

    info->AddActionEntry(sm->GetCString(text), 0.f, false, false, [this, isoMap, game]
                        {
                            // FOCUS
                            mFocusArea->SetCellArea(mTarget->GetRow0(), mTarget->GetCol0(),
                                                    mTarget->GetRow1(), mTarget->GetCol1(), false);
                            mFocusArea->SetCornersColorAction();
                            mFocusArea->SetVisible(true);

                            // CLICK FILTER
                            auto cf = GetClickFilter();
                            cf->ExpandClickableArea();
                        });

    if(mTarget == nullptr || !mGameMap->HasObject(mTarget))
        SetDone();
}

StepGameWaitObjectDestroyed::~StepGameWaitObjectDestroyed()
{
    delete mFocusArea;
}

void StepGameWaitObjectDestroyed::Update(float)
{
    if(!mGameMap->HasObject(mTarget))
        SetDone();
}

} // namespace game
