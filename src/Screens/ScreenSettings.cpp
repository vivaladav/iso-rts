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


// ====== SCREEN SETTINGS ======
ScreenSettings::ScreenSettings(Game * game)
    : Screen(game)
{
    using namespace sgl;

    const int screenW = graphic::Renderer::Instance()->GetWidth();
    const int screenH = graphic::Renderer::Instance()->GetHeight();

    auto tm = graphic::TextureManager::Instance();

    //  BACKGROUND
    auto tex = tm->GetTexture("space_bg.jpg");
    mBg = new graphic::Image(tex);

    // MAIN PANEL
    tex = tm->GetSprite(SpriteFileSettings, IND_SET_PANEL);
    sgl::sgui::Widget * panel = new sgl::sgui::Image(tex);

    panel->SetPosition((screenW - panel->GetWidth()) * 0.5f, (screenH - panel->GetHeight()) * 0.5f);

    // BUTTON BACK
    auto btnBack = new ButtonBack(nullptr);
    const int posBackX = panel->GetX() + panel->GetWidth() - btnBack->GetWidth();
    const int posBackY = panel->GetY();
    btnBack->SetPosition(posBackX, posBackY);

    btnBack->AddOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::MAIN_MENU);
    });
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
