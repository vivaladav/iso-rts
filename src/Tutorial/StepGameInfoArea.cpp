#include "Tutorial/StepGameInfoArea.h"

#include "IsoMap.h"
#include "Widgets/Tutorial/IsoFocusArea.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameInfoArea::StepGameInfoArea(const IsoMap * isoMap, const Cell2D & areaTL,
                                   const Cell2D & areaBR, const char * text1, const char * text2,
                                   const sgl::core::Pointd2D & p0)
    : TutorialInfoStep(550, 150)
    , mFocusArea(new IsoFocusArea(isoMap))
{
    auto sm = sgl::utilities::StringManager::Instance();

    // FOCUS
    mFocusArea->SetCellArea(areaBR.row, areaBR.col, areaTL.row, areaTL.col);
    mFocusArea->SetCornersColorElement();
    mFocusArea->SetVisible(false);

    // INFO
    auto info = GetPanelInfo();

    info->SetPosition(p0.x, p0.y);

    if(text1 != nullptr)
        info->AddInfoEntry(sm->GetCString(text1), 10.f, true, true);

    if(text2 != nullptr)
        info->AddInfoEntry(sm->GetCString(text2), 10.f, true, false);

    info->SetFunctionOnFinished([this]
    {
        SetDone();
    });
}

StepGameInfoArea::~StepGameInfoArea()
{
    delete mFocusArea;
}

void StepGameInfoArea::OnStart()
{
    TutorialInfoStep::OnStart();

    mFocusArea->SetVisible(true);
}

} // namespace game
