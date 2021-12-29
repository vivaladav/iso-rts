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
    mButtonClose = new ButtonClose(this);
    mButtonClose->SetX(imgBg->GetWidth() - mButtonClose->GetWidth());

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
}

DialogNewElement::~DialogNewElement()
{

}

void DialogNewElement::SetFunctionOnBuild(const std::function<void()> & f)
{

}

void DialogNewElement::SetFunctionOnClose(const std::function<void()> & f)
{
    mButtonClose->AddOnClickFunction(f);
}

} // namespace game
