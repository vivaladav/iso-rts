#include "Game.h"

#include <core/event/EventListener.h>
#include <core/event/KeyboardEvent.h>
#include <core/event/MouseButtonEvent.h>
#include <core/event/MouseMotionEvent.h>
#include <graphic/Image.h>
#include <graphic/Renderer.h>
#include <graphic/Window.h>

#include <iostream>

class TestListener : public lib::core::EventListener
{
public:
    TestListener(lib::core::Application * app) : mApp(app) {}
    // -- application events --
    void OnApplicationQuit() override { mApp->Exit(); }

    // -- keyboard events --
    void OnKeyUp(const lib::core::KeyboardEvent & event) override
    {
        if(event.GetKey() == lib::core::KeyboardEvent::KEY_ESC)
            mApp->Exit();
    }

    void OnMouseButtonUp(const lib::core::MouseButtonEvent & event) override
    {
        std::cout << "Mouse clicked - button: " << event.GetButton()
                  << " - pos: " << event.GetX() << " , " << event.GetY() << std::endl;
    }

//    void OnMouseMotion(const lib::core::MouseMotionEvent & event) override
//    {
//        std::cout << "Mouse clicked - pos: " << event.GetX() << " , " << event.GetY()
//                  << " - is LEFT Button down: " << (event.IsButtonPushed(lib::core::MouseEvent::BUTTON_LEFT) ? "Y" : "N") << std::endl;
//    }

private:
    lib::core::Application * mApp = nullptr;
};

Game::Game(int argc, char * argv[])
    : lib::core::Application(argc, argv)
{
    mListener = new TestListener(this);
    SetEventListener(mListener);

    mWin = lib::graphic::Window::Create("Iso RTS - v. 0.0.1", 960, 540);
    mRenderer = lib::graphic::Renderer::Create(mWin);

    // -- Image testing --
    mImg1 = new lib::graphic::Image("data/img/tile01.png");

    mImg2 = new lib::graphic::Image("data/img/tile01.png");
    mImg2->SetPosition(128, 64);

    mImg3 = new lib::graphic::Image("data/img/tile01.png");
    mImg3->SetPosition(256, 128);
    mImg3->ScaleH(2.f);
    mImg3->SetRotation(45.f);
}

Game::~Game()
{
    delete mListener;

    delete mImg1;
    delete mImg2;
    delete mImg3;

    lib::graphic::Renderer::Destroy();
    lib::graphic::Window::Destroy();
}

void Game::Update()
{
    mRenderer->SetRenderColor(0xEE, 0xEE, 0xEE, 0xFF);
    mRenderer->Clear();

    mImg1->Render();
    mImg2->Render();
    mImg3->Render();

    mRenderer->Finalize();
}
