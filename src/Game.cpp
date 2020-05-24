#include "Game.h"

#include "Screens/ScreenIds.h"
#include "Screens/ScreenGame.h"
#include "Screens/ScreenMainMenu.h"
#include "Screens/ScreenTest.h"

#include <graphic/Image.h>
#include <graphic/Renderer.h>
#include <graphic/Window.h>
#include <utilities/StateManager.h>

#include <iostream>

namespace game
{

Game::Game(int argc, char * argv[])
    : lib::core::Application(argc, argv)
{
    mWin = lib::graphic::Window::Create("Iso RTS - v. 0.0.1", 1280, 720);
    mRenderer = lib::graphic::Renderer::Create(mWin);
    mRenderer->SetLogicalSize(1920, 1080);

    // -- State Manager --
    mStateMan = new lib::utilities::StateManager;

    mStateMan->AddState(new ScreenGame(this));
    mStateMan->AddState(new ScreenTest(this));
    mStateMan->AddState(new ScreenMainMenu(this));

    mStateMan->RequestNextActiveState(ScreenId::MAIN_MENU);
}

Game::~Game()
{
    delete mStateMan;

    lib::graphic::Renderer::Destroy();
    lib::graphic::Window::Destroy();
}

void Game::Update()
{
    mRenderer->Clear(mClearR, mClearG, mClearB, mClearA);

    mStateMan->UpdateActive();

    Screen * screen = static_cast<Screen *>(mStateMan->GetActiveState());
    screen->Update();
    screen->Render();

    mRenderer->Finalize();
}

} // namespace game
