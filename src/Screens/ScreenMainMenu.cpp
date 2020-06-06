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
    using namespace lib::graphic;
    using namespace lib::sgui;

    mStage = Stage::Create();
    game->AddMouseListener(mStage);

    int buttonId = 10;

    Widget * container = new Widget(0);

    int buttonY = 0;
    const int VMARGIN = 100;

    // -- BUTTON NEW GAME --
    mButtonNew = new PushButton(buttonId++, container);
    mButtonNew->SetBackground("data/img/buttons/new_game-01.png");

    mButtonNew->SetOnClickFunction([this] { GetStateManager()->RequestNextActiveState(ScreenId::GAME); });

    buttonY += mButtonNew->GetHeight() + VMARGIN;

    // -- BUTTON EXIT --
    mButtonExit = new PushButton(buttonId++, container);
    mButtonExit->SetBackground("data/img/buttons/exit-01.png");

    mButtonExit->SetOnClickFunction([this] { GetGame()->Exit(); });

    mButtonExit->SetY(buttonY);

    // position buttons panel
    const int containerX = (Renderer::Instance()->GetWidth() - mButtonNew->GetWidth()) * 0.5f;
    const int containerY = 300;
    container->SetPosition(containerX, containerY);
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
