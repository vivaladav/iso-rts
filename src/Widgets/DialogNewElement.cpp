#include "Widgets/DialogNewElement.h"

#include "Player.h"
#include "GameObjects/Unit.h"
#include "GameObjects/UnitData.h"
#include "Widgets/ButtonCloseDialog.h"
#include "Widgets/GameButton.h"
#include "Widgets/GameUIData.h"
#include "Widgets/ShortcutButton.h"

#include <core/event/KeyboardEvent.h>
#include <graphic/Camera.h>
#include <graphic/DummyRenderable.h>
#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/Text.h>
#include <graphic/TextureManager.h>
#include <sgui/ButtonsGroup.h>
#include <sgui/Image.h>
#include <sgui/Label.h>

namespace game
{

constexpr int NUM_SLOTS = 5;

// ===== BUTTON CLOSE =====

class ButtonClose : public ShortcutButton
{
public:
    ButtonClose(lib::sgui::Widget * parent)
        : ShortcutButton(lib::core::KeyboardEvent::KEY_ESC, parent)
        , mBody(new lib::graphic::Image)
    {
        // register graphic elements
        RegisterRenderable(mBody);

        // set initial visual state
        SetState(NORMAL);
    }

    ~ButtonClose() { delete mBody; }

private:
    void OnStateChanged(lib::sgui::PushButton::VisualState state) override
    {
        const unsigned int texIds[NUM_VISUAL_STATES] =
        {
            IND_NE_DIALOG_CLOSE_NORMAL,
            IND_NE_DIALOG_CLOSE_NORMAL,     // button can't be disabled
            IND_NE_DIALOG_CLOSE_OVER,
            IND_NE_DIALOG_CLOSE_PUSHED,
            IND_NE_DIALOG_CLOSE_NORMAL,     // button can't be checked
        };

        auto tm = lib::graphic::TextureManager::Instance();
        lib::graphic::Texture * tex = tm->GetSprite(SpriteFileNewElementDialog, texIds[state]);
        mBody->SetTexture(tex);
        // reset BG to make changes visible
        SetCurrBg(mBody);
    }

private:
    lib::graphic::Image * mBody = nullptr;
};

// ===== BUTTON BUILD =====

class ButtonBuild : public ShortcutButton
{
public:
    ButtonBuild(lib::sgui::Widget * parent)
        : ShortcutButton(lib::core::KeyboardEvent::KEY_B, parent)
        , mBody(new lib::graphic::Image)
    {
        using namespace lib::graphic;

        auto fm = FontManager::Instance();
        auto font = fm->GetFont("data/fonts/Lato-Bold.ttf", 11, Font::NORMAL);
        mShortcut = new Text("B", font, true);
        mShortcut->SetColor(0xd5daddff);

        // register graphic elements
        RegisterRenderable(mBody);
        RegisterRenderable(mShortcut);

        // LABEL
        font = fm->GetFont("data/fonts/Lato-Regular.ttf", 19, Font::NORMAL);
        SetLabelFont(font);
        SetLabel("BUILD");

        // set initial visual state
        SetState(NORMAL);
    }

    ~ButtonBuild()
    {
        delete mBody;
        delete mShortcut;
    }

private:
    void OnStateChanged(lib::sgui::PushButton::VisualState state) override
    {
        const unsigned int texIds[NUM_VISUAL_STATES] =
        {
            IND_NE_DIALOG_BUILD_NORMAL,
            IND_NE_DIALOG_BUILD_DISABLED,
            IND_NE_DIALOG_BUILD_OVER,
            IND_NE_DIALOG_BUILD_PUSHED,
            IND_NE_DIALOG_BUILD_PUSHED,
        };

        auto tm = lib::graphic::TextureManager::Instance();
        lib::graphic::Texture * tex = tm->GetSprite(SpriteFileNewElementDialog, texIds[state]);
        mBody->SetTexture(tex);
        // reset BG to make changes visible
        SetCurrBg(mBody);

        // update shortcut label alpha
        const unsigned char alphaEn = 255;
        const unsigned char alphaDis = 128;
        const unsigned char alphaLabel = DISABLED == state ? alphaDis : alphaEn;
        mShortcut->SetAlpha(alphaLabel);
    }

    void HandlePositionChanged() override
    {
        PushButton::HandlePositionChanged();

        // SHORTCUT
        const int shortBgX0 = 182;
        const int shortBgY0 = 22;
        const int shortBgSize = 14;

        const int shortcutX = GetScreenX() + shortBgX0 + (shortBgSize - mShortcut->GetWidth()) * 0.5f;
        const int shortcutY = GetScreenY() + shortBgY0 + (shortBgSize - mShortcut->GetHeight()) * 0.5f;

        mShortcut->SetPosition(shortcutX, shortcutY);
    }

    void OnRender() override
    {
        PushButton::OnRender();

        mShortcut->Render();
    }

private:
    lib::graphic::Image * mBody = nullptr;
    lib::graphic::Text * mShortcut = nullptr;
};

// ===== BUTTON SLOT =====

class ButtonSlot : public ShortcutButton
{
public:
    ButtonSlot(int index)
        : ShortcutButton(KEYS[index], nullptr)
        , mBody(new lib::graphic::Image)
    {
        using namespace lib::graphic;

        SetCheckable(true);

        auto fm = FontManager::Instance();
        auto font = fm->GetFont("data/fonts/Lato-Bold.ttf", 11, Font::NORMAL);
        mShortcut = new Text(SHORTCUTS[index], font, true);
        mShortcut->SetColor(0xd5daddff);

        // register graphic elements
        RegisterRenderable(mBody);
        RegisterRenderable(mShortcut);

        // set initial visual state
        SetState(NORMAL);
    }

    ~ButtonSlot()
    {
        delete mBody;
        delete mShortcut;
    }

private:
    void OnStateChanged(lib::sgui::PushButton::VisualState state) override
    {
        const unsigned int texIds[NUM_VISUAL_STATES] =
        {
            IND_NE_DIALOG_PANEL_NORMAL,
            IND_NE_DIALOG_PANEL_DISABLED,
            IND_NE_DIALOG_PANEL_OVER,
            IND_NE_DIALOG_PANEL_SEL,
            IND_NE_DIALOG_PANEL_SEL,
        };

        auto tm = lib::graphic::TextureManager::Instance();
        lib::graphic::Texture * tex = tm->GetSprite(SpriteFileNewElementDialog, texIds[state]);
        mBody->SetTexture(tex);
        // reset BG to make changes visible
        SetCurrBg(mBody);

        // update shortcut label alpha
        const unsigned char alphaEn = 255;
        const unsigned char alphaDis = 128;
        const unsigned char alphaLabel = DISABLED == state ? alphaDis : alphaEn;
        mShortcut->SetAlpha(alphaLabel);
    }

    void HandlePositionChanged() override
    {
        PushButton::HandlePositionChanged();

        // SHORTCUT
        const int shortBgX0 = 182;
        const int shortBgY0 = 182;
        const int shortBgSize = 14;

        const int shortcutX = GetScreenX() + shortBgX0 + (shortBgSize - mShortcut->GetWidth()) * 0.5f;
        const int shortcutY = GetScreenY() + shortBgY0 + (shortBgSize - mShortcut->GetHeight()) * 0.5f;

        mShortcut->SetPosition(shortcutX, shortcutY);
    }

    void OnRender() override
    {
        PushButton::OnRender();

        mShortcut->Render();
    }

private:
    static const int KEYS[NUM_SLOTS];
    static const char * SHORTCUTS[NUM_SLOTS];

    lib::graphic::Image * mBody = nullptr;
    lib::graphic::Text * mShortcut = nullptr;
};

const int ButtonSlot::KEYS[NUM_SLOTS] = {
                                            lib::core::KeyboardEvent::KEY_1,
                                            lib::core::KeyboardEvent::KEY_2,
                                            lib::core::KeyboardEvent::KEY_3,
                                            lib::core::KeyboardEvent::KEY_4,
                                            lib::core::KeyboardEvent::KEY_5,
                                        };


const char * ButtonSlot::SHORTCUTS[NUM_SLOTS] = { "1", "2", "3", "4", "5" };

// ===== ATTRIBUTE PANEL =====

class PanelAttribute : public lib::sgui::Widget
{
public:
    PanelAttribute(lib::sgui::Widget * parent)
        : lib::sgui::Widget(parent)
    {
        using namespace lib::graphic;

        auto tm = TextureManager::Instance();

        // BACKGROUND
        Texture * tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_ATT_OFF);
        mBg = new Image(tex);
        RegisterRenderable(mBg);

        SetSize(mBg->GetWidth(), mBg->GetHeight());

        // LABEL
        mLabel = new DummyRenderable;
        RegisterRenderable(mLabel);

        // VALUE BAR
        mValueBar = new DummyRenderable;
        RegisterRenderable(mValueBar);
    }

    ~PanelAttribute()
    {
        delete mBg;
        delete mLabel;
        delete mValueBar;
    }

    void ClearData()
    {
        using namespace lib::graphic;

        // check if already cleared
        if(!mHasData)
            return ;

        // BACKGROUND
        auto tm = TextureManager::Instance();
        Texture * tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_ATT_OFF);
        mBg->SetTexture(tex);

        // LABEL
        UnregisterRenderable(mLabel);
        delete mLabel;
        mLabel = new DummyRenderable;
        RegisterRenderable(mLabel);

        // VALUE BAR
        UnregisterRenderable(mValueBar);
        delete mValueBar;
        mValueBar = new DummyRenderable;
        RegisterRenderable(mValueBar);

        // update data flag
        mHasData = false;
    }

    void SetData(const char * txt, unsigned int val)
    {
        using namespace lib::graphic;

        // BACKGROUND
        auto tm = TextureManager::Instance();
        Texture * tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_ATT_ON);
        mBg->SetTexture(tex);

        // LABEL
        auto fm = FontManager::Instance();

        UnregisterRenderable(mLabel);
        delete mLabel;

        auto font = fm->GetFont("data/fonts/Lato-Regular.ttf", 18, Font::NORMAL);
        mLabel = new Text(txt, font);
        mLabel->SetColor(0xf1f2f4ff);
        RegisterRenderable(mLabel);

        // VALUE BAR
        if(!mHasData)
        {
            UnregisterRenderable(mValueBar);
            delete mValueBar;
            mValueBar = new Image;
            RegisterRenderable(mValueBar);
        }

        const unsigned int maxVal = 10;

        if(val > maxVal)
            val = maxVal;

        const unsigned int texId = IND_NE_DIALOG_BAR0 + val;
        tex = tm->GetSprite(SpriteFileNewElementDialog, texId);
        static_cast<Image *>(mValueBar)->SetTexture(tex);

        // update data flag
        mHasData = true;
    }

    void HandlePositionChanged() override
    {
        const int x0 = GetScreenX();
        const int y0 = GetScreenY();

        mBg->SetPosition(x0, y0);

        const int marginH = 10;

        // LABEL
        const int labelX = x0 + marginH;
        const int labelY = y0 + (mBg->GetHeight() - mLabel->GetHeight()) * 0.5f;
        mLabel->SetPosition(labelX, labelY);

        // BAR
        const int barX = x0 + mBg->GetWidth() - marginH - mValueBar->GetWidth();
        const int barY = y0 + (mBg->GetHeight() - mValueBar->GetHeight()) * 0.5f;
        mValueBar->SetPosition(barX, barY);
    }

    void OnRender() override
    {
        mBg->Render();
        mLabel->Render();
        mValueBar->Render();
    }

private:
    lib::graphic::Image * mBg = nullptr;
    lib::graphic::Renderable * mLabel = nullptr;
    lib::graphic::Renderable * mValueBar = nullptr;

    bool mHasData = false;
};

// ===== DIALOG NEW ELEMENT =====
DialogNewElement::DialogNewElement(Player * player)
//    : mPlayer(player)
{
    using namespace lib::sgui;

    auto tm = lib::graphic::TextureManager::Instance();

    // BACKGROUND
    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_BG);
    auto imgBg = new Image(tex, this);

    // CLOSE BUTTON
    mBtnClose = new ButtonClose(this);
    mBtnClose->SetX(imgBg->GetWidth() - mBtnClose->GetWidth());

    // SLOTS
    mSlots = new ButtonsGroup(ButtonsGroup::HORIZONTAL, this);

    for(int i = 0; i < NUM_SLOTS; ++i)
    {
        auto slot = new ButtonSlot(i);

        mSlots->AddButton(slot);
    }

    const int slotsX0 = 52;
    const int slotsY0 = 66;
    mSlots->SetPosition(slotsX0, slotsY0);

    const int slotsMarginBottom = 20;

    // INFO PANEL
    const int panelY0 = slotsY0 + mSlots->GetHeight() + slotsMarginBottom;
    tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_INFO);
    auto panelInfo = new Image(tex, this);
    panelInfo->SetPosition(slotsX0, panelY0);

    // ATTRIBUTE PANELS
    const int panelsX0 = slotsX0 + panelInfo->GetWidth();
    const int panelsY0 = panelY0;
    const int panelBorder = 1;
    int panelsX = panelsX0;
    int panelsY = panelsY0;

    for(int c = 0; c < PANELS_ATT_COLS; ++c)
    {
        const int ind0 = c * PANELS_ATT_ROWS;

        for(int r = 0; r < PANELS_ATT_ROWS; ++r)
        {
            const int ind = ind0 + r;

            auto panAtt = new PanelAttribute(this);
            panAtt->SetPosition(panelsX, panelsY);

            mPanelsAtt[ind] = panAtt;

            panelsY += panAtt->GetHeight() - panelBorder;
        }

        panelsX += mPanelsAtt[0]->GetWidth() - panelBorder;
        panelsY = panelsY0;
    }

    // BUTTON BUILD
    mBtnBuild = new ButtonBuild(this);
    const PanelAttribute * lastPanel = mPanelsAtt[NUM_PANELS_ATT - 1];
    const int btnX = lastPanel->GetX() + lastPanel->GetWidth() - mBtnBuild->GetWidth();
    const int marginBtnTop = 15;
    const int btnY = lastPanel->GetY() + lastPanel->GetHeight() + marginBtnTop;
    mBtnBuild->SetPosition(btnX, btnY);
}

DialogNewElement::~DialogNewElement()
{

}

void DialogNewElement::SetFunctionOnBuild(const std::function<void()> & f)
{
    mBtnBuild->AddOnClickFunction(f);
}

void DialogNewElement::SetFunctionOnClose(const std::function<void()> & f)
{
    mBtnClose->AddOnClickFunction(f);
}

} // namespace game
