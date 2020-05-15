#include "Game.h"

#include <core/event/EventListener.h>
#include <core/event/KeyboardEvent.h>
#include <graphic/Renderer.h>
#include <graphic/Window.h>

class TestListener : public lib::core::EventListener
{
public:
    TestListener(lib::core::Application * app) : mApp(app) {}
    // -- application events --
    void OnApplicationQuit() { mApp->Exit(); }

    // -- keyboard events --
    void OnKeyUp(const lib::core::KeyboardEvent & event)
    {
        if(event.GetKey() == lib::core::KeyboardEvent::KEY_ESC)
            mApp->Exit();
    }

private:
    lib::core::Application * mApp = nullptr;
};

Game::Game(int argc, char * argv[])
    : lib::core::Application(argc, argv)
{
    mListener = new TestListener(this);
    SetEventListener(mListener);

    mWin = new lib::graphic::Window("Iso RTS - v. 0.0.1", 1600, 900);

    mRenderer = new lib::graphic::Renderer(mWin);
}

Game::~Game()
{
    delete mListener;
    delete mRenderer;
    delete mWin;
}

void Game::Update()
{
    mRenderer->SetRenderColor(0xEE, 0xEE, 0xEE, 0xFF);
    mRenderer->Clear();

    mRenderer->Finalize();
}
