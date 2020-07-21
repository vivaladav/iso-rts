#include "Screens/ScreenTest.h"

#include "Game.h"
#include "Widgets/ButtonMainMenu.h"
#include "Widgets/ButtonUnitsSelector.h"

#include <core/event/MouseButtonEvent.h>
#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/Text.h>
#include <sgui/ButtonsGroup.h>
#include <sgui/Label.h>
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
    img->SetColor(0x3366F0FF);
    mRenderables.emplace_back(img);

    // -- Text testing --
    FontManager * fm = FontManager::Instance();

    const int TXT_X0 = 20;
    Font * font = fm->GetFont("data/fonts/OpenSans.ttf", 24);
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

void ScreenTest::Update(float delta)
{
}

void ScreenTest::Render()
{
    for(lib::graphic::TexturedRenderable * r : mRenderables)
        r->Render();
}

void ScreenTest::OnMouseButtonUp(lib::core::MouseButtonEvent & event)
{
    std::cout << "Mouse clicked - button: " << event.GetButton()
              << " - pos: " << event.GetX() << " , " << event.GetY() << std::endl;
}

void ScreenTest::TestSGui()
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    const int marginV = 50;

    Widget * container = new Widget;
    container->SetPosition(600, 20);

    FontManager * fm = FontManager::Instance();

    // -- PUSHBUTTON --
    Font * font = fm->GetFont("data/fonts/OpenSans.ttf", 24);
    font->SetStyle(Font::BOLD);
    Label * label = new Label("PUSH BUTTON", font, container);

    ButtonMainMenu * button = new ButtonMainMenu("BUTTON 1", container);
    button->SetY(label->GetHeight() + 20);

    font->SetStyle(Font::NORMAL);
    label = new Label(font, container);
    label->SetY(button->GetY() + button->GetHeight() + 10);

    button->SetOnClickFunction([label]
    {
        label->SetText("button 1 clicked");
        std::cout << "button 1 clicked" << std::endl;
    });

    button = new ButtonMainMenu("BUTTON 2", container);
    button->SetY(label->GetY() + marginV);

    button->SetOnClickFunction([label]
    {
        label->SetText("button 2 clicked");
        std::cout << "button 2 clicked" << std::endl;
    });

    int buttonY = button->GetY() + button->GetHeight() + marginV;

    button = new ButtonMainMenu("BUTTON 3", container);
    button->SetEnabled(false);
    button->SetY(buttonY);

    buttonY += button->GetHeight() + marginV;

    button = new ButtonMainMenu("TOGGLE 1", container);
    button->SetCheckable(true);
    button->SetY(buttonY);

    // -- BUTTONS GROUP --
    font->SetStyle(Font::BOLD);

    label = new Label("BUTTONS GROUP", font, container);
    label->SetX(400);

    ButtonsGroup * bg = new ButtonsGroup(ButtonsGroup::HORIZONTAL, container);
    bg->SetPosition(label->GetX(), label->GetHeight() + 50);

    bg->AddButton(new ButtonUnitsSelector("1"));
    bg->AddButton(new ButtonUnitsSelector("2"));
    bg->AddButton(new ButtonUnitsSelector("3"));
    bg->AddButton(new ButtonUnitsSelector("4"));

    bg->SetChecked(0);

    bg->GetButton(3)->SetEnabled(false);
}

} // namespace game
