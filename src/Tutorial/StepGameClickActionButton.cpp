#include "Tutorial/StepGameClickActionButton.h"

#include "Game.h"
#include "Widgets/PanelObjectActions.h"
#include "Widgets/Tutorial/FocusArea.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/sgui/AbstractButton.h>
#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameClickActionButton::StepGameClickActionButton(const Game * game, Screen * screen,
                                                     PanelObjectActions * panel, const char * text,
                                                     unsigned int buttonId, float animDelay)
    : TutorialInfoStep(screen, 550, 130)
    , mFocusArea(new FocusArea)
    , mPanelActions(panel)
    , mBtnId(buttonId)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // do not allow object selection during this step
    DisableObjectSelection();

    // FOCUS
    mFocusArea->SetCornersColorAction();
    mFocusArea->SetBlinking(true);
    mFocusArea->SetVisible(false);

    // INFO
    auto info = GetPanelInfo();

    const auto btnID = static_cast<PanelObjectActions::Button>(mBtnId);

    info->AddActionEntry(sm->GetCString(text), 0.f, false, false,
                         [this, panel, btnID, game, animDelay]
                        {
                            auto btn = panel->GetButton(btnID);

                            // CLICK FILTER
                            const int fX = btn->GetScreenX();
                            const int fY = btn->GetScreenY();
                            const int fW = btn->GetWidth();
                            const int fH = btn->GetHeight();

                            auto cf = GetClickFilter();
                            cf->SetButtonToAllow(game->GetButtonSelect());
                            cf->SetScreenClickableArea(fX, fY, fW, fH);

                            // FOCUS
                            const int padding = 10;
                            const int f2X = fX - padding;
                            const int f2Y = fY - padding;
                            const int f2W = fW + (padding * 2);
                            const int f2H = fH + (padding * 2);

                            mFocusArea->SetScreenArea(f2X, f2Y, f2W, f2H, true, animDelay);
                            mFocusArea->SetVisible(true);

                            mShowingFocus = true;
                        });

    mClickId = panel->AddButtonFunction(btnID, [this]
    {
        SetDone();
    });
}

StepGameClickActionButton::~StepGameClickActionButton()
{
    mPanelActions->RemoveButtonFunction(static_cast<PanelObjectActions::Button>(mBtnId), mClickId);

    delete mFocusArea;
}

void StepGameClickActionButton::SetPause(bool paused)
{
    TutorialInfoStep::SetPause(paused);

    if(mShowingFocus)
        mFocusArea->SetVisible(!paused);
}

} // namespace game
