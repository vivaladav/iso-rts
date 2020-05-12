#include "Game.h"

#include <graphic/Renderer.h>
#include <graphic/Window.h>

Game::Game(int argc, char * argv[])
    : lib::core::Application(argc, argv)
{
    mWin = new lib::graphic::Window("Iso RTS - v. 0.0.1", 1600, 900);

    mRenderer = new lib::graphic::Renderer(mWin);
}

Game::~Game()
{
    delete mRenderer;
    delete mWin;
}

void Game::Update()
{
    mRenderer->SetRenderColor(0xEE, 0xEE, 0xEE, 0xFF);
    mRenderer->Clear();

    mRenderer->Finalize();
}
