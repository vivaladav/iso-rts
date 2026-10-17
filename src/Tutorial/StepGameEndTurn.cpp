#include "Tutorial/StepGameEndTurn.h"

#include "Widgets/PanelTurnControl.h"
#include "Widgets/Tutorial/FocusArea.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/sgui/AbstractButton.h>
#include <sgl/sgui/ButtonsGroup.h>
#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameEndTurn::StepGameEndTurn(const PanelTurnControl * panel)
    : TutorialInfoStep(600, 160)
    , mFocusArea(new FocusArea)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // FOCUS
    mFocusArea->SetVisible(false);
    mFocusArea->SetBlinking(true);
    mFocusArea->SetCornersColorAction();

    // INFO
    auto info = GetPanelInfo();

    info->SetPosition(1250, 650);

    info->AddInfoEntry(sm->GetCString("TUT_GAME_END_TURN_1"), 10.f, true, false);

    info->AddActionEntry(sm->GetCString("TUT_GAME_END_TURN_2"), 0.f, false, false, [this, panel]
                        {
                            // CLICK FILTER
                            const int fX = mButton->GetScreenX();
                            const int fY = mButton->GetScreenY();
                            const int fW = mButton->GetWidth();
                            const int fH = mButton->GetHeight();

                            GetClickFilter()->SetScreenClickableArea(fX, fY, fW, fH);

                            // FOCUS
                            const int padding = 10;
                            const int f2X = fX - padding;
                            const int f2Y = fY - padding;
                            const int f2W = fW + (padding * 2);
                            const int f2H = fH + (padding * 2);

                            mFocusArea->SetScreenArea(f2X, f2Y, f2W, f2H, true);
                            mFocusArea->SetVisible(true);

                            mShowingFocus = true;
                        });

    mButton = panel->GetButtonEndTurn();

    mFuncId = mButton->AddOnClickFunction([this]
    {
        SetDone();
    });
}

StepGameEndTurn::~StepGameEndTurn()
{
    delete mFocusArea;

    // clear callback
    mButton->RemoveClickFunction(mFuncId);
}

void StepGameEndTurn::SetPause(bool paused)
{
    TutorialInfoStep::SetPause(paused);

    if(mShowingFocus)
        mFocusArea->SetVisible(!paused);
}

} // namespace game
