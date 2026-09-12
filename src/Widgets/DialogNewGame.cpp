#include "Widgets/DialogNewGame.h"

#include "Game.h"
#include "GameConstants.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonDialogBack.h"
#include "Widgets/ButtonDialogOk.h"
#include "Widgets/GameUIData.h"
#include "Widgets/WidgetsConstants.h"

#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/GraphicConstants.h>
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
#include <sgl/sgui/ImageButton.h>
#include <sgl/sgui/Label.h>
#include <sgl/sgui/TextArea.h>
#include <sgl/utilities/StringManager.h>

namespace
{

using namespace game;

// ----- CONSTANTS -----
constexpr int paddingPanelL = 20;
constexpr int paddingPanelT = 10;

constexpr int sizeFontPanelTitle = 26;

constexpr int panelContY0 = 95;


// ----- PRIMARY BUTTON -----
class ButtonChoice : public sgl::sgui::ImageButton
{
public:
    ButtonChoice(const char * title, const char * content, sgl::sgui::Widget * parent)
        : sgl::sgui::ImageButton({ ID_DLG_NEWG_LEFT_NORMAL, ID_DLG_NEWG_LEFT_DISABLED,
                                   ID_DLG_NEWG_LEFT_OVER, ID_DLG_NEWG_LEFT_NORMAL,
                                   ID_DLG_NEWG_LEFT_CHECKED },
                                 SpriteFileDialogNewGame, parent)
    {
        using namespace sgl;

        auto fm = graphic::FontManager::Instance();

        // TITLE
        const int titleX0 = 20;
        const int titleY0 = 20;
        const int sizeTitle = 24;
        auto fontTitle = fm->GetFont(WidgetsConstants::FontFilePanelHeader, sizeTitle,
                                     graphic::Font::NORMAL);

        mTitle = new sgui::Label(title, fontTitle, this);
        mTitle->SetPosition(titleX0, titleY0);

        // DATA
        if(content != nullptr)
        {
            const int sizeData = 18;
            auto fontData = fm->GetFont(WidgetsConstants::FontFilePanelText, sizeData,
                                         graphic::Font::NORMAL);

            const int areaW = 500;
            const int areaH = 120;
            const int areaY = 60;
            mText = new sgui::TextArea(areaW, areaH, content, fontData, false, this);
            mText->SetPosition(titleX0, areaY);
        }

        UpdateColors(GetState());

        SetCheckable(true);
    }

private:
    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override
    {
        sgl::sgui::ImageButton::OnStateChanged(state);

        UpdateColors(state);
    }

    void UpdateColors(sgl::sgui::AbstractButton::VisualState state)
    {
        if(state == sgl::sgui::AbstractButton::DISABLED)
        {
            mTitle->SetColor(0x6d7073ff);
            mText->SetColor(0x6d7073ff);
        }
        else
        {
            mTitle->SetColor(0xe6eef2ff);
            mText->SetColor(WidgetsConstants::colorPanelText);
        }
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

private:
    sgl::sgui::Label * mTitle = nullptr;
    sgl::sgui::TextArea * mText = nullptr;
};
}

namespace game
{

DialogNewGame::DialogNewGame::DialogNewGame(Game * game)
    : mGame(game)
{
    using namespace sgl;

    auto fm = graphic::FontManager::Instance();
    auto tm = graphic::TextureManager::Instance();
    auto sm = utilities::StringManager::Instance();

    // BACKGROUND
    const int w = 1882;
    graphic::Texture * tex;

    tex = tm->GetSprite(SpriteFileDialogNewGame, ID_DLG_NEWG_BG_L);
    mBgL = new graphic::Image(tex);
    RegisterRenderable(mBgL);

    const int wL = mBgL->GetWidth();
    const int h = mBgL->GetHeight();

    tex = tm->GetSprite(SpriteFileDialogNewGame, ID_DLG_NEWG_BG_R);
    mBgR = new graphic::Image(tex);
    RegisterRenderable(mBgR);

    const int wR = mBgR->GetWidth();

    tex = tm->GetTexture(SpriteFileDialogNewGameExp);
    tex->SetScaleMode(graphic::TSCALE_NEAREST);
    mBgC = new graphic::Image(tex);
    RegisterRenderable(mBgC);

    const int wC = w - wL - wR;
    mBgC->SetWidth(wC);

    SetSize(w, h);

    // BUTTON CLOSE
    mButtonClose = new ButtonDialogBack(this);
    mButtonClose->SetX(w - mButtonClose->GetWidth());

    mButtonClose->AddOnClickFunction([this]
    {
        mGame->RequestNextActiveState(StateId::LEAVE_GAME);
    });

    // TITLE
    auto fontTitle = fm->GetFont(WidgetsConstants::FontFileDialogTitle,
                                 WidgetsConstants::FontSizeDialogTitle, graphic::Font::NORMAL);

    auto title = new sgui::Label(sm->GetCString("NEW_GAME"), fontTitle, this);

    const int titleX = WidgetsConstants::MarginDialogTitleL;
    const int titleY = (WidgetsConstants::DialogTitleBarH - title->GetHeight()) / 2;
    title->SetPosition(titleX, titleY);
    title->SetColor(WidgetsConstants::colorDialogTitle);

    // PANELS
    CreatePanelMode();

    // BUTTON CONTINUE
    const int marginB = 20;

    mButtonContinue = new ButtonDialogOk(sm->GetCString("CONTINUE"), this);
    mButtonContinue->SetEnabled(false);
    mButtonContinue->SetPosition(w - mButtonContinue->GetWidth() - WidgetsConstants::MarginDialogContentR,
                                 h - mButtonContinue->GetHeight() - marginB);

    mButtonContinue->AddOnClickFunction([this]
    {
        mGame->RequestNextActiveState(StateId::FACTION_SEL);
    });
}

DialogNewGame::~DialogNewGame()
{
    delete mButtonsMode;
    delete mButtonsDiff;
    delete mButtonsSett;
}

void DialogNewGame::CreatePanelMode()
{
    using namespace sgl;

    if(mPanelMode != nullptr)
        return ;

    auto fm = graphic::FontManager::Instance();
    auto tm = graphic::TextureManager::Instance();
    auto sm = utilities::StringManager::Instance();

    // BACKGROUND
    graphic::Texture * tex = tm->GetSprite(SpriteFileDialogNewGame, ID_DLG_NEWG_PANEL);
    mPanelMode = new sgui::Image(tex, this);

    mPanelMode->SetPosition(WidgetsConstants::MarginDialogContentL,
                            WidgetsConstants::DialogTitleBarH + WidgetsConstants::MarginDialogContentT);

    // PANEL TITLE
    auto fontTitle = fm->GetFont(WidgetsConstants::FontFilePanelHeader, sizeFontPanelTitle,
                                 graphic::Font::NORMAL);

    auto title = new sgui::Label(sm->GetCString("GAME_MODE"), fontTitle, mPanelMode);
    title->SetPosition(paddingPanelL, paddingPanelT);
    title->SetColor(WidgetsConstants::colorDialogTitle);

    // BUTTONS
    const int spacingButtons = 10;
    int btnY = panelContY0;

    mButtonsMode = new sgui::AbstractButtonsGroup;

    mButtonsMode->AddFunctionOnToggle([this](unsigned int ind, bool checked)
    {
        if(checked)
            CreatePanelDifficulty();
    });

#ifdef DEMO_MODE
    const char * descSingleMission = sm->GetCString("NOT_IN_DEMO");
    const char * descEndless = sm->GetCString("NOT_IN_DEMO");
#else
    const char * descSingleMission = sm->GetCString("SMISSION_DESC");
    const char * descEndless = sm->GetCString("ENDLESS_DESC");
#endif

    // CHOICE CAMPAIGN
    auto btn = new ButtonChoice(sm->GetCString("FCAMPAIGN"), sm->GetCString("FCAMPAIGN_DESC"),
                                mPanelMode);
    btn->SetPosition(paddingPanelL, btnY);

    mButtonsMode->AddButton(btn);

    btnY +=btn->GetHeight() + spacingButtons;

    // CHOICE SINGLE MISSION
    btn = new ButtonChoice(sm->GetCString("SMISSION"), descSingleMission, mPanelMode);
    // TODO enable when implemented
    btn->SetEnabled(false);
    btn->SetPosition(paddingPanelL, btnY);

    mButtonsMode->AddButton(btn);

    btnY +=btn->GetHeight() + spacingButtons;

    // CHOICE ENDLESS
    btn = new ButtonChoice(sm->GetCString("ENDLESSM"), descEndless, mPanelMode);
    // TODO enable when implemented
    btn->SetEnabled(false);
    btn->SetPosition(paddingPanelL, btnY);

    mButtonsMode->AddButton(btn);
}

void DialogNewGame::CreatePanelDifficulty()
{
    using namespace sgl;

    if(mPanelDiff != nullptr)
        return ;

    auto fm = graphic::FontManager::Instance();
    auto tm = graphic::TextureManager::Instance();
    auto sm = utilities::StringManager::Instance();

    // BACKGROUND
    graphic::Texture * tex = tm->GetSprite(SpriteFileDialogNewGame, ID_DLG_NEWG_PANEL);
    mPanelDiff = new sgui::Image(tex, this);

    mPanelDiff->SetPosition((GetWidth() - mPanelDiff->GetWidth()) / 2,
                            WidgetsConstants::DialogTitleBarH + WidgetsConstants::MarginDialogContentT);

    // PANEL TITLE
    auto fontTitle = fm->GetFont(WidgetsConstants::FontFilePanelHeader, sizeFontPanelTitle,
                                 graphic::Font::NORMAL);

    auto title = new sgui::Label(sm->GetCString("DIFFICULTY"), fontTitle, mPanelDiff);
    title->SetPosition(paddingPanelL, paddingPanelT);
    title->SetColor(WidgetsConstants::colorDialogTitle);
}

void DialogNewGame::CreatePanelSettings()
{

}

void DialogNewGame::HandlePositionChanged()
{
    SetPositions();
}

void DialogNewGame::SetPositions()
{
    const int y = GetScreenY();
    int x = GetScreenX();

    // BACKGROUND
    mBgL->SetPosition(x, y);
    x += mBgL->GetWidth();

    mBgC->SetPosition(x, y);
    x += mBgC->GetWidth();

    mBgR->SetPosition(x, y);
}

} // namespace game