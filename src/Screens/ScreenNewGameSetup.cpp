#include "Screens/ScreenNewGameSetup.h"

#include "Widgets/DialogNewGame.h"

#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/Stage.h>

namespace game
{

ScreenNewGameSetup::ScreenNewGameSetup(Game * game)
    : Screen(game)
{
    using namespace sgl;

    const int screenW = graphic::Renderer::Instance()->GetWidth();
    const int screenH = graphic::Renderer::Instance()->GetHeight();

    auto tm = graphic::TextureManager::Instance();

    // BACKGROUND
    auto tex = tm->GetTexture("space_bg.jpg");
    mBg = new graphic::Image(tex);

    // DIALOG
    auto dialog = new DialogNewGame(game);
    dialog->SetPosition((screenW - dialog->GetWidth()) / 2,
                        (screenH - dialog->GetHeight()) / 2);

    dialog->SetFocus();
}

ScreenNewGameSetup::~ScreenNewGameSetup()
{
    sgl::sgui::Stage::Instance()->ClearWidgets();

    delete mBg;
}

void ScreenNewGameSetup::Update(float delta) { }

void ScreenNewGameSetup::Render()
{
    mBg->Render();
}

} // namespace game
