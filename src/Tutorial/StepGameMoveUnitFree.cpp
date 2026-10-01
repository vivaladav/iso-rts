#include "Tutorial/StepGameMoveUnitFree.h"

#include "Game.h"
#include "IsoMap.h"
#include "GameObjects/Unit.h"
#include "Widgets/Tutorial/IsoFocusArea.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameMoveUnitFree::StepGameMoveUnitFree(const Game * game, const Unit * unit,
                                           const IsoMap * isoMap, const char * text,
                                           const Cell2D & target, const sgl::core::Pointd2D & p0)
    : TutorialInfoStep(550, 130)
    , mFocusArea(new IsoFocusArea(isoMap))
    , mUnit(unit)
    , mTarget(target)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // FOCUS
    mFocusArea->SetVisible(false);

    // INFO
    auto info = GetPanelInfo();

    info->SetPosition(p0.x, p0.y);

    info->AddActionEntry(sm->GetCString(text), 0.f, false, false, [this, isoMap, game]
                        {
                            // FOCUS
                            mFocusArea->SetCell(mTarget.row, mTarget.col);
                            mFocusArea->SetCornersColorAction();
                            mFocusArea->SetBlinking(true);
                            mFocusArea->SetVisible(true);

                            // CLICK FILTER
                            auto cf = GetClickFilter();
                            cf->ExpandClickableArea();
                        });
}

StepGameMoveUnitFree::~StepGameMoveUnitFree()
{
    delete mFocusArea;
}

void StepGameMoveUnitFree::Update(float)
{
    if(mUnit->GetRow0() == mTarget.row && mUnit->GetCol0() == mTarget.col)
        SetDone();
}

} // namespace game
