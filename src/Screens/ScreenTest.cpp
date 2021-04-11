#include "Screens/ScreenTest.h"

#include "Game.h"
#include "Player.h"
#include "Widgets/ButtonMainMenu.h"
#include "Widgets/ButtonUnitsSelector.h"
#include "Widgets/CellProgressBar.h"

#include <core/event/MouseButtonEvent.h>
#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/Text.h>
#include <graphic/Texture.h>
#include <graphic/TextureManager.h>
#include <sgui/ButtonsGroup.h>
#include <sgui/Label.h>
#include <sgui/Stage.h>

#include <iostream>

namespace game
{

const float TIME_PB = 4.f;

ScreenTest::ScreenTest(Game * game)
    : Screen(game)
{
    using namespace lib::graphic;

    game->SetClearColor(0x22, 0x22, 0x22, 0xFF);

    // -- Image testing --
    Image * img = nullptr;

    img = new Image("data/img/obj_null.png");
    img->SetPosition(64, 32);
    mRenderables.emplace_back(img);

    img = new Image("data/img/obj_null.png");
    img->SetPosition(128, 64);
    img->SetAlpha(128);
    mRenderables.emplace_back(img);

    img = new Image("data/img/obj_null.png");
    img->SetPosition(300, 32);
    img->ScaleH(2.f);
    img->SetRotation(45.f);
    img->SetColor(0x3366F0FF);
    mRenderables.emplace_back(img);

    img = new Image("data/img/obj_null.png");
    img->SetPosition(32, 64);
    img->SetWidth(64);
    img->SetHeight(32);
    mRenderables.emplace_back(img);

    // -- Text testing --
    FontManager * fm = FontManager::Instance();

    const int TXT_X0 = 20;
    Font * font = fm->GetFont("data/fonts/OpenSans.ttf", 24, Font::NORMAL);
    Text * txt = nullptr;

    txt = new Text("Text 1", font);
    txt->SetPosition(TXT_X0, 250);
    mRenderables.emplace_back(txt);

    font = fm->GetFont("data/fonts/OpenSans.ttf", 24, Font::BOLD | Font::ITALIC);
    txt = new Text("Text 2 (bold & italic)", font);
    txt->SetPosition(TXT_X0, 300);
    mRenderables.emplace_back(txt);

    font = fm->GetFont("data/fonts/OpenSans.ttf", 24, Font::BOLD);
    txt = new Text("Text 3 (bold - A=128)", font);
    txt->SetPosition(TXT_X0, 350);
    txt->SetAlpha(128);
    mRenderables.emplace_back(txt);

    font = fm->GetFont("data/fonts/OpenSans.ttf", 24, Font::NORMAL);
    txt = new Text("Text 4 (50, 150, 250, 255)", font);
    txt->SetPosition(TXT_X0, 400);
    txt->SetColor(50, 150, 250, 255);
    mRenderables.emplace_back(txt);

    txt = new Text("Text 5 (50, 150, 250, 128)", font);
    txt->SetPosition(TXT_X0, 450);
    txt->SetColor(50, 150, 250, 128);
    mRenderables.emplace_back(txt);

    TestSprite();

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
    if(mTimerPb < TIME_PB)
    {
        mTimerPb += delta;

        const float perc = mTimerPb * 100.f / TIME_PB;

        mPb0->SetValuePerc(perc);
        mPb1->SetValuePerc(perc);
        mPb2->SetValuePerc(perc);
    }
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
    Font * font = fm->GetFont("data/fonts/OpenSans.ttf", 24, Font::BOLD);
    Label * label = new Label("PUSH BUTTON", font, container);

    ButtonMainMenu * button = new ButtonMainMenu("BUTTON 1", container);
    button->SetY(label->GetHeight() + 20);

    font = fm->GetFont("data/fonts/OpenSans.ttf", 24, Font::NORMAL);
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

    button = new ButtonMainMenu("SELECTED", container);
    button->SetCheckable(true);
    button->SetY(buttonY);

    buttonY += button->GetHeight() + marginV;

    // testing change of Texture in graphic::Image
    Image * img = new Image;
    Texture * tex = TextureManager::Instance()->GetTexture("data/img/base-f1.png");
    img->SetTexture(tex);
    img->SetPosition(container->GetX(), buttonY);
    mRenderables.emplace_back(img);

    button->SetOnToggleFunction([img](bool checked)
    {
        Texture * tex;

        if(checked)
            tex = TextureManager::Instance()->GetTexture("data/img/base-f1-sel.png");
        else
            tex = TextureManager::Instance()->GetTexture("data/img/base-f1.png");

        img->SetTexture(tex);
    });

    // -- BUTTONS GROUP --
    font = fm->GetFont("data/fonts/OpenSans.ttf", 24, Font::BOLD);

    label = new Label("BUTTONS GROUP", font, container);
    label->SetX(400);

    ButtonsGroup * bg = new ButtonsGroup(ButtonsGroup::HORIZONTAL, container);
    bg->SetPosition(label->GetX(), label->GetHeight() + 50);

    bg->AddButton(new ButtonUnitsSelector("1"));
    bg->AddButton(new ButtonUnitsSelector("2"));
    bg->AddButton(new ButtonUnitsSelector("3"));
    bg->AddButton(new ButtonUnitsSelector("4"));

    // set initial checked
    bg->SetButtonChecked(0, true);

    // test set checked on disabled
    bg->SetButtonEnabled(3, false);
    bg->SetButtonChecked(3, true);

    // -- PROGRESS BAR --
    font = fm->GetFont("data/fonts/OpenSans.ttf", 24, Font::BOLD);

    label = new Label("PROGRESS BAR", font, container);
    label->SetX(700);

    const int pbX = label->GetX();
    const int pbY = label->GetY() + label->GetHeight() + 50;
    const int marginbBars = 25;

    mPb0 = new CellProgressBar(FACTION_1, container);
    mPb0->SetPosition(pbX, pbY);
    mPb0->SetValue(0);
    mPb0->SetFunctionOnCompleted([]{ std::cout << "TEST PROGRESS 0 100%" << std::endl; });

    mPb1 = new CellProgressBar(FACTION_2, container);
    mPb1->SetPosition(pbX, pbY + mPb0->GetHeight() + marginbBars);
    mPb1->SetValue(0);
    mPb1->SetFunctionOnCompleted([]{ std::cout << "TEST PROGRESS 1 100%" << std::endl; });

    mPb2 = new CellProgressBar(FACTION_3, container);
    mPb2->SetPosition(pbX, mPb1->GetY() + mPb1->GetHeight() + marginbBars);
    mPb2->SetValue(0);
    mPb2->SetFunctionOnCompleted([]{ std::cout << "TEST PROGRESS 2 100%" << std::endl; });

    button = new ButtonMainMenu("RESET", container);
    button->SetPosition(mPb0->GetX(), mPb2->GetY() + mPb2->GetHeight() + marginbBars);
    button->SetOnClickFunction([this]{
        mTimerPb = 0;
        mPb0->Reset();
        mPb1->Reset();
        mPb2->Reset();
    });
}

void ScreenTest::TestSprite()
{
    using namespace lib::graphic;

    auto tm = TextureManager::Instance();

    const std::vector<lib::core::Rectd> rects
    {
        { 0, 0, 40, 40 },
        { 40, 0, 40, 40 },
        { 0, 40, 40, 40 },
        { 40, 40, 40, 40 }
    };

    const unsigned int numRects = rects.size();

    const char file[] = "data/img/test_sprite.png";

    tm->RegisterSprite(file, rects);

    const int x0 = 20;
    const int y0 = 550;
    const int marginH = 20;
    const int marginV = 20;

    Image * img = nullptr;

    img = new Image(tm->GetTexture(file));
    mRenderables.emplace_back(img);

    img->SetPosition(x0, y0);

    const int x1 = x0 + img->GetWidth() + marginH;

    // row 1
    int x = x1;

    for(unsigned int i = 0; i < numRects; ++i)
    {
        img = new Image(tm->GetSprite(file, i));
        img->SetPosition(x, y0);
        mRenderables.emplace_back(img);

        x += img->GetWidth() + marginH;
    }

    // row 2
    x = x1;
    const int y1 = y0 + img->GetHeight() + marginV;

    for(unsigned int i = 0; i < numRects; ++i)
    {
        img = new Image(tm->GetSprite(file, numRects - i - 1));
        img->SetPosition(x, y1);
        mRenderables.emplace_back(img);

        x += img->GetWidth() + marginH;
    }
}

} // namespace game
