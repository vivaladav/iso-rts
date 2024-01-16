#include "Widgets/DialogExploreTemple.h"

#include "Game.h"
#include "Player.h"
#include "GameObjects/Temple.h"
#include "Widgets/GameButton.h"
#include "Widgets/GameSliderH.h"
#include "Widgets/GameUIData.h"

#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/media/AudioManager.h>
#include <sgl/media/AudioPlayer.h>
#include <sgl/sgui/Image.h>
#include <sgl/sgui/Label.h>

#include <sstream>

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

DialogExploreTemple::DialogExploreTemple(Player * player, Temple * temple)
    : mPlayer(player)
    , mTemple(temple)
{
    using namespace sgl;

    auto fm = graphic::FontManager::Instance();
    auto tm = graphic::TextureManager::Instance();

    const char * headerFontFile = "Lato-Regular.ttf";
    const int headerFontSize = 22;
    const char * labelFontFile = "Lato-Regular.ttf";
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
    auto fontTitle = fm->GetFont("Lato-Regular.ttf", 32, graphic::Font::NORMAL);

    sgui::Label * title = new sgui::Label("EXPLORE ABANDONED TEMPLE", fontTitle, this);

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
    const int sliderLabelFontSize = 18;
    const int iconsX0 = marginSide;
    const int iconsY0 = 135;
    const int iconsX1 = 510;
    const int iconsY1 = 215;
    const int marginIconR = 20;
    const int marginSliderR = 20;

    const int minInvest = 0;

    graphic::Texture * texSliderBg = tm->GetSprite(SpriteFileDialogExploreTemple, ID_DLG_EXTM_SLIDER_BG);
    graphic::Texture * texSliderBar = tm->GetSprite(SpriteFileDialogExploreTemple, ID_DLG_EXTM_SLIDER_BAR);
    graphic::Texture * texSliderBtn = tm->GetSprite(SpriteFileDialogExploreTemple, ID_DLG_EXTM_SLIDER_BTN);

    auto fontHeader = fm->GetFont(headerFontFile, headerFontSize, graphic::Font::NORMAL);
    auto fontLabel = fm->GetFont(labelFontFile, sliderLabelFontSize, graphic::Font::NORMAL);

    // HEADER INVEST
    mHeaderInvest = new graphic::Text("INVEST RESOURCES", fontHeader);
    mHeaderInvest->SetColor(colorHeader);
    RegisterRenderable(mHeaderInvest);

    // MONEY
    const int playerMoney = player->GetStat(Player::Stat::MONEY).GetIntValue();
    const int maxTempleMoney = temple->GetMaxInvestableMoney();
    const int maxMoney =  playerMoney < maxTempleMoney ? playerMoney : maxTempleMoney;

    tex = tm->GetSprite(SpriteFileDialogExploreTemple, ID_DLG_EXTM_ICON_MONEY);

    auto icon = new sgui::Image(tex, this);
    icon->SetColor(colorHeader);
    icon->SetPosition(iconsX0, iconsY0);

    mSliderMoney = new GameSliderH(texSliderBg, texSliderBar, texSliderBtn, this);
    mSliderMoney->SetMinMax(minInvest, maxMoney);
    mSliderMoney->SetValue(minInvest + (maxMoney - minInvest) / 2);
    mSliderMoney->SetPosition(icon->GetX() + icon->GetWidth() + marginIconR,
                              icon->GetY() + (icon->GetHeight() - mSliderMoney->GetHeight()) / 2);

    auto label = new sgui::Label(std::to_string(mSliderMoney->GetValue()).c_str(), fontLabel, this);
    label->SetColor(colorLabel);
    label->SetPosition(mSliderMoney->GetX() + mSliderMoney->GetWidth() + marginSliderR,
                       mSliderMoney->GetY());

    mSliderMoney->SetOnValueChanged([this, label](int val)
    {
        label->SetText(std::to_string(val).c_str());

        OnInvestmentChanged();
    });

    // MATERIAL
    const int playerMaterial = player->GetStat(Player::Stat::MATERIAL).GetIntValue();
    const int maxTempleMaterial = temple->GetMaxInvestableMaterial();
    const int maxMaterial =  playerMaterial < maxTempleMaterial ? playerMaterial : maxTempleMaterial;

    tex = tm->GetSprite(SpriteFileDialogExploreTemple, ID_DLG_EXTM_ICON_MATERIAL);

    icon = new sgui::Image(tex, this);
    icon->SetColor(colorHeader);
    icon->SetPosition(iconsX0, iconsY1);

    mSliderMaterial = new GameSliderH(texSliderBg, texSliderBar, texSliderBtn, this);
    mSliderMaterial->SetMinMax(minInvest, maxMaterial);
    mSliderMaterial->SetValue(minInvest + (maxMaterial - minInvest) / 2);
    mSliderMaterial->SetPosition(icon->GetX() + icon->GetWidth() + marginIconR,
                                 icon->GetY() + (icon->GetHeight() - mSliderMoney->GetHeight()) / 2);

    label = new sgui::Label(std::to_string(mSliderMaterial->GetValue()).c_str(), fontLabel, this);
    label->SetColor(colorLabel);
    label->SetPosition(mSliderMaterial->GetX() + mSliderMaterial->GetWidth() + marginSliderR,
                       mSliderMaterial->GetY());

    mSliderMaterial->SetOnValueChanged([this, label](int val)
    {
        label->SetText(std::to_string(val).c_str());

        OnInvestmentChanged();
    });

    // BLOBS
    const int playerBlobs = player->GetStat(Player::Stat::BLOBS).GetIntValue();
    const int maxTempleBlobs = temple->GetMaxInvestableBlobs();
    const int maxBlobs =  playerBlobs < maxTempleBlobs ? playerBlobs : maxTempleBlobs;

    tex = tm->GetSprite(SpriteFileDialogExploreTemple, ID_DLG_EXTM_ICON_BLOBS);

    icon = new sgui::Image(tex, this);
    icon->SetColor(colorHeader);
    icon->SetPosition(iconsX1, iconsY0);

    mSliderBlobs = new GameSliderH(texSliderBg, texSliderBar, texSliderBtn, this);
    mSliderBlobs->SetMinMax(minInvest, maxBlobs);
    mSliderBlobs->SetValue(minInvest + (maxBlobs - minInvest) / 2);
    mSliderBlobs->SetPosition(icon->GetX() + icon->GetWidth() + marginIconR,
                              icon->GetY() + (icon->GetHeight() - mSliderBlobs->GetHeight()) / 2);

    label = new sgui::Label(std::to_string(mSliderBlobs->GetValue()).c_str(), fontLabel, this);
    label->SetColor(colorLabel);
    label->SetPosition(mSliderBlobs->GetX() + mSliderBlobs->GetWidth() + marginSliderR,
                       mSliderBlobs->GetY());

    mSliderBlobs->SetOnValueChanged([this, label](int val)
    {
        label->SetText(std::to_string(val).c_str());

        OnInvestmentChanged();
    });

    // DIAMONDS
    const int playerDiamonds = player->GetStat(Player::Stat::DIAMONDS).GetIntValue();
    const int maxTempleDiamonds = temple->GetMaxInvestableBlobs();
    const int maxDiamonds =  playerDiamonds < maxTempleDiamonds ? playerDiamonds : maxTempleDiamonds;

    tex = tm->GetSprite(SpriteFileDialogExploreTemple, ID_DLG_EXTM_ICON_DIAMONDS);

    icon = new sgui::Image(tex, this);
    icon->SetColor(colorHeader);
    icon->SetPosition(iconsX1, iconsY1);

    mSliderDiamonds = new GameSliderH(texSliderBg, texSliderBar, texSliderBtn, this);
    mSliderDiamonds->SetMinMax(minInvest, maxDiamonds);
    mSliderDiamonds->SetValue(minInvest + (maxBlobs - minInvest) / 2);
    mSliderDiamonds->SetPosition(icon->GetX() + icon->GetWidth() + marginIconR,
                                 icon->GetY() + (icon->GetHeight() - mSliderDiamonds->GetHeight()) / 2);

    label = new sgui::Label(std::to_string(mSliderDiamonds->GetValue()).c_str(), fontLabel, this);
    label->SetColor(colorLabel);
    label->SetPosition(mSliderDiamonds->GetX() + mSliderDiamonds->GetWidth() + marginSliderR,
                       mSliderDiamonds->GetY());

    mSliderDiamonds->SetOnValueChanged([this, label](int val)
    {
        label->SetText(std::to_string(val).c_str());

        OnInvestmentChanged();
    });

    // -- OUTCOME PANEL --
    mHeaderTime = new graphic::Text("TIME REQUIRED", fontHeader);
    mHeaderTime->SetColor(colorHeader);
    RegisterRenderable(mHeaderTime);

    mLabelTime = new sgui::Label(fontLabel, this);
    mLabelTime->SetColor(colorLabel);

    mHeaderSuccess = new graphic::Text("SUCCESS PROBABILITY", fontHeader);
    mHeaderSuccess->SetColor(colorHeader);
    RegisterRenderable(mHeaderSuccess);

    mLabelSuccess = new sgui::Label(fontLabel, this);
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

    // INIT VALUES
    OnInvestmentChanged();
}

void DialogExploreTemple::SetFunctionOnCancel(const std::function<void()> & f)
{
    mBtnAbort->AddOnClickFunction(f);
}

void DialogExploreTemple::SetFunctionOnExplore(const std::function<void()> & f)
{
    mBtnExplore->AddOnClickFunction(f);
}

void DialogExploreTemple::HandlePositionChanged()
{
    SetPositions();
}

void DialogExploreTemple::OnInvestmentChanged()
{
    // update invested resources
    mTemple->SetInvestedResources(mSliderMoney->GetValue(), mSliderMaterial->GetValue(),
                                  mSliderBlobs->GetValue(), mSliderDiamonds->GetValue());

    // update time
    int time = mTemple->GetExplorationTime();
    // mins
    const int secsInM = 60;
    const int timeM = time / secsInM;
    // secs
    time -= timeM * secsInM;
    const int timeS = time;

    const int fieldW = 2;
    const char fieldF = '0';
    std::ostringstream ss;
    ss.width(fieldW);
    ss.fill(fieldF);
    ss << timeM << ":";
    ss.width(fieldW);
    ss.fill(fieldF);
    ss << timeS;

    mLabelTime->SetText(ss.str().c_str());

    ss.str(std::string());
    ss.clear();

    // update success rate
    const int success = mTemple->GetExplorationSuccessRate();

    ss << success << "%";

    mLabelSuccess->SetText(ss.str().c_str());

    // reposition elements
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
    const int marginPanel2V = 40;
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
