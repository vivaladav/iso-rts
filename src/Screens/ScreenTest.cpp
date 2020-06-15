#include "Screens/ScreenTest.h"

#include "Game.h"

#include <core/event/MouseButtonEvent.h>
#include <graphic/Font.h>
#include <graphic/Image.h>
#include <graphic/Text.h>

#include <iostream>

namespace game
{

ScreenTest::ScreenTest(Game * game)
    : Screen(game)
{
    using namespace lib::graphic;

    game->SetClearColor(0x22, 0x22, 0x22, 0xFF);

    // -- Image testing --
    mImg1 = new Image("data/img/tile01.png");

    mImg2 = new Image("data/img/tile01.png");
    mImg2->SetPosition(64, 32);

    mImg3 = new Image("data/img/tile01.png");
    mImg3->SetPosition(256, 128);
    mImg3->ScaleH(2.f);
    mImg3->SetRotation(45.f);

    // -- Text testing --
    mFont1 = new Font("data/fonts/OpenSans.ttf", 20);
    mTxt1 = new Text("Test text", mFont1);
    mTxt1->SetPosition(20, 300);
}

ScreenTest::~ScreenTest()
{
    delete mImg1;
    delete mImg2;
    delete mImg3;

    delete mFont1;
    delete mTxt1;
}

void ScreenTest::Update()
{
}

void ScreenTest::Render()
{
    mImg1->Render();
    mImg2->Render();
    mImg3->Render();

    mTxt1->Render();
}

void ScreenTest::OnMouseButtonUp(const lib::core::MouseButtonEvent & event)
{
    std::cout << "Mouse clicked - button: " << event.GetButton()
              << " - pos: " << event.GetX() << " , " << event.GetY() << std::endl;
}

} // namespace game
