#include "Widgets/DialogExploreTemple.h"

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
#include <sgl/sgui/Label.h>

namespace game
{

// ===== BUTTON =====
class ButtonExploreTemple : public GameButton
{
public:
    ButtonExploreTemple(sgl::sgui::Widget * parent)
        : GameButton(SpriteFileDialogExploreTemple,
        { ID_DLG_EXTM_BTN_NORMAL, ID_DLG_EXTM_BTN_DISABLED,
          ID_DLG_EXTM_BTN_OVER, ID_DLG_EXTM_BTN_PUSHED, ID_DLG_EXTM_BTN_PUSHED },
        { 0xc3dfeeff, 0x5a6266ff, 0xebf4f9ff, 0xc3dfeeff, 0xc3dfeeff },
        parent)
    {
        using namespace sgl;

        const char * fileFont = "Lato-Regular.ttf";
        const int size = 24;

        auto fm = graphic::FontManager::Instance();
        graphic::Font * fnt = fm->GetFont(fileFont, size, graphic::Font::NORMAL);
        SetLabelFont(fnt);
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

// ===== DIALOG EXPLORE =====
const int marginSide = 40;

DialogExploreTemple::DialogExploreTemple()
{
    using namespace sgl;

    auto fm = graphic::FontManager::Instance();
    auto tm = graphic::TextureManager::Instance();

    const char * headerFontFile = "Lato-Regular.ttf";
    const int headerFontSize = 22;
    const unsigned int colorHeader = 0x9dcbe2ff;
    const unsigned int colorLabel = 0x70a7c2ff;

    // BACKGROUND
    graphic::Texture * tex = tm->GetSprite(SpriteFileDialogExploreTemple, ID_DLG_EXTM_BACKGROUND);
    mBg = new graphic::Image(tex);
    RegisterRenderable(mBg);

    const int w = mBg->GetWidth();
    const int h = mBg->GetHeight();
    SetSize(w, h);

    // TITLE
    auto font = fm->GetFont("Lato-Regular.ttf", 32, graphic::Font::NORMAL);

    sgui::Label * title = new sgui::Label("EXPLORE ABANDONED TEMPLE", font, this);

    const int titleX = (w - title->GetWidth()) / 2;
    const int titleY = 10;
    title->SetPosition(titleX, titleY);
    title->SetColor(0xf1f3f4ff);

    // HORIZ BAR 1
    tex = tm->GetSprite(SpriteFileDialogExploreTemple, ID_DLG_EXTM_LINE_H);
    mLineH1 = new graphic::Image(tex);
    RegisterRenderable(mLineH1);

    // HORIZ BAR 2
    mLineH2 = new graphic::Image(tex);
    RegisterRenderable(mLineH2);

    // -- INVESTING PANEL --
    font = fm->GetFont(headerFontFile, headerFontSize, graphic::Font::NORMAL);

    // HEADER INVEST
    mHeaderInvest = new graphic::Text("INVEST RESOURCES", font);
    mHeaderInvest->SetColor(colorHeader);
    RegisterRenderable(mHeaderInvest);

    // -- OUTCOME PANEL --
    font = fm->GetFont(headerFontFile, headerFontSize, graphic::Font::NORMAL);

    mHeaderTime = new graphic::Text("TIME REQUIRED", font);
    mHeaderTime->SetColor(colorHeader);
    RegisterRenderable(mHeaderTime);

    mLabelTime = new sgui::Label("?", font, this);
    mLabelTime->SetColor(colorLabel);

    mHeaderSuccess = new graphic::Text("SUCCESS PROBABILITY", font);
    mHeaderSuccess->SetColor(colorHeader);
    RegisterRenderable(mHeaderSuccess);

    mLabelSuccess = new sgui::Label("?", font, this);
    mLabelSuccess->SetColor(colorLabel);

    // -- BUTTONS --
    const int blockW = (w - (marginSide * 2)) / 2;

    // BUTTON ABORT
    mBtnAbort = new ButtonExploreTemple(this);
    mBtnAbort->SetLabel("ABORT");

    // BUTTON EXPLORE
    mBtnExplore = new ButtonExploreTemple(this);
    mBtnExplore->SetLabel("EXPLORE");

    // position buttons
    const int marginButtonsB = 20;
    const int button1X0 = marginSide + (blockW - mBtnAbort->GetWidth()) / 2;
    const int buttonsY = h - marginButtonsB - mBtnAbort->GetHeight();

    mBtnAbort->SetPosition(button1X0, buttonsY);

    const int button2X0 = marginSide + blockW + (blockW - mBtnExplore->GetWidth()) / 2;
    mBtnExplore->SetPosition(button2X0, buttonsY);
}

void DialogExploreTemple::SetFunctionOnClose(const std::function<void()> & f)
{
    mBtnAbort->AddOnClickFunction(f);
}

void DialogExploreTemple::HandlePositionChanged()
{
    SetPositions();
}

void DialogExploreTemple::SetPositions()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();
    const int w = mBg->GetWidth();
    const int blockW = (w - (marginSide * 2)) / 2;

    // BACKGROUND
    mBg->SetPosition(x0, y0);

    // HORIZ LINE 1
    const int lineX = x0 + (w - mLineH1->GetWidth()) / 2;
    const int line1Y = y0 + 280;
    mLineH1->SetPosition(lineX, line1Y);

    // HORIZ LINE 2
    const int line2Y = y0 + 410;
    mLineH2->SetPosition(lineX, line2Y);

    // -- INVESTING PANEL --
    const int marginPanel1V = 85;

    // HEADER INVEST
    const int headerInvestX = x0 + marginSide;
    const int headerInvestY = y0 + marginPanel1V;
    mHeaderInvest->SetPosition(headerInvestX, headerInvestY);

    // -- OUTCOME PANEL --
    const int marginPanel2V = 37;
    const int marginHeaderB = 5;

    // HEADER TIME
    const int headerTimeX = x0 + marginSide + (blockW - mHeaderTime->GetWidth()) / 2;
    const int headerTimeY = line1Y + marginPanel2V;
    mHeaderTime->SetPosition(headerTimeX, headerTimeY);

    // LABEL TIME
    const int labelTimeX = headerTimeX + (mHeaderTime->GetWidth() - mLabelTime->GetWidth()) / 2 - x0;
    const int labelTimeY = headerTimeY + mHeaderTime->GetHeight() + marginHeaderB - y0;
    mLabelTime->SetPosition(labelTimeX, labelTimeY);

    // HEADER SUCCESS
    const int headerSuccessX = x0 + marginSide + blockW + (blockW - mHeaderSuccess->GetWidth()) / 2;
    mHeaderSuccess->SetPosition(headerSuccessX, headerTimeY);

    // LABEL SUCCESS
    const int labelSuccessX = headerSuccessX + (mHeaderSuccess->GetWidth() - mLabelSuccess->GetWidth()) / 2  - x0;
    mLabelSuccess->SetPosition(labelSuccessX, labelTimeY);
}

} // namespace game
