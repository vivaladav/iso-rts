#include "Widgets/DialogNewElement.h"

#include "Player.h"
#include "GameObjects/ObjectData.h"
#include "GameObjects/Unit.h"
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

#include <cassert>

namespace game
{

constexpr int NUM_SLOTS = 5;

class ImageButton : public lib::sgui::PushButton
{
public:
    ImageButton(const std::array<unsigned int, NUM_VISUAL_STATES> & texIds,
                const char * spriteFile, lib::sgui::Widget * parent)
        : lib::sgui::PushButton(parent)
        , mBody(new lib::graphic::Image)
    {
        RegisterRenderable(mBody);

        for(unsigned int i = 0; i < NUM_VISUAL_STATES; ++i)
        {
            auto tm = lib::graphic::TextureManager::Instance();
            mTexs[i] = tm->GetSprite(spriteFile, texIds[i]);
        }

        // set initial visual state
        SetState(NORMAL);
    }
private:
    void OnStateChanged(lib::sgui::PushButton::VisualState state) override
    {
        mBody->SetTexture(mTexs[state]);
        // reset BG to make changes visible
        SetCurrBg(mBody);
    }

private:
    std::array<lib::graphic::Texture *, NUM_VISUAL_STATES> mTexs;

    lib::graphic::Image * mBody = nullptr;
};

// ===== BUTTON LEFT =====
class ButtonLeft : public ImageButton
{
public:
    ButtonLeft(lib::sgui::Widget * parent)
        : ImageButton({ IND_NE_DIALOG_LEFT_NORMAL,
                        IND_NE_DIALOG_LEFT_DISABLED,
                        IND_NE_DIALOG_LEFT_OVER,
                        IND_NE_DIALOG_LEFT_PUSHED,
                        IND_NE_DIALOG_LEFT_NORMAL },
                        SpriteFileNewElementDialog, parent)
    {
    }
};

// ===== BUTTON RIGHT =====
class ButtonRight : public ImageButton
{
public:
    ButtonRight(lib::sgui::Widget * parent)
        : ImageButton({ IND_NE_DIALOG_RIGHT_NORMAL,
                        IND_NE_DIALOG_RIGHT_DISABLED,
                        IND_NE_DIALOG_RIGHT_OVER,
                        IND_NE_DIALOG_RIGHT_PUSHED,
                        IND_NE_DIALOG_RIGHT_NORMAL },
                        SpriteFileNewElementDialog, parent)
    {
    }
};

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

private:
    void OnStateChanged(lib::sgui::PushButton::VisualState state) override
    {
        // BACKGROUND
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

        // LABEL
        const unsigned int colorLabel[NUM_VISUAL_STATES] =
        {
            0xe3e6e8ff,
            0x454f54ff,
            0xf1f2f4ff,
            0xabb4baff,
            0xc2c2a3ff
        };

        SetLabelColor(colorLabel[state]);

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

        // image
        mImage = new DummyRenderable;

        // shortcut
        auto fm = FontManager::Instance();
        auto font = fm->GetFont("data/fonts/Lato-Bold.ttf", 11, Font::NORMAL);
        mShortcut = new Text(SHORTCUTS[index], font, true);
        mShortcut->SetColor(0xd5daddff);

        // register graphic elements
        RegisterRenderable(mBody);
        RegisterRenderable(mImage);
        RegisterRenderable(mShortcut);

        // set initial visual state
        SetState(NORMAL);
    }

    void ClearImage()
    {
        if(!mHasImg)
            return;

        UnregisterRenderable(mImage);
        delete mImage;

        mImage = new lib::graphic::DummyRenderable;
        RegisterRenderable(mImage);

        mHasImg = false;
    }

    void SetImage(lib::graphic::Texture * tex)
    {
        if(!mHasImg)
        {
            UnregisterRenderable(mImage);
            delete mImage;

            mImage = new lib::graphic::Image;
            RegisterRenderable(mImage);
        }

        static_cast<lib::graphic::Image *>(mImage)->SetTexture(tex);

        mHasImg = true;

        // reset positions
        HandlePositionChanged();
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

        // image - same alpha as label
        mImage->SetAlpha(alphaLabel);
    }

    void HandlePositionChanged() override
    {
        PushButton::HandlePositionChanged();

        // IMAGE
        const int imgX = GetScreenX() + (GetWidth() - mImage->GetWidth()) * 0.5f;
        const int imgY = GetScreenY() + (GetHeight() - mImage->GetHeight()) * 0.5f;
        mImage->SetPosition(imgX, imgY);

        // SHORTCUT
        const int shortBgX0 = 182;
        const int shortBgY0 = 182;
        const int shortBgSize = 14;

        const int shortcutX = GetScreenX() + shortBgX0 + (shortBgSize - mShortcut->GetWidth()) * 0.5f;
        const int shortcutY = GetScreenY() + shortBgY0 + (shortBgSize - mShortcut->GetHeight()) * 0.5f;

        mShortcut->SetPosition(shortcutX, shortcutY);
    }

private:
    static const int KEYS[NUM_SLOTS];
    static const char * SHORTCUTS[NUM_SLOTS];

    lib::graphic::Image * mBody = nullptr;
    lib::graphic::Renderable * mImage = nullptr;
    lib::graphic::Text * mShortcut = nullptr;

    bool mHasImg = false;
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

        // reset positions
        HandlePositionChanged();
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
DialogNewElement::DialogNewElement(const std::vector<ObjectData> & data, const char * title)
    : mData(data)
{
    using namespace lib::sgui;

    assert(!data.empty());

    auto fm = lib::graphic::FontManager::Instance();
    auto tm = lib::graphic::TextureManager::Instance();

    const int marginL = 52;
    const int marginT = 6;

    // BACKGROUND
    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_BG);
    auto imgBg = new Image(tex, this);

    // CLOSE BUTTON
    mBtnClose = new ButtonClose(this);
    mBtnClose->SetX(imgBg->GetWidth() - mBtnClose->GetWidth());

    // TITLE
    auto font = fm->GetFont("data/fonts/Lato-Regular.ttf", 28, lib::graphic::Font::NORMAL);
    mTitle = new Label(title, font, this);
    mTitle->SetColor(0xf1f2f4ff);
    mTitle->SetPosition(marginL, marginT);

    // SLOTS
    mSlots = new ButtonsGroup(ButtonsGroup::HORIZONTAL, this);

    for(int i = 0; i < NUM_SLOTS; ++i)
    {
        auto slot = new ButtonSlot(i);

        slot->AddOnToggleFunction([this, i](bool checked)
        {
            if(checked)
                ShowData(mFirstElem + i);
        });

        mSlots->AddButton(slot);
    }

    const int slotsY0 = 66;
    mSlots->SetPosition(marginL, slotsY0);

    const int marginButtonsLR = 10;

    const int numData = data.size();

    mBtnLeft = new ButtonLeft(this);
    const int posLX = mSlots->GetX() - mBtnLeft->GetWidth() - marginButtonsLR;
    const int posLY = mSlots->GetY() + (mSlots->GetHeight() - mBtnLeft->GetHeight()) * 0.5f;
    mBtnLeft->SetPosition(posLX, posLY);
    mBtnLeft->SetEnabled(false);

    mBtnLeft->AddOnClickFunction([this]
    {
       mFirstElem -= NUM_SLOTS;
       UpdateSlots();

       mBtnLeft->SetEnabled(mFirstElem > 0);
       mBtnRight->SetEnabled(true);
    });

    mBtnRight = new ButtonRight(this);
    const int posRX = mSlots->GetX() + mSlots->GetWidth() + marginButtonsLR;
    const int posRY = mSlots->GetY() + (mSlots->GetHeight() - mBtnRight->GetHeight()) * 0.5f;
    mBtnRight->SetPosition(posRX, posRY);
    mBtnRight->SetEnabled(numData > NUM_SLOTS);

    mBtnRight->AddOnClickFunction([this, numData]
    {
       mFirstElem += NUM_SLOTS;
       UpdateSlots();

       mBtnLeft->SetEnabled(true);
       mBtnRight->SetEnabled(mFirstElem + NUM_SLOTS < numData);
    });

    // INFO PANEL
    const int slotsMarginBottom = 20;

    const int panelY0 = slotsY0 + mSlots->GetHeight() + slotsMarginBottom;
    tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_INFO);
    auto panelInfo = new Image(tex, this);
    panelInfo->SetPosition(marginL, panelY0);

    const unsigned int colorHeader = 0xf1f2f4ff;
    const unsigned int colorText = 0xa9afbcff;

    auto fontHeader = fm->GetFont("data/fonts/Lato-Bold.ttf", 18, lib::graphic::Font::NORMAL);
    auto fontText = fm->GetFont("data/fonts/Lato-Regular.ttf", 17, lib::graphic::Font::NORMAL);

    const int marginPanelXY0 = 15;
    const int marginPanelBlock = 35;
    const int marginPanelIconH = 10;
    const int marginPanelIconV = 5;
    const int marginPanelIconBlock = 85;
    const int marginPanelDataV = 10;

    // header category
    auto headerCat = new Label("CATEGORY", fontHeader, panelInfo);
    headerCat->SetColor(colorHeader);
    headerCat->SetPosition(marginPanelXY0, marginPanelXY0);

    int contY = headerCat->GetY() + headerCat->GetHeight() + marginPanelIconV;

    // text category
    mLabelCategory = new Label("-", fontText, panelInfo);
    mLabelCategory->SetColor(colorText);
    mLabelCategory->SetPosition(marginPanelXY0, contY);

    contY += mLabelCategory->GetHeight() + marginPanelBlock;

    // header cost
    headerCat = new Label("COST", fontHeader, panelInfo);
    headerCat->SetColor(colorHeader);
    headerCat->SetPosition(marginPanelXY0, contY);

    contY = headerCat->GetY() + headerCat->GetHeight() + marginPanelIconV;

    // data cost 1
    tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_ICON_ENERGY);
    auto imgIcon = new Image(tex, panelInfo);
    imgIcon->SetPosition(marginPanelXY0, contY);

    int contX = imgIcon->GetX() + imgIcon->GetWidth() + marginPanelIconH;

    mLabelsCost[0] = new Label("-", fontText, panelInfo);
    mLabelsCost[0]->SetColor(colorText);
    contY = imgIcon->GetY() + (imgIcon->GetHeight() - mLabelsCost[0]->GetHeight()) * 0.5f;
    mLabelsCost[0]->SetPosition(contX, contY);

    contX = imgIcon->GetX() + imgIcon->GetWidth() + marginPanelIconBlock;
    contY = imgIcon->GetY();

    // data cost 2
    tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_ICON_MATERIAL);
    imgIcon = new Image(tex, panelInfo);
    imgIcon->SetPosition(contX, contY);

    contX = imgIcon->GetX() + imgIcon->GetWidth() + marginPanelIconH;

    mLabelsCost[1] = new Label("-", fontText, panelInfo);
    mLabelsCost[1]->SetColor(colorText);
    contY = imgIcon->GetY() + (imgIcon->GetHeight() - mLabelsCost[1]->GetHeight()) * 0.5f;
    mLabelsCost[1]->SetPosition(contX, contY);

    contY = imgIcon->GetY() + imgIcon->GetHeight() + marginPanelDataV;

    // data cost 3
    tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_ICON_DIAMOND);
    imgIcon = new Image(tex, panelInfo);
    imgIcon->SetPosition(marginPanelXY0, contY);

    contX = imgIcon->GetX() + imgIcon->GetWidth() + marginPanelIconH;

    mLabelsCost[2] = new Label("-", fontText, panelInfo);
    mLabelsCost[2]->SetColor(colorText);
    contY = imgIcon->GetY() + (imgIcon->GetHeight() - mLabelsCost[2]->GetHeight()) * 0.5f;
    mLabelsCost[2]->SetPosition(contX, contY);

    contX = imgIcon->GetX() + imgIcon->GetWidth() + marginPanelIconBlock;
    contY = imgIcon->GetY();

    // data cost 4
    tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_ICON_BLOB);
    imgIcon = new Image(tex, panelInfo);
    imgIcon->SetPosition(contX, contY);

    contX = imgIcon->GetX() + imgIcon->GetWidth() + marginPanelIconH;

    mLabelsCost[3] = new Label("-", fontText, panelInfo);
    mLabelsCost[3]->SetColor(colorText);
    contY = imgIcon->GetY() + (imgIcon->GetHeight() - mLabelsCost[3]->GetHeight()) * 0.5f;
    mLabelsCost[3]->SetPosition(contX, contY);

    // ATTRIBUTE PANELS
    const int panelsX0 = marginL + panelInfo->GetWidth();
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

    // finally show data
    UpdateSlots();
}

void DialogNewElement::SetFunctionOnBuild(const std::function<void()> & f)
{
    mBtnBuild->AddOnClickFunction(f);
}

void DialogNewElement::SetFunctionOnClose(const std::function<void()> & f)
{
    mBtnClose->AddOnClickFunction(f);
}

int DialogNewElement::GetSelectedIndex() const
{
    return mFirstElem + mSlots->GetIndexChecked();
}

void DialogNewElement::UpdateSlots()
{
    const int numData = mData.size();
    const int leftData = numData - mFirstElem;
    const int limitData = leftData < NUM_SLOTS ? leftData : NUM_SLOTS;

    auto tm = lib::graphic::TextureManager::Instance();

    for(int i = 0; i < limitData; ++i)
    {
        ButtonSlot * slot = static_cast<ButtonSlot *>(mSlots->GetButton(i));
        slot->SetEnabled(true);

        const int indData = mFirstElem + i;
        const ObjectData & data = mData[indData];

        auto tex = tm->GetSprite(data.iconFile, data.iconTexId);
        slot->SetImage(tex);

        // check first
        slot->SetChecked(false);
    }

    mSlots->GetButton(0)->SetChecked(true);

    for(int i = limitData; i < NUM_SLOTS; ++i)
    {
        ButtonSlot * slot = static_cast<ButtonSlot *>(mSlots->GetButton(i));
        slot->ClearImage();
        slot->SetEnabled(false);
    }
}

void DialogNewElement::ShowData(int ind)
{
    assert(ind < mData.size());

    const ObjectData & data = mData[ind];

    // CLASS
    mLabelCategory->SetText(ObjectData::STR_CLASS[data.objClass]);

    // COSTS
    for(int i = 0; i < NUM_COSTS; ++i)
        mLabelsCost[i]->SetText(std::to_string(data.costs[i]).c_str());

    // STATS
    const int numStats = data.stats.size();

    for(int i = 0; i < numStats; ++i)
        mPanelsAtt[i]->SetData(ObjectData::STR_STAT[i], data.stats[i]);

    for(int i = numStats; i < NUM_PANELS_ATT; ++i)
        mPanelsAtt[i]->ClearData();
}

} // namespace game
