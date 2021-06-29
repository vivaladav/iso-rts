#include "Screens/ScreenMainMenu.h"

#include "Game.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"
#include "Widgets/ButtonMainMenuSocial.h"
#include "Widgets/ButtonMainMenuWishlist.h"
#include "Widgets/GameUIData.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/Renderer.h>
#include <graphic/TextureManager.h>
#include <sgui/Label.h>
#include <sgui/PushButton.h>
#include <sgui/Stage.h>
#include <utilities/System.h>

namespace game
{

ScreenMainMenu::ScreenMainMenu(Game * game)
    : Screen(game)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    InitSprites();

    game->SetClearColor(0x12, 0x12, 0x12, 0xFF);

    Widget * panelButtons = new Widget;

    int buttonY = 0;
    const int VMARGIN = 30;

    const int screenW = lib::graphic::Renderer::Instance()->GetWidth();
    const int screenH = lib::graphic::Renderer::Instance()->GetHeight();

    // -- BACKGROUND --
    mBg = new lib::graphic::Image("data/img/main_menu_bg.png");

    // -- BUTTON NEW GAME --
    ButtonMainMenu * button = new ButtonMainMenu("NEW GAME", panelButtons);

    button->SetOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::FACTION_SEL);
    });

    buttonY += button->GetHeight() + VMARGIN;

    // -- BUTTON EXIT --
    button = new ButtonMainMenu("EXIT", panelButtons);
    button->SetY(buttonY);

    button->SetOnClickFunction([game] { game->Exit(); });

    // position buttons panel
    const int centerX = 450;
    const int containerX = centerX - panelButtons->GetWidth() * 0.5f;
    const int containerY = 280;
    panelButtons->SetPosition(containerX, containerY);

    // -- BUTTON WISHLIST --
    auto btnWishlist = new ButtonMainMenuWishlist(nullptr);

    int buttonX = centerX - btnWishlist->GetWidth() * 0.5f;
    buttonY = screenH - 100 - btnWishlist->GetHeight();

    btnWishlist->SetOnClickFunction([]
    {
        lib::utilities::System sys;
        sys.OpenUrlInBrowser("https://store.steampowered.com/app/1607580?utm_campaign=game&utm_source=mainmenu");
    });

    btnWishlist->SetPosition(buttonX, buttonY);

    // -- SOCIAL BUTTONS --
    auto panelSocial = new Widget;

    const int socialMarginH = 15;
    int socialX = 0;

    // DISCORD
    auto btnSocial = new ButtonMainMenuSocial(IND_MM_ICON_DISCORD, panelSocial);
    socialX += btnSocial->GetWidth() + socialMarginH;

    btnSocial->SetOnClickFunction([]
    {
        lib::utilities::System sys;
        sys.OpenUrlInBrowser("https://discord.gg/pdEPr7efQX");
    });

    // YOUTUBE
    btnSocial = new ButtonMainMenuSocial(IND_MM_ICON_YOUTUBE, panelSocial);
    btnSocial->SetX(socialX);
    socialX += btnSocial->GetWidth() + socialMarginH;

    btnSocial->SetOnClickFunction([]
    {
        lib::utilities::System sys;
        sys.OpenUrlInBrowser("https://www.youtube.com/c/vivaladev");
    });

    // TWITTER
    btnSocial = new ButtonMainMenuSocial(IND_MM_ICON_TWITTER, panelSocial);
    btnSocial->SetX(socialX);
    socialX += btnSocial->GetWidth() + socialMarginH;

    btnSocial->SetOnClickFunction([]
    {
        lib::utilities::System sys;
        sys.OpenUrlInBrowser("https://twitter.com/vivaladev");
    });

    // LINKEDIN
    btnSocial = new ButtonMainMenuSocial(IND_MM_ICON_LINKEDIN, panelSocial);
    btnSocial->SetX(socialX);

    btnSocial->SetOnClickFunction([]
    {
        lib::utilities::System sys;
        sys.OpenUrlInBrowser("https://www.linkedin.com/company/vivaladev/");
    });

    // position panel social buttons
    const int psMarginTop = 25;
    const int psX = centerX - panelSocial->GetWidth() * 0.5f;
    const int psY = btnWishlist->GetY() + btnWishlist->GetHeight() + psMarginTop;
    panelSocial->SetPosition(psX, psY);

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
