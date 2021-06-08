#include "Screens/ScreenMainMenu.h"

#include "Game.h"
#include "GameData.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"
#include "Widgets/ButtonMainMenuWishlist.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/Renderer.h>
#include <graphic/TextureManager.h>
#include <sgui/Label.h>
#include <sgui/PushButton.h>
#include <sgui/Stage.h>

namespace game
{

ScreenMainMenu::ScreenMainMenu(Game * game)
    : Screen(game)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    InitSprites();

    game->SetClearColor(0x12, 0x12, 0x12, 0xFF);

    Widget * panel = new Widget;

    int buttonY = 0;
    const int VMARGIN = 30;

    const int screenW = lib::graphic::Renderer::Instance()->GetWidth();
    const int screenH = lib::graphic::Renderer::Instance()->GetHeight();

    // -- BACKGROUND --
    mBg = new lib::graphic::Image("data/img/main_menu_bg.png");

    // -- BUTTON NEW GAME --
    ButtonMainMenu * button = new ButtonMainMenu("NEW GAME", panel);

    button->SetOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::NEW_GAME);
    });

    buttonY += button->GetHeight() + VMARGIN;

    // -- BUTTON EXIT --
    button = new ButtonMainMenu("EXIT", panel);

    button->SetOnClickFunction([game] { game->Exit(); });

    button->SetY(buttonY);

    // position buttons panel
    const int centerX = 450;
    const int containerX = centerX - panel->GetWidth() * 0.5f;
    const int containerY = 280;
    panel->SetPosition(containerX, containerY);

    // BUTTON WISHLIST
    auto btnWishlist = new ButtonMainMenuWishlist(nullptr);

    int buttonX = centerX - btnWishlist->GetWidth() * 0.5f;
    buttonY = screenH - 100 - btnWishlist->GetHeight();

    btnWishlist->SetPosition(buttonX, buttonY);

    // VERSION LABEL
    auto fm = FontManager::Instance();
    Font * fnt = fm->GetFont("data/fonts/Lato-Regular.ttf", 16, Font::NORMAL);

    auto labelVer = new Label("v. 0.0.1", fnt);
    labelVer->SetColor(0xb2b2b2ff);
    const int labelX = screenW - labelVer->GetWidth() - 40;
    const int labelY = screenH - labelVer->GetHeight() - 10;
    labelVer->SetPosition(labelX, labelY);
}

ScreenMainMenu::~ScreenMainMenu()
{
    lib::sgui::Stage::Instance()->ClearWidgets();

    delete mBg;
}

void ScreenMainMenu::Update(float update)
{
}

void ScreenMainMenu::Render()
{
    mBg->Render();
}

void ScreenMainMenu::InitSprites()
{
    // NOTE this code will need to be moved to a dedicated screen to init data on start
    auto tm = lib::graphic::TextureManager::Instance();

    // CELLS
    std::vector<lib::core::Rectd> rectsMainMenu
    {
        // PRIMARY BUTTON
        { 0, 0, 360, 70 },
        { 0, 70, 360, 70 },
        { 0, 140, 360, 70 },
        { 0, 210, 360, 70 },
        { 0, 280, 360, 70 },

        // WISHLIST BUTTON
        { 0, 350, 285, 50 },
        { 0, 400, 285, 50 },
        { 0, 450, 285, 50 },

        // SOCIAL BUTTON
        { 285, 350, 90, 50 },
        { 285, 400, 90, 50 },
        { 285, 450, 90, 50 },

        // ICONS
        { 0, 500, 32, 24 },
        { 32, 500, 32, 24 },
        { 64, 500, 30, 24 },
        { 94, 500, 34, 24 },
        { 128, 500, 24, 24 }
    };

    tm->RegisterSprite(SpriteFileMainMenu, rectsMainMenu);
}

} // namespace game
