#include "Screens/ScreenSettings.h"

#include "Game.h"
#include "States/StatesIds.h"
#include "Widgets/GameButton.h"
#include "Widgets/GameUIData.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/Image.h>
#include <sgl/sgui/Label.h>
#include <sgl/sgui/Stage.h>

namespace game
{
// ====== BUTTON BACK =====
class ButtonBack : public GameButton
{
public:
    ButtonBack(sgl::sgui::Widget * parent)
        : GameButton(SpriteFileSettings,
                     {
                         IND_SET_BTN_NORMAL,
                         IND_SET_BTN_DISABLED,
                         IND_SET_BTN_OVER,
                         IND_SET_BTN_PUSHED,
                         IND_SET_BTN_NORMAL
                      },
                      {
                         0xdce3e8ff,
                         0x506773b2,
                         0xe6edf2ff,
                         0xced4d9ff,
                         0xdce3e8ff
                      },
                     parent)
    {
        using namespace sgl::graphic;

        // set label font
        auto fm = FontManager::Instance();
        Font * font = fm->GetFont("data/fonts/Lato-Regular.ttf", 24, Font::NORMAL);

        SetLabelFont(font);

        SetLabel("BACK");
    }
};

// ====== PANEL CONTENT ======
class PanelContentSettings : public sgl::sgui::Widget
{
public:
    PanelContentSettings(int h, sgl::sgui::Widget * parent)
        : sgl::sgui::Widget(parent)
    {
        using namespace sgl;

        auto tm = graphic::TextureManager::Instance();
        graphic::Texture * tex;

        tex = tm->GetSprite(SpriteFileSettingsExp, IND_SET_PANEL2_TOP);
        mImgTop = new graphic::Image(tex);
        RegisterRenderable(mImgTop);

        tex = tm->GetSprite(SpriteFileSettingsExp, IND_SET_PANEL2_BOTTOM);
        mImgBot = new graphic::Image(tex);
        RegisterRenderable(mImgBot);

        tex = tm->GetSprite(SpriteFileSettingsExp, IND_SET_PANEL2_MID);
        tex->SetScaleMode(0);
        mImgMid = new graphic::Image(tex);
        mImgMid->SetHeight(h - mImgTop->GetHeight() - mImgBot->GetHeight());
        RegisterRenderable(mImgMid);

        SetSize(mImgTop->GetWidth(), h);

        UpdatePositions();
    }

private:
    void HandlePositionChanged() override
    {
        UpdatePositions();
    }

    void UpdatePositions()
    {
        const int x0 = GetScreenX();
        const int y0 = GetScreenY();

        // BACKGROUND
        int y = y0;
        mImgTop->SetPosition(x0, y);

        y += mImgTop->GetHeight();
        mImgMid->SetPosition(x0, y);

        y += mImgMid->GetHeight();
        mImgBot->SetPosition(x0, y);
    }

private:
    sgl::graphic::Image * mImgTop = nullptr;
    sgl::graphic::Image * mImgMid = nullptr;
    sgl::graphic::Image * mImgBot = nullptr;
};

// ====== SCREEN SETTINGS ======
ScreenSettings::ScreenSettings(Game * game)
    : Screen(game)
{
    using namespace sgl;

    const int screenW = graphic::Renderer::Instance()->GetWidth();
    const int screenH = graphic::Renderer::Instance()->GetHeight();

    auto fm = graphic::FontManager::Instance();
    auto tm = graphic::TextureManager::Instance();
    graphic::Font * font;
    graphic::Texture * tex;

    //  BACKGROUND
    tex = tm->GetTexture("space_bg.jpg");
    mBg = new graphic::Image(tex);

    // MAIN PANEL
    tex = tm->GetSprite(SpriteFileSettings, IND_SET_PANEL);
    auto win = new sgui::Image(tex);

    win->SetPosition((screenW - win->GetWidth()) * 0.5f,
                     (screenH - win->GetHeight()) * 0.5f);

    const int marginContTop = 5;
    const int marginContLeft = 50;
    const int marginPanelTop = 105;

    // BUTTON BACK
    auto btnBack = new ButtonBack(nullptr);

    btnBack->SetPosition(win->GetX() + win->GetWidth() - btnBack->GetWidth(),
                         win->GetY());

    btnBack->AddOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::MAIN_MENU);
    });

    // TITLE
    const unsigned int colorTitle = 0xe6eef2ff;
    font = fm->GetFont("data/fonts/Lato-Regular.ttf", 30, graphic::Font::NORMAL);
    auto labelTitle = new sgui::Label("SETTINGS", font, win);

    labelTitle->SetColor(colorTitle);
    labelTitle->SetPosition(marginContLeft, marginContTop);

    // PANEL CONTENT
    auto panel = new PanelContentSettings(320, win);
    panel->SetPosition((win->GetWidth() - panel->GetWidth()) * 0.5f,marginPanelTop);
}

ScreenSettings::~ScreenSettings()
{
    sgl::sgui::Stage::Instance()->ClearWidgets();

    delete mBg;
}

void ScreenSettings::Update(float update)
{
}

void ScreenSettings::Render()
{
    mBg->Render();
}

} // namespace game
