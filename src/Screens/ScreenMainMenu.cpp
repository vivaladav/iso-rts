#include "Screens/ScreenMainMenu.h"

#include "Game.h"
#include "Version.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"
#include "Widgets/ButtonMainMenuSocial.h"
#include "Widgets/ButtonMainMenuWishlist.h"
#include "Widgets/GameUIData.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/PushButton.h>
#include <sgl/sgui/Stage.h>
#include <sgl/sgui/TextArea.h>
#include <sgl/utilities/System.h>

namespace game
{

ScreenMainMenu::ScreenMainMenu(Game * game)
    : Screen(game)
{
    using namespace sgl::graphic;
    using namespace sgl::sgui;

    game->SetClearColor(0x12, 0x12, 0x12, 0xFF);

    Widget * panelButtons = new Widget;

    int buttonY = 0;
    const int VMARGIN = 30;

    const int screenW = sgl::graphic::Renderer::Instance()->GetWidth();
    const int screenH = sgl::graphic::Renderer::Instance()->GetHeight();

    // -- BACKGROUND --
    mBg = new sgl::graphic::Image("data/img/main_menu_bg.png");

    // -- BUTTON NEW GAME --
    ButtonMainMenu * button = new ButtonMainMenu("NEW GAME", panelButtons);

    button->AddOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::FACTION_SEL);
    });

    buttonY += button->GetHeight() + VMARGIN;

    // -- BUTTON SETTINGS --
    button = new ButtonMainMenu("SETTINGS", panelButtons);
    button->SetY(buttonY);

    button->AddOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::SETTINGS);
    });

    buttonY += button->GetHeight() + VMARGIN;

    // -- BUTTON EXIT --
    button = new ButtonMainMenu("EXIT", panelButtons);
    button->SetY(buttonY);

    button->AddOnClickFunction([game] { game->Exit(); });

    // position buttons panel
    const int centerX = 450;
    const int containerX = centerX - panelButtons->GetWidth() * 0.5f;
    const int containerY = 280;
    panelButtons->SetPosition(containerX, containerY);

    // -- BUTTON WISHLIST --
    auto btnWishlist = new ButtonMainMenuWishlist(nullptr);

    int buttonX = centerX - btnWishlist->GetWidth() * 0.5f;
    buttonY = screenH - 100 - btnWishlist->GetHeight();

    btnWishlist->AddOnClickFunction([]
    {
        sgl::utilities::System sys;
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

    btnSocial->AddOnClickFunction([]
    {
        sgl::utilities::System sys;
        sys.OpenUrlInBrowser("https://discord.gg/pdEPr7efQX");
    });

    // YOUTUBE
    btnSocial = new ButtonMainMenuSocial(IND_MM_ICON_YOUTUBE, panelSocial);
    btnSocial->SetX(socialX);
    socialX += btnSocial->GetWidth() + socialMarginH;

    btnSocial->AddOnClickFunction([]
    {
        sgl::utilities::System sys;
        sys.OpenUrlInBrowser("https://www.youtube.com/c/vivaladev");
    });

    // TWITTER
    btnSocial = new ButtonMainMenuSocial(IND_MM_ICON_TWITTER, panelSocial);
    btnSocial->SetX(socialX);
    socialX += btnSocial->GetWidth() + socialMarginH;

    btnSocial->AddOnClickFunction([]
    {
        sgl::utilities::System sys;
        sys.OpenUrlInBrowser("https://twitter.com/vivaladev");
    });

    // LINKEDIN
    btnSocial = new ButtonMainMenuSocial(IND_MM_ICON_LINKEDIN, panelSocial);
    btnSocial->SetX(socialX);

    btnSocial->AddOnClickFunction([]
    {
        sgl::utilities::System sys;
        sys.OpenUrlInBrowser("https://www.linkedin.com/company/vivaladev/");
    });

    // position panel social buttons
    const int psMarginTop = 25;
    const int psX = centerX - panelSocial->GetWidth() * 0.5f;
    const int psY = btnWishlist->GetY() + btnWishlist->GetHeight() + psMarginTop;
    panelSocial->SetPosition(psX, psY);

    // VERSION LABEL
    auto fm = FontManager::Instance();
    Font * fnt = fm->GetFont("data/fonts/Lato-Regular.ttf", 18, Font::NORMAL);

    const std::string strVersion = std::string(VERSION) + std::string("\n") +
                                   std::string(VERSION_BRANCH) + std::string("-") +
                                   std::string(VERSION_NUM) + std::string("-") +
                                   std::string(VERSION_SHORT_HASH);
    auto labelVer = new TextArea(200, 50, strVersion.c_str(), fnt);
    labelVer->setTextAlignment(sgl::sgui::TextArea::ALIGN_H_RIGHT,
                               sgl::sgui::TextArea::ALIGN_V_BOTTOM);
    labelVer->SetColor(0xb2b2b2ff);
    const int marginLabelH = 15;
    const int marginLabelV = 10;
    const int labelX = screenW - labelVer->GetWidth() - marginLabelH;
    const int labelY = screenH - labelVer->GetHeight() - marginLabelV;
    labelVer->SetPosition(labelX, labelY);
}

ScreenMainMenu::~ScreenMainMenu()
{
    sgl::sgui::Stage::Instance()->ClearWidgets();

    delete mBg;
}

void ScreenMainMenu::Update(float update)
{
}

void ScreenMainMenu::Render()
{
    mBg->Render();
}

} // namespace game
