#include "Widgets/ExternalButtons.h"

#include "Widgets/GameUIData.h"
#include "Widgets/WidgetsConstants.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/media/AudioManager.h>
#include <sgl/media/AudioPlayer.h>
#include <sgl/utilities/System.h>
#include <sgl/utilities/StringManager.h>

namespace
{
constexpr int sizeExtBtn = 18;
}

namespace game
{

// ---- BUTTON DISCORD ----
ButtonExtDiscord::ButtonExtDiscord(sgl::sgui::Widget * parent)
    : GameButton(parent)
{
    using namespace sgl;

    SetData(SpriteFileUIShared,
            {
                ID_BTN_DISCORD_NORMAL,
                ID_BTN_DISCORD_NORMAL,
                ID_BTN_DISCORD_OVER,
                ID_BTN_DISCORD_PUSHED,
                ID_BTN_DISCORD_NORMAL,
            },
            {
                0xdadcf1ff,
                0xdadcf1ff,
                0xedeef8ff,
                0xb5b9e3ff,
                0xdadcf1ff
            });

    // ICON
    const int marginIcon = 10;
    auto tm = graphic::TextureManager::Instance();
    auto tex = tm->GetSprite(SpriteFileUIShared, ID_ICON_DISCORD);
    SetIcon(tex);
    SetIconMargin(marginIcon);

    // TEXT
    auto sm = utilities::StringManager::Instance();
    auto fm = graphic::FontManager::Instance();
    auto font = fm->GetFont(WidgetsConstants::FontFileButton, sizeExtBtn, graphic::Font::NORMAL);

    SetLabelFont(font);
    SetLabel(sm->GetCString("JOIN_DISCORD"));

    AddOnClickFunction([]
        {
            utilities::System sys;
            sys.OpenUrlInBrowser("https://discord.gg/VkamNzEzDg");
        });
}

void ButtonExtDiscord::HandleMouseOver()
{
    GameButton::HandleMouseOver();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_over-02.ogg");
}

void ButtonExtDiscord::HandleButtonDown()
{
    GameButton::HandleButtonDown();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_click-02.ogg");
}

// ---- BUTTON FEEDBACK ----
ButtonExtFeedback::ButtonExtFeedback(sgl::sgui::Widget * parent)
    : GameButton(parent)
{
    using namespace sgl;

    SetData(SpriteFileUIShared,
            {
                ID_BTN_FEEDBACK_NORMAL,
                ID_BTN_FEEDBACK_NORMAL,
                ID_BTN_FEEDBACK_OVER,
                ID_BTN_FEEDBACK_PUSHED,
                ID_BTN_FEEDBACK_NORMAL,
            },
            {
                0xdaf1e5ff,
                0xdaf1e5ff,
                0xedf8f2ff,
                0xb5e3cbff,
                0xdaf1e5ff
            });

    // ICON
    const int marginIcon = 10;
    auto tm = graphic::TextureManager::Instance();
    auto tex = tm->GetSprite(SpriteFileUIShared, ID_ICON_FEEDBACK);
    SetIcon(tex);
    SetIconMargin(marginIcon);

    // TEXT
    auto sm = utilities::StringManager::Instance();
    auto fm = graphic::FontManager::Instance();
    auto font = fm->GetFont(WidgetsConstants::FontFileButton, sizeExtBtn, graphic::Font::NORMAL);

    SetLabelFont(font);
    SetLabel(sm->GetCString("GIVE_FEEDBACK"));

    AddOnClickFunction([]
        {
            utilities::System sys;
            sys.OpenUrlInBrowser("https://docs.google.com/forms/d/e/1FAIpQLSdhgF0ZyWRZrKb7etpulBOII"
                                 "gt_marh7PNt3fNAYQECv8_6Aw/viewform?usp=header");
        });
}

void ButtonExtFeedback::HandleMouseOver()
{
    GameButton::HandleMouseOver();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_over-02.ogg");
}

void ButtonExtFeedback::HandleButtonDown()
{
    GameButton::HandleButtonDown();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_click-02.ogg");
}

// ---- BUTTON STEAM ----
ButtonExtSteam::ButtonExtSteam(const std::string & url, sgl::sgui::Widget * parent)
    : GameButton(parent)
{
    using namespace sgl;

    SetData(SpriteFileUIShared,
            {
                ID_BTN_STEAM_NORMAL,
                ID_BTN_STEAM_NORMAL,
                ID_BTN_STEAM_OVER,
                ID_BTN_STEAM_PUSHED,
                ID_BTN_STEAM_NORMAL,
            },
            {
                0xc7ced1ff,
                0xc7ced1ff,
                0xe3e6e8ff,
                0xabb5baff,
                0xc7ced1ff
            });

    // ICON
    const int marginIcon = 10;
    auto tm = graphic::TextureManager::Instance();
    auto tex = tm->GetSprite(SpriteFileUIShared, ID_ICON_STEAM);
    SetIcon(tex);
    SetIconMargin(marginIcon);

    // TEXT
    auto sm = utilities::StringManager::Instance();
    auto fm = graphic::FontManager::Instance();
    auto font = fm->GetFont(WidgetsConstants::FontFileButton, sizeExtBtn, graphic::Font::NORMAL);

    SetLabelFont(font);
    SetLabel(sm->GetCString("WL_NOW"));

    AddOnClickFunction([url]
        {
            utilities::System sys;
            sys.OpenUrlInBrowser(url);
        });
}

void ButtonExtSteam::HandleMouseOver()
{
    GameButton::HandleMouseOver();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_over-02.ogg");
}

void ButtonExtSteam::HandleButtonDown()
{
    GameButton::HandleButtonDown();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_click-02.ogg");
}

} // namespace game