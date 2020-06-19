#include "Screens/ScreenTest.h"

#include "Game.h"

#include <core/event/MouseButtonEvent.h>
#include <graphic/Font.h>
#include <graphic/Image.h>
#include <graphic/Text.h>
#include <sgui/Label.h>
#include <sgui/PushButton.h>
#include <sgui/Stage.h>

#include <iostream>

namespace game
{

ScreenTest::ScreenTest(Game * game)
    : Screen(game)
{
    using namespace lib::graphic;

    game->SetClearColor(0x22, 0x22, 0x22, 0xFF);

    // -- Image testing --
    Image * img = nullptr;

    img = new Image("data/img/tile00.png");
    img->SetPosition(64, 32);
    mRenderables.emplace_back(img);

    img = new Image("data/img/tile00.png");
    img->SetPosition(128, 64);
    img->SetAlpha(128);
    mRenderables.emplace_back(img);

    img = new Image("data/img/tile00.png");
    img->SetPosition(300, 32);
    img->ScaleH(2.f);
    img->SetRotation(45.f);
    img->SetColor(50, 150, 250, 255);
    mRenderables.emplace_back(img);

    // -- Text testing --
    const int TXT_X0 = 20;
    Font * font = new Font("data/fonts/OpenSans.ttf", 24);
    Text * txt = nullptr;

    txt = new Text("Text 1", font);
    txt->SetPosition(TXT_X0, 250);
    mRenderables.emplace_back(txt);

    font->SetStyle(Font::BOLD | Font::ITALIC);
    txt = new Text("Text 2 (bold & italic)", font);
    txt->SetPosition(TXT_X0, 300);
    mRenderables.emplace_back(txt);

    font->SetStyle(Font::BOLD);
    txt = new Text("Text 3 (bold - A=128)", font);
    txt->SetPosition(TXT_X0, 350);
    txt->SetAlpha(128);
    mRenderables.emplace_back(txt);

    font->SetStyle(Font::NORMAL);
    txt = new Text("Text 4 (50, 150, 250, 255)", font);
    txt->SetPosition(TXT_X0, 400);
    txt->SetColor(50, 150, 250, 255);
    mRenderables.emplace_back(txt);

    txt = new Text("Text 5 (50, 150, 250, 128)", font);
    txt->SetPosition(TXT_X0, 450);
    txt->SetColor(50, 150, 250, 128);
    mRenderables.emplace_back(txt);

    TestSGui();
}

ScreenTest::~ScreenTest()
{
    for(lib::graphic::TexturedRenderable * r : mRenderables)
        delete r;

    lib::sgui::Stage::Instance()->ClearWidgets();
}

void ScreenTest::Update()
{
}

void ScreenTest::Render()
{
    for(lib::graphic::TexturedRenderable * r : mRenderables)
        r->Render();
}

void ScreenTest::OnMouseButtonUp(const lib::core::MouseButtonEvent & event)
{
    std::cout << "Mouse clicked - button: " << event.GetButton()
              << " - pos: " << event.GetX() << " , " << event.GetY() << std::endl;
}

void ScreenTest::TestSGui()
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    int wid = 0;

    Widget * container = new Widget(wid++);
    container->SetPosition(600, 20);

    Font * font = new Font("data/fonts/OpenSans.ttf", 24);
    font->SetStyle(Font::BOLD);
    Label * label = new Label("PUSH BUTTON", wid++, font, container);

    PushButton * button = new PushButton(wid++, container);
    button->SetBackground("data/img/buttons/main_menu-bg-01.png");
    button->SetLabel("BUTTON 1");
    button->SetLabelColor(24, 57, 41, 255);
    button->SetY(label->GetHeight() + 20);

    font->SetStyle(Font::NORMAL);
    label = new Label(wid++, font, container);
    label->SetY(button->GetY() + button->GetHeight() + 10);

    button->SetOnClickFunction([label]{ label->SetText("button clicked"); });
}

} // namespace game
