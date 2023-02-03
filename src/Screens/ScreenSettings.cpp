#include "Screens/ScreenSettings.h"

#include "Game.h"
#include "States/StatesIds.h"
#include "Widgets/GameButton.h"
#include "Widgets/GameSliderH.h"
#include "Widgets/GameUIData.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/graphic/Window.h>
#include <sgl/media/AudioManager.h>
#include <sgl/media/AudioPlayer.h>
#include <sgl/sgui/AbstractButtonsGroup.h>
#include <sgl/sgui/ComboBox.h>
#include <sgl/sgui/ComboBoxItem.h>
#include <sgl/sgui/Image.h>
#include <sgl/sgui/Label.h>
#include <sgl/sgui/Stage.h>

#include <iostream>
#include <sstream>

namespace
{
    constexpr unsigned int colorTxt = 0x73a6bfff;
    constexpr unsigned int colorTxtSlider = 0xadc2ccff;
    constexpr unsigned int sizeTxt = 22;
    const char * fontTxt = "Lato-Regular.ttf";

    constexpr int blockSettingW = 500;
    constexpr int blockSettingH = 100;
}

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
        Font * font = fm->GetFont("Lato-Regular.ttf", 24, Font::NORMAL);

        SetLabelFont(font);

        SetLabel("BACK");
    }

private:
    void HandleMouseOver()
    {
        sgl::sgui::AbstractButton::HandleMouseOver();

        auto player = sgl::media::AudioManager::Instance()->GetPlayer();
        player->PlaySound("UI/button_over-02.ogg");
    }

    void HandleButtonDown()
    {
        sgl::sgui::AbstractButton::HandleButtonDown();

        auto player = sgl::media::AudioManager::Instance()->GetPlayer();
        player->PlaySound("UI/button_click-02.ogg");
    }
};

// ====== PANEL BUTTON =====

class ButtonPanel : public sgl::sgui::AbstractButton
{
public:
    ButtonPanel(const char * text, sgl::sgui::Widget * parent)
        : sgl::sgui::AbstractButton(parent)
    {
        using namespace sgl;

        // UPDATE SIZE
        const int w = 250;
        const int h = 40;
        SetSize(w, h);

        // LABEL
        auto fm = graphic::FontManager::Instance();
        graphic::Font * font = fm->GetFont("Lato-Regular.ttf", 24, graphic::Font::NORMAL);

        mLabel = new graphic::Text(text, font);
        RegisterRenderable(mLabel);

        // BAR
        auto tm = graphic::TextureManager::Instance();
        graphic::Texture * tex = tm->GetSprite(SpriteFileSettings, IND_SET_BTN2_LINE);

        mBar = new sgui::Image(tex, this);
        mBar->SetY(h - mBar->GetHeight());

        // UPDATE CONTENT
        UpdateGraphics(NORMAL);
        UpdatePositions();
    }

private:
    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override
    {
        UpdateGraphics(state);
    }

    void HandlePositionChanged() override
    {
        UpdatePositions();
    }

    void UpdateGraphics(sgl::sgui::AbstractButton::VisualState state)
    {
        const unsigned int colors[NUM_VISUAL_STATES] =
        {
           0x98c2d9ff,
           0x506773ff,
           0xaad9f2ff,
           0xa1cee5ff,
           0xaad9f2ff,
        };

        mLabel->SetColor(colors[state]);

        mBar->SetVisible(IsChecked());
    }

    void UpdatePositions()
    {
        const int x0 = GetScreenX();
        const int y0 = GetScreenY();

        // LABEL
        mLabel->SetPosition(x0 + (GetWidth() - mLabel->GetWidth()) * 0.5f,
                            y0 + (GetHeight() - mLabel->GetHeight()) * 0.5f);
    }

private:
    sgl::graphic::Text * mLabel = nullptr;

    sgl::sgui::Image * mBar = nullptr;
};


// ====== COMBOBOX =====
class SettingsComboBox : public sgl::sgui::ComboBox
{
public:
    SettingsComboBox(sgl::sgui::Widget * parent)
        : sgl::sgui::ComboBox(parent)
    {
        using namespace sgl::graphic;

        auto fm = FontManager::Instance();
        Font * font = fm->GetFont("Lato-Regular.ttf", 20, Font::NORMAL);
        SetLabelFont(font);
    }

private:
    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override
    {
        using namespace sgl::graphic;

        // BODY
        const unsigned int texIds[NUM_VISUAL_STATES] =
        {
            IND_SET_CB_NORMAL,
            IND_SET_CB_DISABLED,
            IND_SET_CB_OVER,
            IND_SET_CB_PUSHED,
            IND_SET_CB_NORMAL,
        };

        auto tm = TextureManager::Instance();
        Texture * tex = tm->GetSprite(SpriteFileSettings, texIds[state]);
        SetBodyTexture(tex);

        // TEXT
        const unsigned int txtColors[NUM_VISUAL_STATES] =
        {
            0xd7eaf4ff,
            0x506c7cff,
            0xebf4f9ff,
            0xc3dfeeff,
            0xd7eaf4ff
        };

       SetLabelColor(txtColors[state]);
    }
};

// ====== COMBOBOX ITEM =====
class SettingsComboBoxItem : public sgl::sgui::ComboBoxItem
{
public:
    SettingsComboBoxItem(const char * txt)
        : sgl::sgui::ComboBoxItem(txt)
    {
        using namespace sgl::graphic;

        mBody = new Image;
        RegisterRenderable(mBody);

        // TEXT LABEL
        auto fm = FontManager::Instance();
        Font * font = fm->GetFont("Lato-Regular.ttf", 20, Font::NORMAL);
        mText = new Text(txt, font, true);
        RegisterRenderable(mText);

        // init to normal state
        SetState(NORMAL);

        PositionElements();
    }

private:
    void HandlePositionChanged() override
    {
        PositionElements();
    }

    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override
    {
        using namespace sgl::graphic;

        // BODY
        const unsigned int texIds[NUM_VISUAL_STATES] =
        {
            IND_SET_CBI_NORMAL,
            IND_SET_CBI_DISABLED,
            IND_SET_CBI_OVER,
            IND_SET_CBI_PUSHED,
            IND_SET_CBI_NORMAL,
        };

        auto tm = TextureManager::Instance();
        Texture * tex = tm->GetSprite(SpriteFileSettings, texIds[state]);
        mBody->SetTexture(tex);

        SetSize(mBody->GetWidth(), mBody->GetHeight());

        // TEXT
        const unsigned int txtColors[NUM_VISUAL_STATES] =
        {
            0xd7eaf4ff,
            0x506c7cff,
            0xebf4f9ff,
            0xc3dfeeff,
            0xd7eaf4ff
        };

       mText->SetColor(txtColors[state]);
    }

    void PositionElements()
    {
        const int x0 = GetScreenX();
        const int y0 = GetScreenY();

        // position BG
        mBody->SetPosition(x0, y0);

        // text
        const int textX = x0 + (GetWidth() - mText->GetWidth()) * 0.5f;
        const int textY = y0 + (GetHeight() - mText->GetHeight()) * 0.5f;
        mText->SetPosition(textX, textY);
    }

private:
    sgl::graphic::Image * mBody = nullptr;
    sgl::graphic::Text * mText = nullptr;
};

class ComboBoxItemResolution : public SettingsComboBoxItem
{
public:
    ComboBoxItemResolution(int display, int mode, const char * txt)
        : SettingsComboBoxItem(txt)
        , mDisplay(display)
        , mMode(mode)
    {
    }

    int GetDisplay() const { return mDisplay; }
    int GetMode() const { return mMode; }

private:
    int mDisplay = 0;
    int mMode = 0;
};

// ====== CHECKBOX BUTTON =====

class SettingsCheckbox : public sgl::sgui::AbstractButton
{
public:
    SettingsCheckbox(sgl::sgui::Widget * parent)
        : sgl::sgui::AbstractButton(parent)
    {
        using namespace sgl;

        SetCheckable(true);

        // BAR
        mBg = new graphic::Image;
        RegisterRenderable(mBg);

        // UPDATE CONTENT
        UpdateGraphics(NORMAL);
        UpdatePositions();
    }

private:
    void HandleButtonDown() override
    {
        sgl::sgui::AbstractButton::HandleButtonDown();

        auto player = sgl::media::AudioManager::Instance()->GetPlayer();
        player->PlaySound("UI/checkbox-01.ogg");
    }

    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override
    {
        UpdateGraphics(state);
    }

    void HandlePositionChanged() override
    {
        UpdatePositions();
    }

    void UpdateGraphics(sgl::sgui::AbstractButton::VisualState state)
    {
        using namespace sgl;

        auto tm = graphic::TextureManager::Instance();
        graphic::Texture * tex = nullptr;

        if(IsChecked())
        {
            if(IsEnabled())
            {
                if(MOUSE_OVER == state)
                    tex = tm->GetSprite(SpriteFileSettings, IND_SET_CHB_CHECKED_OVER);
                else if(PUSHED == state)
                    tex = tm->GetSprite(SpriteFileSettings, IND_SET_CHB_CHECKED_PUSHED);
                else
                    tex = tm->GetSprite(SpriteFileSettings, IND_SET_CHB_CHECKED_NORMAL);
            }
            else
                tex = tm->GetSprite(SpriteFileSettings, IND_SET_CHB_CHECKED_DISABLED);
        }
        else
        {
            if(IsEnabled())
            {
                if(MOUSE_OVER == state)
                    tex = tm->GetSprite(SpriteFileSettings, IND_SET_CHB_OVER);
                else if(PUSHED == state)
                    tex = tm->GetSprite(SpriteFileSettings, IND_SET_CHB_PUSHED);
                else
                    tex = tm->GetSprite(SpriteFileSettings, IND_SET_CHB_NORMAL);
            }
            else
                tex = tm->GetSprite(SpriteFileSettings, IND_SET_CHB_DISABLED);
        }

        mBg->SetTexture(tex);

        SetSize(mBg->GetWidth(), mBg->GetHeight());
    }

    void UpdatePositions()
    {
        const int x0 = GetScreenX();
        const int y0 = GetScreenY();
        mBg->SetPosition(x0, y0);
    }

private:
    sgl::graphic::Image * mBg = nullptr;
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
    const int marginButtonsTop = 65;
    const int marginPanelTop = 105;

    int x, y;

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
    font = fm->GetFont("Lato-Regular.ttf", 30, graphic::Font::NORMAL);
    auto labelTitle = new sgui::Label("SETTINGS", font, win);

    labelTitle->SetColor(colorTitle);
    labelTitle->SetPosition(marginContLeft, marginContTop);

    // BUTTONS PANEL
    mGroupButtons = new sgl::sgui::AbstractButtonsGroup;

    x = marginContLeft;
    y = marginButtonsTop;

    auto btn = new ButtonPanel("GAME", win);
    btn->SetPosition(x, y);
    mGroupButtons->AddButton(btn);

    x += btn->GetWidth();

    btn = new ButtonPanel("AUDIO", win);
    btn->SetPosition(x, y);
    mGroupButtons->AddButton(btn);

    x += btn->GetWidth();

    btn = new ButtonPanel("VIDEO", win);
    btn->SetPosition(x, y);
    mGroupButtons->AddButton(btn);

    x += btn->GetWidth();

    btn = new ButtonPanel("CONTROLS", win);
    btn->SetPosition(x, y);
    mGroupButtons->AddButton(btn);

    mGroupButtons->SetFunctionOnToggle([this](unsigned int index, bool checked)
    {
        for(unsigned int i = 0; i < Panel::NUM_PANELS; ++i)
            mPanels[i]->SetVisible(i == index);
    });

    // PANEL CONTENT
    x = marginContLeft;
    y += btn->GetHeight();

    CreatePanelGame(win);
    mPanels[Panel::GAME]->SetPosition(x, y);

    CreatePanelAudio(win);
    mPanels[Panel::AUDIO]->SetPosition(x, y);

    CreatePanelVideo(win);
    mPanels[Panel::VIDEO]->SetPosition(x, y);

    CreatePanelControls(win);
    mPanels[Panel::CONTROLS]->SetPosition(x, y);

    // default panel is GAME
    mGroupButtons->SetButtonChecked(Panel::GAME, true);
}

ScreenSettings::~ScreenSettings()
{
    sgl::sgui::Stage::Instance()->ClearWidgets();

    delete mGroupButtons;

    delete mBg;
}

void ScreenSettings::Update(float update)
{
}

void ScreenSettings::Render()
{
    mBg->Render();
}

void ScreenSettings::CreatePanelGame(sgl::sgui::Widget * parent)
{
    const int h =150;
    mPanels[Panel::GAME] = new PanelContentSettings(h, parent);
}

void ScreenSettings::CreatePanelAudio(sgl::sgui::Widget *parent)
{
    using namespace sgl;

    const int h = 450;
    auto panel = new PanelContentSettings(h, parent);
    mPanels[Panel::AUDIO] = panel;

    const int x0 = 30;
    const int y0 = 40;

    int x = x0;
    int y = y0;

    auto fm = graphic::FontManager::Instance();
    graphic::Font * font = fm->GetFont(fontTxt, sizeTxt, graphic::Font::NORMAL);

    auto am = media::AudioManager::Instance();
    auto ap = am->GetPlayer();

    auto tm = graphic::TextureManager::Instance();

    // MUSIC ENABLED
    auto label = new sgui::Label("MUSIC", font, panel);
    label->SetColor(colorTxt);
    label->SetPosition(x, y);

    auto cb = new SettingsCheckbox(panel);
    cb->SetChecked(ap->IsMusicEnabled());

    x += blockSettingW;
    y += (label->GetHeight() - cb->GetHeight()) * 0.5;
    cb->SetPosition(x, y);

    cb->AddOnToggleFunction([ap](bool checked)
    {
        ap->SetMusicEnabled(checked);

        // restart music if re-enabled
        if(checked)
            ap->PlayMusic("menus/menu_01.ogg");
    });

    // SOUNDS ENABLED
    x = x0;
    y = y0 + blockSettingH;

    label = new sgui::Label("SOUNDS", font, panel);
    label->SetColor(colorTxt);
    label->SetPosition(x, y);

    cb = new SettingsCheckbox(panel);
    cb->SetChecked(ap->IsSoundEnabled());

    x += blockSettingW;
    y += (label->GetHeight() - cb->GetHeight()) * 0.5;
    cb->SetPosition(x, y);

    cb->AddOnToggleFunction([ap](bool checked)
    {
        ap->SetSoundEnabled(checked);
    });

    // MUSIC VOLUME
    x = x0;
    y = y0 + blockSettingH * 2;

    const int volumeMin = 0;
    const int volumeMax = 100;
    const int volumeStep = 5;

    const int marginSliderR = 30;

    label = new sgui::Label("MUSIC VOLUME", font, panel);
    label->SetColor(colorTxt);
    label->SetPosition(x, y);

    graphic::Texture * texSliderBg = tm->GetSprite(SpriteFileSettingsExp,IND_SET_SLIDERH_BG);
    graphic::Texture * texSliderBar = tm->GetSprite(SpriteFileSettingsExp,IND_SET_SLIDERH_BAR);
    graphic::Texture * texSliderBtn = tm->GetSprite(SpriteFileSettingsExp,IND_SET_SLIDERH_BUTTON);

    auto slider = new GameSliderH(texSliderBg, texSliderBar, texSliderBtn, panel);
    slider->SetMinMax(volumeMin, volumeMax);
    slider->SetStep(volumeStep);
    slider->SetValue(am->GetVolumeMusic());

    x += blockSettingW;
    y += (label->GetHeight() - slider->GetHeight()) * 0.5;
    slider->SetPosition(x, y);

    label = new sgui::Label(std::to_string(slider->GetValue()).c_str(), font, panel);
    label->SetColor(colorTxtSlider);
    label->SetPosition(slider->GetX() + slider->GetWidth() + marginSliderR, slider->GetY());

    slider->SetOnValueChanged([label, am](int val)
    {
        am->SetVolumeMusic(val);

        label->SetText(std::to_string(val).c_str());
    });

    // SOUNDS VOLUME
    x = x0;
    y = y0 + blockSettingH * 3;

    label = new sgui::Label("SOUNDS VOLUME", font, panel);
    label->SetColor(colorTxt);
    label->SetPosition(x, y);

    slider = new GameSliderH(texSliderBg, texSliderBar, texSliderBtn, panel);
    slider->SetMinMax(volumeMin, volumeMax);
    slider->SetStep(volumeStep);
    slider->SetValue(am->GetVolumeSound());

    x += blockSettingW;
    y += (label->GetHeight() - slider->GetHeight()) * 0.5;
    slider->SetPosition(x, y);

    label = new sgui::Label(std::to_string(slider->GetValue()).c_str(), font, panel);
    label->SetColor(colorTxtSlider);
    label->SetPosition(slider->GetX() + slider->GetWidth() + marginSliderR, slider->GetY());

    slider->SetOnValueChanged([label, am](int val)
    {
        am->SetVolumeSound(val);

        label->SetText(std::to_string(val).c_str());
    });
}

void ScreenSettings::CreatePanelVideo(sgl::sgui::Widget * parent)
{
    using namespace sgl;

    const int h = 340;
    auto panel = new PanelContentSettings(h, parent);
    mPanels[Panel::VIDEO] = panel;

    const int x0 = 30;
    const int y0 = 40;

    int x = x0;
    int y = y0;

    auto fm = graphic::FontManager::Instance();
    graphic::Font * font = fm->GetFont(fontTxt, sizeTxt, graphic::Font::NORMAL);

    // RESOLUTION
    auto label = new sgui::Label("RESOLUTION", font, panel);
    label->SetColor(colorTxt);
    label->SetPosition(x, y);

    auto combo = new SettingsComboBox(panel);

    auto win = graphic::Window::Instance();

    const sgl::graphic::DisplayMode currDM = win->GetCurrentDisplayMode();

    int currIndex = -1;
    int validModes = 0;

    for(int d = 0; d < win->GetNumDisplays(); ++d)
    {
        for(int m = 0; m < win->GetNumDisplayModes(d); ++m)
        {
            graphic::DisplayMode dm = win->GetDisplayMode(d, m);

            // display requirements
            const int minW = 1280;
            const float minRatio = 1.6f;
            const float ratio = static_cast<float>(dm.width) / static_cast<float>(dm.height);

            if(ratio < minRatio || dm.width < minW)
                continue ;

            /// add combo item
            std::ostringstream oss;
            oss << dm.width << "x" << dm.height << " @ " << dm.refresh << "Hz";
            combo->AddItem(new ComboBoxItemResolution(d, m, oss.str().c_str()));

            // record current mode
            if(dm.width == currDM.width && dm.height == currDM.height && dm.refresh == currDM.refresh)
                currIndex = validModes;

            ++validModes;
        }

        combo->SetActiveItem(currIndex);

        // NOTE only handling display 0 for now, this might change later
        break ;
    }

    combo->SetOnActiveChanged([combo, win](int)
    {
        auto item = static_cast<const ComboBoxItemResolution *>(combo->GetActiveItem());

        win->SetDisplayMode(item->GetDisplay(), item->GetMode());
    });

    x += blockSettingW;
    y += (label->GetHeight() - combo->GetHeight()) * 0.5;
    combo->SetPosition(x, y);

    // FULLSCREEN
    x = x0;
    y = y0 + blockSettingH;

    label = new sgui::Label("FULLSCREEN", font, panel);
    label->SetColor(colorTxt);
    label->SetPosition(x, y);

    auto cb = new SettingsCheckbox(panel);
    cb->SetChecked(graphic::Window::Instance()->IsFullscreen());

    x += blockSettingW;
    y += (label->GetHeight() - cb->GetHeight()) * 0.5;
    cb->SetPosition(x, y);

    cb->AddOnToggleFunction([](bool checked)
    {
        graphic::Window::Instance()->SetFullscreen(checked);
    });

    // VSYNC
    x = x0;
    y = y0 + blockSettingH * 2;

    label = new sgui::Label("VSYNC", font, panel);
    label->SetColor(colorTxt);
    label->SetPosition(x, y);

    auto label2 = new sgui::Label("ON (not implemented yet)", font, panel);
    label2->SetColor(colorTxt);

    x += blockSettingW;
    label2->SetPosition(x, y);
}

void ScreenSettings::CreatePanelControls(sgl::sgui::Widget * parent)
{
    const int h = 650;
    mPanels[Panel::CONTROLS] = new PanelContentSettings(h, parent);
}

} // namespace game
