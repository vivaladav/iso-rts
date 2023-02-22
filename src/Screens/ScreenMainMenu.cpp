#include "Screens/ScreenMainMenu.h"

#include "Game.h"
#include "GameConstants.h"
#include "Version.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"
#include "Widgets/ButtonMainMenuSocial.h"
#include "Widgets/ButtonMainMenuWishlist.h"
#include "Widgets/GameUIData.h"
#include "Widgets/DialogChangelog.h"

#include <sgl/core/event/ApplicationEvent.h>
#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/media/AudioManager.h>
#include <sgl/media/AudioPlayer.h>
#include <sgl/sgui/Label.h>
#include <sgl/sgui/PushButton.h>
#include <sgl/sgui/Stage.h>
#include <sgl/sgui/TextArea.h>
#include <sgl/utilities/System.h>

namespace game
{

ScreenMainMenu::ScreenMainMenu(Game * game)
    : Screen(game)
{
    using namespace sgl;

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

    // CHANGELOG
    CreateChangelog();

    // VERSION LABEL
    auto fm = graphic::FontManager::Instance();
    graphic::Font * fnt = fm->GetFont("Lato-Regular.ttf", 18, graphic::Font::NORMAL);

    const unsigned int colorVersion = 0xb2b2b2ff;
    const int marginLabelH = 15;
    const int marginLabelV = 10;

    auto labelVer = new sgui::Label(VERSION, fnt);
    labelVer->SetColor(colorVersion);

#if defined(DEBUG)
    const std::string strBuildType("D");
#elif defined(RELEASE)
    const std::string strBuildType("R");
#endif

    const std::string strBuild = std::string(VERSION_BRANCH) + std::string("-") +
                                 std::string(VERSION_NUM) + strBuildType + std::string("-") +
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

    // START MUSIC
    sgl::media::AudioManager::Instance()->GetPlayer()->PlayMusic("menus/menu_01.ogg");
}

ScreenMainMenu::~ScreenMainMenu()
{
    sgl::sgui::Stage::Instance()->ClearWidgets();

    delete mBg;
}

void ScreenMainMenu::OnKeyUp(sgl::core::KeyboardEvent & event)
{
    const int key = event.GetKey();

    // CTRL-T -> open test screen
    if(key == sgl::core::KeyboardEvent::KEY_T && event.IsModCtrlDown())
        GetGame()->RequestNextActiveState(StateId::TEST);
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

void ScreenMainMenu::CreateChangelog()
{
    using namespace sgl;

    const int screenW = graphic::Renderer::Instance()->GetWidth();
    const int screenH = graphic::Renderer::Instance()->GetHeight();

    // BUTTON
    mButtonChangelog = new ButtonChangelog;
    const int btnChangelogX = screenW - mButtonChangelog->GetWidth();
    const int btnChangelogY = (screenH - mButtonChangelog->GetHeight()) / 2;
    mButtonChangelog->SetPosition(btnChangelogX, btnChangelogY);

    mButtonChangelog->AddOnClickFunction([this]
    {
        mButtonChangelog->SetVisible(false);
        mDialogChangelog->SetVisible(true);
    });

    // DIALOG
    mDialogChangelog = new DialogChangelog;
    const int dialogChangelogX = screenW - mDialogChangelog->GetWidth();
    const int dialogChangelogY = (screenH - mDialogChangelog->GetHeight()) / 2;
    mDialogChangelog->SetPosition(dialogChangelogX, dialogChangelogY);
    mDialogChangelog->SetVisible(false);

    mDialogChangelog->AddOnCloseClickFunction([this]
    {
        mButtonChangelog->SetVisible(true);
        mDialogChangelog->SetVisible(false);
    });

    // CONTENT
    const unsigned int colorContent = 0xb8ced9ff;
    const int contentW = 385;
    const int marginL = 20;
    const int marginT = 10;
    const int paddingV = 10;

    auto fm = graphic::FontManager::Instance();
    auto font = fm->GetFont("Lato-Regular.ttf", 18, graphic::Font::NORMAL);

    auto content = new sgl::sgui::Widget;

    const int contX = marginL;
    int contY = marginT;

    auto title = new sgui::Label("0.1.2 - \"Almost alpha\"", font, content);
    title->SetPosition(contX, contY);
    title->SetColor(colorContent);

    contY += title->GetHeight() + paddingV;

    // CONTENT BLOCK: NEW FEATURES
    int blockH = 200;
    auto textNewFeat = new sgui::TextArea(contentW, blockH, font, content);
    textNewFeat->SetText("NEW FEATURES\n"
                         "- Added music\n"
                         "- Added sounds effects\n"
                         "- Introduced control areas to highlight what part of a map is controlled by each faction\n"
                         "- Implemented tooltips for UI elements\n"
                         "- Added audio settings\n"
                         "- New changelog dialog in main menu\n");
    textNewFeat->SetPosition(contX, contY);
    textNewFeat->SetColor(colorContent);

    contY += blockH + paddingV;

    // CONTENT BLOCK: IMPROVEMENTS
    blockH = 230;
    auto textImpr = new sgui::TextArea(contentW, blockH, font, content);
    textImpr->SetText("IMPROVEMENTS\n"
                      "- Settings dialog now available in game\n"
                      "- Map scrolling stops when mouse is outside the window\n"
                      "- Game pauses when window is minimized\n"
                      "- Disabled keys to navigate screens to avoid random exits\n"
                      "- Action icons for units and structures got a bit bigger\n");
    textImpr->SetPosition(contX, contY);
    textImpr->SetColor(colorContent);

    contY += blockH + paddingV;

    // CONTENT BLOCK: FIXES
    blockH = 100;
    auto textFix = new sgui::TextArea(contentW, blockH, font, content);
    textFix->SetText("FIXES\n"
                     "- Camera can't scroll to an empty area when scrolling diagonally");
    textFix->SetPosition(contX, contY);
    textFix->SetColor(colorContent);

    // set content
    mDialogChangelog->SetContent(content);
}

} // namespace game
