#include "Game.h"

#include "Screens/ScreenTest.h"

#include <graphic/Image.h>
#include <graphic/Renderer.h>
#include <graphic/Window.h>
#include <utilities/StateManager.h>

#include <iostream>

Game::Game(int argc, char * argv[])
    : lib::core::Application(argc, argv)
{
    mWin = lib::graphic::Window::Create("Iso RTS - v. 0.0.1", 960, 540);
    mRenderer = lib::graphic::Renderer::Create(mWin);

    // -- State Manager --
    mStateMan = new lib::utilities::StateManager;

    Screen * s = new ScreenTest(Screens::TEST, this);
    mStateMan->AddState(s);

    mStateMan->SetActiveState(Screens::TEST);
}

Game::~Game()
{
    delete mStateMan;

    lib::graphic::Renderer::Destroy();
    lib::graphic::Window::Destroy();
}

void Game::Update()
{
    mRenderer->SetRenderColor(0xEE, 0xEE, 0xEE, 0xFF);
    mRenderer->Clear();

    static_cast<Screen *>(mStateMan->GetActiveState())->Update();

    mRenderer->Finalize();
}
