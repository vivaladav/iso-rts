#include "Screens/ScreenMainMenu.h"

#include "Game.h"
#include "GameConstants.h"
#include "Version.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"
#include "Widgets/ButtonMainMenuSocial.h"
#include "Widgets/ButtonMainMenuWishlist.h"
#include "Widgets/GameUIData.h"

#include <sgl/core/event/ApplicationEvent.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/Label.h>
#include <sgl/sgui/PushButton.h>
#include <sgl/sgui/Stage.h>
#include <sgl/utilities/System.h>

namespace game
{

ScreenMainMenu::ScreenMainMenu(Game * game)
    : Screen(game)
{
    using namespace sgl;

    game->AddApplicationListener(this);

    game->SetClearColor(0x12, 0x12, 0x12, 0xFF);

    auto panelButtons = new sgui::Widget;

    int buttonY = 0;
    const int VMARGIN = 30;

    const int screenW = graphic::Renderer::Instance()->GetWidth();
    const int screenH = graphic::Renderer::Instance()->GetHeight();

    // -- BACKGROUND --
    auto tm = graphic::TextureManager::Instance();
    auto tex = tm->GetTexture("main_menu_bg.png");

    mBg = new graphic::Image(tex);

    // -- BUTTON NEW GAME --
    ButtonMainMenu * button = new ButtonMainMenu("NEW GAME", panelButtons);

    button->AddOnClickFunction([game]
    {
        game->InitGameData();

        // TODO REMOVE WHEN PLANET SELECTION IS DONE
        game->SetCurrentPlanet(PLANET_1);
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
        utilities::System sys;
        sys.OpenUrlInBrowser("https://store.steampowered.com/app/1607580/Virtueror_The_Virtual_Conqueror/"
                             "?utm_source=game&utm_medium=button&utm_campaign=game&utm_content=mainmenu");
    });

    btnWishlist->SetPosition(buttonX, buttonY);

    // -- SOCIAL BUTTONS --
    auto panelSocial = new sgui::Widget;

    const int socialMarginH = 15;
    int socialX = 0;

    // DISCORD
    auto btnSocial = new ButtonMainMenuSocial(IND_MM_ICON_DISCORD, panelSocial);
    socialX += btnSocial->GetWidth() + socialMarginH;

    btnSocial->AddOnClickFunction([]
    {
        utilities::System sys;
        sys.OpenUrlInBrowser("https://discord.gg/pdEPr7efQX");
    });

    // YOUTUBE
    btnSocial = new ButtonMainMenuSocial(IND_MM_ICON_YOUTUBE, panelSocial);
    btnSocial->SetX(socialX);
    socialX += btnSocial->GetWidth() + socialMarginH;

    btnSocial->AddOnClickFunction([]
    {
        utilities::System sys;
        sys.OpenUrlInBrowser("https://www.youtube.com/c/vivaladev");
    });

    // TWITTER
    btnSocial = new ButtonMainMenuSocial(IND_MM_ICON_TWITTER, panelSocial);
    btnSocial->SetX(socialX);
    socialX += btnSocial->GetWidth() + socialMarginH;

    btnSocial->AddOnClickFunction([]
    {
        utilities::System sys;
        sys.OpenUrlInBrowser("https://twitter.com/vivaladev");
    });

    // LINKEDIN
    btnSocial = new ButtonMainMenuSocial(IND_MM_ICON_LINKEDIN, panelSocial);
    btnSocial->SetX(socialX);

    btnSocial->AddOnClickFunction([]
    {
        utilities::System sys;
        sys.OpenUrlInBrowser("https://www.linkedin.com/company/vivaladev/");
    });

    // position panel social buttons
    const int psMarginTop = 25;
    const int psX = centerX - panelSocial->GetWidth() * 0.5f;
    const int psY = btnWishlist->GetY() + btnWishlist->GetHeight() + psMarginTop;
    panelSocial->SetPosition(psX, psY);

    // VERSION LABEL
    auto fm = graphic::FontManager::Instance();
    graphic::Font * fnt = fm->GetFont("Lato-Regular.ttf", 18, graphic::Font::NORMAL);

    const unsigned int colorVersion = 0xb2b2b2ff;
    const int marginLabelH = 15;
    const int marginLabelV = 10;

    auto labelVer = new sgui::Label(VERSION, fnt);
    labelVer->SetColor(colorVersion);

    const std::string strBuild = std::string(VERSION_BRANCH) + std::string("-") +
                                 std::string(VERSION_NUM) + std::string("-") +
                                 std::string(VERSION_SHORT_HASH);
    auto labelBuild = new sgui::Label(strBuild.c_str(), fnt);
    labelBuild->SetColor(colorVersion);

    const int labelBuildX = screenW - labelBuild->GetWidth() - marginLabelH;
    const int labelBuildY = screenH - labelBuild->GetHeight() - marginLabelV;
    labelBuild->SetPosition(labelBuildX, labelBuildY);

    const int labelVerX = screenW - labelVer->GetWidth() - marginLabelH;
    const int labelVerY = labelBuildY - labelVer->GetHeight();
    labelVer->SetPosition(labelVerX, labelVerY);

    // makes sure game data is cleared before starting something new
    game->ClearGameData();
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

void ScreenMainMenu::OnApplicationQuit(sgl::core::ApplicationEvent & event)
{
    GetGame()->Exit();

    event.SetConsumed();
}

} // namespace game
