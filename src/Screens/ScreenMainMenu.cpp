#include "Screens/ScreenMainMenu.h"

#include "Game.h"
#include "Screens/ScreenIds.h"

#include <graphic/Renderer.h>
#include <sgui/PushButton.h>
#include <sgui/Stage.h>
#include <utilities/StateManager.h>

namespace game
{

ScreenMainMenu::ScreenMainMenu(Game * game)
    : Screen(ScreenId::MAIN_MENU, game)
{
    using namespace lib::sgui;

    mStage = Stage::Create();
    game->AddMouseListener(mStage);

    const int VMARGIN = 100;
    int buttonId = 0;

    // -- BUTTON NEW GAME --
    mButtonNew = new lib::sgui::PushButton(buttonId++);
    mButtonNew->SetBackground("data/img/buttons/new_game-01.png");

    const int buttonX = (lib::graphic::Renderer::Instance()->GetWidth() - mButtonNew->GetWidth()) * 0.5f;
    int buttonY = 300;

    mButtonNew->SetPosition(buttonX, buttonY);

    mButtonNew->SetOnClickFunction([this] { GetStateManager()->RequestNextActiveState(ScreenId::GAME); });

    buttonY += mButtonNew->GetHeight() + VMARGIN;

    // -- BUTTON EXIT --
    mButtonExit = new lib::sgui::PushButton(buttonId++);
    mButtonExit->SetBackground("data/img/buttons/exit-01.png");

    mButtonExit->SetPosition(buttonX, buttonY);

    mButtonExit->SetOnClickFunction([this] { GetGame()->Exit(); });
}

ScreenMainMenu::~ScreenMainMenu()
{
    lib::sgui::Stage::Destroy();
}

void ScreenMainMenu::Update()
{
}

void ScreenMainMenu::Render()
{
    mStage->Render();
}

void ScreenMainMenu::OnActive()
{
    Screen::OnActive();

    GetGame()->SetClearColor(0x11, 0x11, 0x11, 0xFF);
}

} // namespace game
