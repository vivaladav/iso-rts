#include "Widgets/DialogEndMission.h"

#include "Game.h"
#include "Widgets/GameButton.h"
#include "Widgets/GameUIData.h"

#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/media/AudioManager.h>
#include <sgl/media/AudioPlayer.h>
#include <sgl/sgui/ImageButton.h>
#include <sgl/sgui/Label.h>
#include <sgl/utilities/System.h>

#include <sstream>

namespace game
{

// ===== BUTTON =====
class ButtonDialogEndMission : public GameButton
{
public:
    ButtonDialogEndMission(sgl::sgui::Widget * parent)
        : GameButton(SpriteFileDialogEndMission,
        { IND_DIA_EM_BTN_NORMAL, IND_DIA_EM_BTN_DISABLED,
          IND_DIA_EM_BTN_OVER, IND_DIA_EM_BTN_PUSHED, IND_DIA_EM_BTN_PUSHED },
        { 0xc1e0f0ff, 0x5a6266ff, 0xd6e9f5ff, 0xadd6ebff, 0xadd6ebff },
        parent)
    {
        using namespace sgl;

        const char * fileFont = "Lato-Regular.ttf";
        const int size = 24;

        auto fm = graphic::FontManager::Instance();
        graphic::Font * fnt = fm->GetFont(fileFont, size, graphic::Font::NORMAL);
        SetLabelFont(fnt);
        SetLabel("CONTINUE");

        SetShortcutKey(sgl::core::KeyboardEvent::KEY_RETURN);
    }

    void HandleMouseOver() override
    {
        sgl::sgui::AbstractButton::HandleMouseOver();

        auto player = sgl::media::AudioManager::Instance()->GetPlayer();
        player->PlaySound("UI/button_over-01.ogg");
    }

    void HandleButtonDown() override
    {
        sgl::sgui::AbstractButton::HandleButtonDown();

        auto player = sgl::media::AudioManager::Instance()->GetPlayer();
        player->PlaySound("UI/button_click-01.ogg");
    }
};

// ===== DIALOG =====
DialogEndMission::DialogEndMission(int time, int territoryConquered, int enemiesKilled,
                                   int casualties, bool victory)
{
    using namespace sgl;

    auto fm = graphic::FontManager::Instance();
    auto tm = graphic::TextureManager::Instance();

    // BACKGROUND
    graphic::Texture * tex = tm->GetSprite(SpriteFileDialogEndMission, IND_DIA_EM_BG);
    mBg = new graphic::Image(tex);
    RegisterRenderable(mBg);

    const int w = mBg->GetWidth();
    const int h = mBg->GetHeight();
    SetSize(w, h);

    // BUTTON CLOSE
    mButton = new ButtonDialogEndMission(this);

    const int buttonX = (w - mButton->GetWidth()) / 2;
    const int buttonY = h - 30 - mButton->GetHeight();
    mButton->SetPosition(buttonX, buttonY);

    // TITLE
    auto font = fm->GetFont("Lato-Regular.ttf", 32, graphic::Font::NORMAL);

    sgui::Label * title = nullptr;

    if(victory)
        title = new sgui::Label("VICTORY", font, this);
    else
        title = new sgui::Label("DEFEAT", font, this);

    const int titleX = (w - title->GetWidth()) / 2;
    const int titleY = 10;
    title->SetPosition(titleX, titleY);
    title->SetColor(0xf1f3f4ff);

    // -- CONTENT --
    const int limitR = 720;
    const int marginL = 40;
    const int marginT = 120;
    const int marginWidgetH = 35;
    const unsigned int colorHeader = 0x9dcbe2ff;
    const unsigned int colorData = 0x70a7c2ff;

    int widgetX = marginL;
    int widgetY = marginT;

    font = fm->GetFont("Lato-Regular.ttf", 24, graphic::Font::NORMAL);

    // TIME DEPLOYED
    auto label = new sgui::Label("TIME DEPLOYED", font, this);
    label->SetColor(colorHeader);
    label->SetPosition(widgetX, widgetY);

    const int secsInM = 60;
    const int secsInH = 60 * 60;
    const int timeH = time / secsInH;
    time -= timeH * secsInH;
    const int timeM = time / secsInM;
    time -= timeM * secsInM;
    const int timeS = time;

    const int fieldW = 2;
    const char fieldF = '0';
    std::ostringstream ss;
    ss.width(fieldW);
    ss.fill(fieldF);
    ss << timeH << ":";
    ss.width(fieldW);
    ss.fill(fieldF);
    ss << timeM << ":";
    ss.width(fieldW);
    ss.fill(fieldF);
    ss << timeS;

    label = new sgui::Label(ss.str().c_str(), font, this);
    label->SetColor(colorData);
    widgetX += limitR - label->GetWidth();
    label->SetPosition(widgetX, widgetY);

    widgetX = marginL;
    widgetY += marginWidgetH + label->GetHeight();

    ss.str(std::string());
    ss.clear();

    // TERRITORY CONQUERED
    label = new sgui::Label("TERRITORY CONQUERED", font, this);
    label->SetColor(colorHeader);
    label->SetPosition(widgetX, widgetY);

    ss << territoryConquered << "%";

    label = new sgui::Label(ss.str().c_str(), font, this);
    label->SetColor(colorData);
    widgetX += limitR - label->GetWidth();
    label->SetPosition(widgetX, widgetY);

    widgetX = marginL;
    widgetY += marginWidgetH + label->GetHeight();

    ss.str(std::string());
    ss.clear();

    // ENEMIES DESTROYED
    label = new sgui::Label("ENEMIES DESTROYED", font, this);
    label->SetColor(colorHeader);
    label->SetPosition(widgetX, widgetY);

    ss << enemiesKilled;

    label = new sgui::Label(ss.str().c_str(), font, this);
    label->SetColor(colorData);
    widgetX += limitR - label->GetWidth();
    label->SetPosition(widgetX, widgetY);

    widgetX = marginL;
    widgetY += marginWidgetH + label->GetHeight();

    ss.str(std::string());
    ss.clear();

    // CASUALTIES
    label = new sgui::Label("CASUALTIES", font, this);
    label->SetColor(colorHeader);
    label->SetPosition(widgetX, widgetY);

    ss << casualties;

    label = new sgui::Label(ss.str().c_str(), font, this);
    label->SetColor(colorData);
    widgetX += limitR - label->GetWidth();
    label->SetPosition(widgetX, widgetY);

    widgetX = marginL;
    widgetY += marginWidgetH + label->GetHeight();
}

void DialogEndMission::SetFunctionOnClose(const std::function<void()> & f)
{
    mButton->AddOnClickFunction(f);
}

void DialogEndMission::HandlePositionChanged()
{
    SetPositions();
}

void DialogEndMission::SetPositions()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();
    const int w = mBg->GetWidth();

    // BACKGROUND
    mBg->SetPosition(x0, y0);
}

} // namespace game
