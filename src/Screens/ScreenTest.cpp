#include "Screens/ScreenTest.h"

#include "Game.h"
#include "Screens/ScreenIds.h"

#include <core/event/KeyboardEvent.h>
#include <core/event/MouseButtonEvent.h>
#include <graphic/Image.h>

#include <iostream>

namespace game
{

ScreenTest::ScreenTest(Game * game)
    : Screen(ScreenId::TEST, game)
{
    // -- Image testing --
    mImg1 = new lib::graphic::Image("data/img/tile01.png");

    mImg2 = new lib::graphic::Image("data/img/tile01.png");
    mImg2->SetPosition(128, 64);

    mImg3 = new lib::graphic::Image("data/img/tile01.png");
    mImg3->SetPosition(256, 128);
    mImg3->ScaleH(2.f);
    mImg3->SetRotation(45.f);
}

ScreenTest::~ScreenTest()
{
    delete mImg1;
    delete mImg2;
    delete mImg3;
}

void ScreenTest::Update()
{
    mImg1->Render();
    mImg2->Render();
    mImg3->Render();
}

void ScreenTest::OnApplicationQuit() { mGame->Exit(); }

void ScreenTest::OnKeyUp(const lib::core::KeyboardEvent & event)
{
    if(event.GetKey() == lib::core::KeyboardEvent::KEY_ESC)
        mGame->Exit();
}

void ScreenTest::OnMouseButtonUp(const lib::core::MouseButtonEvent & event)
{
    std::cout << "Mouse clicked - button: " << event.GetButton()
              << " - pos: " << event.GetX() << " , " << event.GetY() << std::endl;
}

} // namespace game
