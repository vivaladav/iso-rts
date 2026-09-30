#include "Tutorial/StepGameConquerStructFree.h"

#include "Game.h"
#include "IsoMap.h"
#include "GameObjects/Unit.h"
#include "Widgets/Tutorial/IsoFocusArea.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameConquerStructFree::StepGameConquerStructFree(const Game * game, const Unit * unit,
                                                     const GameObject * obj,
                                                     const IsoMap * isoMap, const char * text,
                                                     const sgl::core::Pointd2D & p0)
    : TutorialInfoStep(550, 150)
    , mFocusArea(new IsoFocusArea(isoMap))
    , mStruct(obj)
    , mUnit(unit)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // FOCUS
    mFocusArea->SetVisible(false);

    // INFO
    auto info = GetPanelInfo();

    info->SetPosition(p0.x, p0.y);

    info->AddActionEntry(sm->GetCString(text), 0.f, false, false,
                        [this]
                        {
                            // FOCUS
                            mFocusArea->SetCellArea(mStruct->GetRow0(), mStruct->GetCol0(),
                                                    mStruct->GetRow1(), mStruct->GetCol1());
                            mFocusArea->SetCornersColorAction();
                            mFocusArea->SetBlinking(true);
                            mFocusArea->SetVisible(true);

                            // CLICK FILTER
                            GetClickFilter()->SetEnabled(false);
                        });
}

StepGameConquerStructFree::~StepGameConquerStructFree()
{
    delete mFocusArea;
}

void StepGameConquerStructFree::Update(float)
{
    if(mStruct->GetFaction() != NO_FACTION)
        SetDone();
    else if(mUnit != nullptr && mUnit->GetCurrentAction() == CONQUER_STRUCTURE)
    {
        mFocusArea->SetVisible(false);

        // hide info panel while conquest is in progress
        GetPanelInfo()->SetVisible(false);
    }
}

} // namespace game
