#include "Tutorial/StepGameMoveToCellFree.h"

#include "GameMap.h"
#include "IsoMap.h"
#include "GameObjects/GameObject.h"
#include "Widgets/Tutorial/IsoFocusArea.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameMoveToCellFree::StepGameMoveToCellFree(const GameMap * gm, const IsoMap * isoMap,
                                               const char * text, const Cell2D & target,
                                               const sgl::core::Pointd2D & p0)
    : TutorialInfoStep(550, 130)
    , mFocusArea(new IsoFocusArea(isoMap))
    , mGameMap(gm)
    , mTarget(target)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // FOCUS
    mFocusArea->SetVisible(false);

    // INFO
    auto info = GetPanelInfo();

    info->SetPosition(p0.x, p0.y);

    info->AddActionEntry(sm->GetCString(text), 0.f, false, false, [this, isoMap, gm]
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

StepGameMoveToCellFree::~StepGameMoveToCellFree()
{
    delete mFocusArea;
}

void StepGameMoveToCellFree::Update(float)
{
    auto obj = mGameMap->GetObject(mTarget.row, mTarget.col);

    if(obj != nullptr && obj->GetObjectCategory() == ObjectData::CAT_UNIT)
        SetDone();
}

} // namespace game
