#include "Widgets/DialogNewElement.h"

#include "GameConstants.h"
#include "Player.h"
#include "GameObjects/ObjectData.h"
#include "GameObjects/Unit.h"
#include "Widgets/GameButton.h"
#include "Widgets/GameUIData.h"
#include "Widgets/ShortcutButton.h"

#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/graphic/DummyRenderable.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/ButtonsGroup.h>
#include <sgl/sgui/ImageButton.h>
#include <sgl/sgui/Image.h>
#include <sgl/sgui/Label.h>
#include <sgl/sgui/TextArea.h>

#include <cassert>

namespace game
{

constexpr int NUM_SLOTS = 6;

// ===== BUTTON LEFT =====
class ButtonLeft : public sgl::sgui::ImageButton
{
public:
    ButtonLeft(sgl::sgui::Widget * parent)
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
class ButtonRight : public sgl::sgui::ImageButton
{
public:
    ButtonRight(sgl::sgui::Widget * parent)
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
    ButtonClose(sgl::sgui::Widget * parent)
        : ShortcutButton(sgl::core::KeyboardEvent::KEY_ESC, parent)
        , mBody(new sgl::graphic::Image)
    {
        // register graphic elements
        RegisterRenderable(mBody);

        // set initial visual state
        SetState(NORMAL);
    }

private:
    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override
    {
        const unsigned int texIds[NUM_VISUAL_STATES] =
        {
            IND_NE_DIALOG_CLOSE_NORMAL,
            IND_NE_DIALOG_CLOSE_NORMAL,     // button can't be disabled
            IND_NE_DIALOG_CLOSE_OVER,
            IND_NE_DIALOG_CLOSE_PUSHED,
            IND_NE_DIALOG_CLOSE_NORMAL,     // button can't be checked
        };

        auto tm = sgl::graphic::TextureManager::Instance();
        sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileNewElementDialog, texIds[state]);
        mBody->SetTexture(tex);

        SetSize(mBody->GetWidth(), mBody->GetHeight());
    }

    void HandlePositionChanged() override
    {
        mBody->SetPosition(GetScreenX(), GetScreenY());
    }

private:
    sgl::graphic::Image * mBody = nullptr;
};

// ===== BUTTON BUILD =====

class ButtonBuild : public ShortcutButton
{
public:
    ButtonBuild(sgl::sgui::Widget * parent)
        : ShortcutButton(sgl::core::KeyboardEvent::KEY_B, parent)
        , mBody(new sgl::graphic::Image)
    {
        using namespace sgl::graphic;

        auto fm = FontManager::Instance();
        auto font = fm->GetFont("data/fonts/Lato-Bold.ttf", 11, Font::NORMAL);
        mShortcut = new Text("B", font, true);
        mShortcut->SetColor(0xd5daddff);

        // register graphic elements
        RegisterRenderable(mBody);
        RegisterRenderable(mShortcut);

        // LABEL
        font = fm->GetFont("data/fonts/Lato-Regular.ttf", 19, Font::NORMAL);
        mLabel = new Text("BUILD", font);
        RegisterRenderable(mLabel);

        // set initial visual state
        SetState(NORMAL);
    }

private:
    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override
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

        auto tm = sgl::graphic::TextureManager::Instance();
        sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileNewElementDialog, texIds[state]);
        mBody->SetTexture(tex);

        SetSize(mBody->GetWidth(), mBody->GetHeight());

        // LABEL
        const unsigned int colorLabel[NUM_VISUAL_STATES] =
        {
            0xe3e6e8ff,
            0x454f54ff,
            0xf1f2f4ff,
            0xabb4baff,
            0xc2c2a3ff
        };

        mLabel->SetColor(colorLabel[state]);

        // update shortcut label alpha
        const unsigned char alphaEn = 255;
        const unsigned char alphaDis = 128;
        const unsigned char alphaLabel = DISABLED == state ? alphaDis : alphaEn;
        mShortcut->SetAlpha(alphaLabel);
    }

    void HandlePositionChanged() override
    {
        ShortcutButton::HandlePositionChanged();

        const int x0 = GetScreenX();
        const int y0 = GetScreenY();

        // position BG
        mBody->SetPosition(x0, y0);

        // SHORTCUT
        const int shortBgX0 = 182;
        const int shortBgY0 = 22;
        const int shortBgSize = 14;

        const int shortcutX = x0 + shortBgX0 + (shortBgSize - mShortcut->GetWidth()) * 0.5f;
        const int shortcutY = y0 + shortBgY0 + (shortBgSize - mShortcut->GetHeight()) * 0.5f;

        mShortcut->SetPosition(shortcutX, shortcutY);

        // LABEL
        const int labelX = x0 + (GetWidth() - mLabel->GetWidth()) * 0.5f;
        const int labelY = y0 + (GetHeight() - mLabel->GetHeight()) * 0.5f;

        mLabel->SetPosition(labelX, labelY);
    }

private:
    sgl::graphic::Image * mBody = nullptr;
    sgl::graphic::Text * mLabel = nullptr;
    sgl::graphic::Text * mShortcut = nullptr;
};

// ===== BUTTON SLOT =====

class ButtonSlot : public ShortcutButton
{
public:
    ButtonSlot(int index)
        : ShortcutButton(KEYS[index], nullptr)
        , mBody(new sgl::graphic::Image)
    {
        using namespace sgl::graphic;

        SetCheckable(true);

        auto fm = FontManager::Instance();

        // TITLE
        auto tm = sgl::graphic::TextureManager::Instance();
        sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_PANEL_NORMAL);
        auto font = fm->GetFont("data/fonts/Lato-Regular.ttf", 16, Font::NORMAL);
        mTitle = new sgl::sgui::TextArea(tex->GetWidth(), TITLE_H, font, this);
        mTitle->setTextAlignment(sgl::sgui::TextArea::ALIGN_H_CENTER, sgl::sgui::TextArea::ALIGN_V_CENTER);

        // IMAGE
        mImage = new DummyRenderable;

        // SHORTCUT
        font = fm->GetFont("data/fonts/Lato-Bold.ttf", 12, Font::NORMAL);
        mShortcut = new Text(SHORTCUTS[index], font);
        mShortcut->SetColor(0xd5daddff);

        // register graphic elements
        RegisterRenderable(mBody);
        RegisterRenderable(mImage);
        RegisterRenderable(mShortcut);

        // set initial visual state
        SetState(NORMAL);
    }

    void ClearData()
    {
        // already no data -> exit
        if(!mHasData)
            return;

        // title
        mTitle->ClearText();

        // image
        UnregisterRenderable(mImage);
        delete mImage;

        mImage = new sgl::graphic::DummyRenderable;
        RegisterRenderable(mImage);

        // clear flag
        mHasData = false;
    }

    void SetData(const char * title, sgl::graphic::Texture * texImg)
    {
        using namespace sgl::graphic;

        // title
        mTitle->SetText(title);

        // image
        if(!mHasData)
        {
            UnregisterRenderable(mImage);
            delete mImage;

            mImage = new Image;
            RegisterRenderable(mImage);
        }

        static_cast<Image *>(mImage)->SetTexture(texImg);

        mHasData = true;

        // reset positions
        HandlePositionChanged();

        OnStateChanged(GetState());
    }

private:
    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override
    {
        const unsigned int texIds[NUM_VISUAL_STATES] =
        {
            IND_NE_DIALOG_PANEL_NORMAL,
            IND_NE_DIALOG_PANEL_DISABLED,
            IND_NE_DIALOG_PANEL_OVER,
            IND_NE_DIALOG_PANEL_SEL,
            IND_NE_DIALOG_PANEL_SEL,
        };

        auto tm = sgl::graphic::TextureManager::Instance();
        sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileNewElementDialog, texIds[state]);
        mBody->SetTexture(tex);

        SetSize(mBody->GetWidth(), mBody->GetHeight());

        // set title color
        const unsigned int colorTitle[NUM_VISUAL_STATES] =
        {
            0xc6cad2ff,
            0x687183ff,
            0xc9cdd4ff,
            0xc6c99cff,
            0xc6c99cff
        };

        mTitle->SetColor(colorTitle[state]);

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
        ShortcutButton::HandlePositionChanged();

        const int x0 = GetScreenX();
        const int y0 = GetScreenY();

        // position BG
        mBody->SetPosition(x0, y0);

        // IMAGE
        const int imageBlockH = GetHeight() - TITLE_H;
        const int imgX = x0 + (GetWidth() - mImage->GetWidth()) * 0.5f;
        const int imgY = y0 + TITLE_H + (imageBlockH - mImage->GetHeight()) * 0.5f;
        mImage->SetPosition(imgX, imgY);

        // SHORTCUT
        const int shortBgX0 = 182;
        const int shortBgY0 = 182;
        const int shortBgSize = 14;

        const int shortcutX = x0 + shortBgX0 + (shortBgSize - mShortcut->GetWidth()) * 0.5f;
        const int shortcutY = y0 + shortBgY0 + (shortBgSize - mShortcut->GetHeight()) * 0.5f;

        mShortcut->SetPosition(shortcutX, shortcutY);
    }

private:
    static const int KEYS[NUM_SLOTS];
    static const char * SHORTCUTS[NUM_SLOTS];
    static const int TITLE_H = 50;

private:
    sgl::graphic::Image * mBody = nullptr;
    sgl::sgui::TextArea * mTitle = nullptr;
    sgl::graphic::Renderable * mImage = nullptr;
    sgl::graphic::Text * mShortcut = nullptr;

    bool mHasData = false;
};

const int ButtonSlot::KEYS[NUM_SLOTS] = {
                                            sgl::core::KeyboardEvent::KEY_1,
                                            sgl::core::KeyboardEvent::KEY_2,
                                            sgl::core::KeyboardEvent::KEY_3,
                                            sgl::core::KeyboardEvent::KEY_4,
                                            sgl::core::KeyboardEvent::KEY_5,
                                            sgl::core::KeyboardEvent::KEY_6
                                        };


const char * ButtonSlot::SHORTCUTS[NUM_SLOTS] = { "1", "2", "3", "4", "5", "6" };

// ===== ATTRIBUTE PANEL =====

class PanelAttribute : public sgl::sgui::Widget
{
public:
    PanelAttribute(sgl::sgui::Widget * parent)
        : sgl::sgui::Widget(parent)
    {
        using namespace sgl::graphic;

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
        using namespace sgl::graphic;

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
        using namespace sgl::graphic;

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
    sgl::graphic::Image * mBg = nullptr;
    sgl::graphic::Renderable * mLabel = nullptr;
    sgl::graphic::Renderable * mValueBar = nullptr;

    bool mHasData = false;
};

// ===== DIALOG NEW ELEMENT =====
DialogNewElement::DialogNewElement(const std::vector<ObjectData> & data, const char * title, Player *player)
    : mData(data)
    , mPlayer(player)
{
    using namespace sgl::sgui;

    assert(!data.empty());

    auto fm = sgl::graphic::FontManager::Instance();
    auto tm = sgl::graphic::TextureManager::Instance();

    const int marginL = 40;
    const int marginT = 8;

    // BACKGROUND
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_BG);
    auto imgBg = new Image(tex, this);

    // CLOSE BUTTON
    mBtnClose = new ButtonClose(this);
    mBtnClose->SetX(imgBg->GetWidth() - mBtnClose->GetWidth());

    // TITLE
    auto font = fm->GetFont("data/fonts/Lato-Regular.ttf", 28, sgl::graphic::Font::NORMAL);
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
            {
                const int ind = mFirstElem + i;
                ShowData(ind);
                CheckBuild(ind);
            }
        });

        mSlots->AddButton(slot);
    }

    const int slotsY0 = 65;
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
    const int slotsMarginBottom = 15;

    const int panelY0 = slotsY0 + mSlots->GetHeight() + slotsMarginBottom;
    tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_INFO);
    auto panelInfo = new Image(tex, this);
    panelInfo->SetPosition(marginL, panelY0);

    const unsigned int colorHeader = 0xf1f2f4ff;
    const unsigned int colorText = 0xb6c0c9ff;

    auto fontHeader = fm->GetFont("data/fonts/Lato-Bold.ttf", 18, sgl::graphic::Font::NORMAL);
    auto fontText = fm->GetFont("data/fonts/Lato-Regular.ttf", 17, sgl::graphic::Font::NORMAL);

    const int marginPanelXY0 = 10;
    const int marginPanelBlock = 20;
    const int marginPanelIconH = 5;
    const int marginPanelIconV = 5;
    const int marginPanelIconBlock = 85;
    const int marginPanelDataV = 5;

    // header DESCRIPTION
    auto headerDesc = new Label("DESCRIPTION", fontHeader, panelInfo);
    headerDesc->SetColor(colorHeader);
    headerDesc->SetPosition(marginPanelXY0, marginPanelXY0);

    int contY = headerDesc->GetY() + headerDesc->GetHeight() + marginPanelDataV;

    // description text area
    const int areaW = panelInfo->GetWidth() - (2 * marginPanelXY0);
    const int areaH = 85;
    mDescription = new TextArea(areaW, areaH, fontText, panelInfo);
    mDescription->SetColor(colorText);
    mDescription->SetPosition(marginPanelXY0, contY);

    contY += mDescription->GetHeight() + marginPanelBlock;

    // header category
    auto headerCat = new Label("CATEGORY", fontHeader, panelInfo);
    headerCat->SetColor(colorHeader);
    headerCat->SetPosition(marginPanelXY0, contY);

    contY = headerCat->GetY() + headerCat->GetHeight() + marginPanelDataV;

    // text category
    mCategory = new Label("-", fontText, panelInfo);
    mCategory->SetColor(colorText);
    mCategory->SetPosition(marginPanelXY0, contY);

    contY += mCategory->GetHeight() + marginPanelBlock;

    // header cost
    headerCat = new Label("COST", fontHeader, panelInfo);
    headerCat->SetColor(colorHeader);
    headerCat->SetPosition(marginPanelXY0, contY);

    contY = headerCat->GetY() + headerCat->GetHeight() + marginPanelDataV;

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

    contX = imgIcon->GetX() + imgIcon->GetWidth() + marginPanelIconBlock;
    contY = imgIcon->GetY();

    // data cost 3
    tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_ICON_DIAMOND);
    imgIcon = new Image(tex, panelInfo);
    imgIcon->SetPosition(contX, contY);

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

void DialogNewElement::CheckBuild()
{
    const int ind = GetSelectedIndex();
    CheckBuild(ind);
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

const ObjectData & DialogNewElement::GetSelectedData() const
{
    return mData[GetSelectedIndex()];
}

void DialogNewElement::UpdateSlots()
{
    const int numData = mData.size();
    const int leftData = numData - mFirstElem;
    const int limitData = leftData < NUM_SLOTS ? leftData : NUM_SLOTS;

    auto tm = sgl::graphic::TextureManager::Instance();

    for(int i = 0; i < limitData; ++i)
    {
        ButtonSlot * slot = static_cast<ButtonSlot *>(mSlots->GetButton(i));
        slot->SetEnabled(true);

        const int indData = mFirstElem + i;
        const ObjectData & data = mData[indData];

        auto tex = tm->GetSprite(data.iconFile, data.iconTexId);
        slot->SetData(data.title, tex);

        // check first
        slot->SetChecked(false);
    }

    mSlots->GetButton(0)->SetChecked(true);

    for(int i = limitData; i < NUM_SLOTS; ++i)
    {
        ButtonSlot * slot = static_cast<ButtonSlot *>(mSlots->GetButton(i));
        slot->ClearData();
        slot->SetEnabled(false);
    }
}

void DialogNewElement::ShowData(int ind)
{
    assert(ind < mData.size());

    const ObjectData & data = mData[ind];

    // DESCRIPTION
    mDescription->SetText(data.description);

    // CLASS
    mCategory->SetText(ObjectData::STR_CLASS[data.objClass]);

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

void DialogNewElement::CheckBuild(int ind)
{
    const std::vector<int> & costs = mData[ind].costs;

    const bool SPEND[NUM_COSTS] =
    {
        mPlayer->GetStat(Player::ENERGY).GetIntValue() >= costs[RES_ENERGY],
        mPlayer->GetStat(Player::MATERIAL).GetIntValue() >= costs[RES_MATERIAL1],
        mPlayer->GetStat(Player::DIAMONDS).GetIntValue() >= costs[RES_DIAMONDS],
        mPlayer->GetStat(Player::BLOBS).GetIntValue() >= costs[RES_BLOBS]
    };

    const bool allowed =  SPEND[0] && SPEND[1] && SPEND[2] && SPEND[3];

    // enable BUILD button
    mBtnBuild->SetEnabled(allowed);

    // set color of costs
    for(unsigned int i = 0; i < NUM_COSTS; ++i)
    {
        const unsigned int COLORS[] = { 0xe08585ff, 0x5cd666ff};
        const int ind = static_cast<int>(SPEND[i]);

        mLabelsCost[i]->SetColor(COLORS[ind]);
    }
}

} // namespace game
