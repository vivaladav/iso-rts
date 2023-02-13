#include "Screens/ScreenSettings.h"

#include "Game.h"
#include "States/StatesIds.h"
#include "Widgets/DialogSettings.h"

#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/Stage.h>

namespace game
{

ScreenSettings::ScreenSettings(Game * game)
    : Screen(game)
{
    using namespace sgl;

    const int screenW = graphic::Renderer::Instance()->GetWidth();
    const int screenH = graphic::Renderer::Instance()->GetHeight();

    //  BACKGROUND
    auto tm = graphic::TextureManager::Instance();
    graphic::Texture * tex = tm->GetTexture("space_bg.jpg");
    mBg = new graphic::Image(tex);

    // DIALOG
    mDialog = new DialogSettings();
    mDialog->SetPosition((screenW - mDialog->GetWidth()) / 2,
                         (screenH - mDialog->GetHeight()) / 2);

    mDialog->AddOnBackClickFunction([game]
    {
        game->RequestNextActiveState(StateId::MAIN_MENU);
    });
}

ScreenSettings::~ScreenSettings()
{
    sgl::sgui::Stage::Instance()->ClearWidgets();

    delete mBg;
}

void ScreenSettings::Update(float update)
{
}

void ScreenSettings::Render()
{
    mBg->Render();
}

} // namespace game
