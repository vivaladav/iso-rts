#include "Tutorial/StepGameConnectStructFree.h"

#include "Game.h"
#include "IsoMap.h"
#include "GameObjects/GameObject.h"
#include "Widgets/Tutorial/IsoFocusArea.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameConnectStructFree::StepGameConnectStructFree(const Game * game, const GameObject * obj,
                                                     const IsoMap * isoMap, const char * text,
                                                     const sgl::core::Pointd2D & p0)
    : TutorialInfoStep(550, 150)
    , mFocusArea(new IsoFocusArea(isoMap))
    , mStruct(obj)
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

StepGameConnectStructFree::~StepGameConnectStructFree()
{
    delete mFocusArea;
}

void StepGameConnectStructFree::Update(float)
{
    if(mStruct->IsLinked())
        SetDone();
}

} // namespace game
