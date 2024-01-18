#include "Screens/Screen.h"

#include "Game.h"
#include "Screens/SharedScreenListener.h"
#include "Widgets/DialogSettings.h"

#include <sgl/graphic/Camera.h>
#include <sgl/graphic/Renderer.h>

namespace game
{

Screen::Screen(Game * game)
    : mGame(game)
    , mSharedListener(new SharedScreenListener(game, this))
{
    mGame->AddKeyboardListener(mSharedListener);

    auto cam = sgl::graphic::Camera::GetDefaultCamera();
    cam->ClearFunctionOnMove();
    cam->ResetPosition();
}

Screen::~Screen()
{
    delete mSharedListener;
}

DialogSettings * Screen::ShowDialogSettings()
{
    using namespace sgl;

    // DIALOG
    const int screenW = graphic::Renderer::Instance()->GetWidth();
    const int screenH = graphic::Renderer::Instance()->GetHeight();

    mSettings = new DialogSettings(mGame);
    mSettings->SetFocus();
    mSettings->SetPosition((screenW - mSettings->GetWidth()) / 2,
                         (screenH - mSettings->GetHeight()) / 2);

    mSettings->AddOnCloseClickedFunction([this]
    {
        mSettings->DeleteLater();
        mSettings = nullptr;
    });

    return mSettings;
}

} // namespace game
