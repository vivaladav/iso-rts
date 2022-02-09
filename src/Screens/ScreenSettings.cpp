#include "Screens/ScreenSettings.h"

#include "Game.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"
#include "Widgets/GameUIData.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/Image.h>
#include <sgl/sgui/Label.h>
#include <sgl/sgui/Stage.h>

namespace game
{

ScreenSettings::ScreenSettings(Game * game)
    : Screen(game)
{
    using namespace sgl;

    const int screenW = graphic::Renderer::Instance()->GetWidth();
    const int screenH = graphic::Renderer::Instance()->GetHeight();

    mBg = new graphic::Image("data/img/space_bg.jpg");

    // BUTTON BACK
    const int marginButton = 10;
    auto btnBack = new ButtonMainMenu("BACK", nullptr);
    const int posBackX = screenW - btnBack->GetWidth() - marginButton;
    const int posBackY = screenH - btnBack->GetHeight() - marginButton;
    btnBack->SetPosition(posBackX, posBackY);

    btnBack->AddOnClickFunction([game]
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
