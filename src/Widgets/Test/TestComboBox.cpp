#include "Widgets/Test/TestComboBox.h"

#include "GameTestData.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/TextureManager.h>

namespace game
{

// ==== COMBO BOX ITEM ====
TestComboBoxItem::TestComboBoxItem(const char * txt)
    : sgl::sgui::ComboBoxItem(txt)
{
    using namespace sgl::graphic;

    mBody = new Image;
    RegisterRenderable(mBody);

    // TEXT LABEL
    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("Lato-Regular.ttf", 22, Font::NORMAL);
    mText = new Text(txt, font, true);
    RegisterRenderable(mText);

    // init to normal state
    SetState(NORMAL);

    PositionElements();
}

void TestComboBoxItem::HandlePositionChanged()
{
    sgl::sgui::ComboBoxItem::HandlePositionChanged();

    PositionElements();
}

void TestComboBoxItem::OnStateChanged(sgl::sgui::AbstractButton::VisualState state)
{
    // BODY
    const unsigned int texIds[NUM_VISUAL_STATES] =
    {
        IND_TCBI_NORMAL,
        IND_TCBI_DISABLED,
        IND_TCBI_OVER,
        IND_TCBI_PUSHED,
        IND_TCBI_CHECKED
    };

    auto tm = sgl::graphic::TextureManager::Instance();
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileTestUI, texIds[state]);
    mBody->SetTexture(tex);

    SetSize(mBody->GetWidth(), mBody->GetHeight());

    // TEXT
    const unsigned int txtColors[NUM_VISUAL_STATES] =
    {
        0xe3e6e8ff,
        0x5c6970ff,
        0xe3e6e8ff,
        0xd5daddff,
        0xe3e6e8ff
    };

   mText->SetColor(txtColors[state]);
}

void TestComboBoxItem::PositionElements()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();
    const int w = GetWidth();
    const int h = GetHeight();

    // position BG
    mBody->SetPosition(x0, y0);

    // text
    const int textX = x0 + (w - mText->GetWidth()) * 0.5f;
    const int textY = y0 + (h - mText->GetHeight()) * 0.5f;
    mText->SetPosition(textX, textY);
}

// ==== COMBO BOX ====
TestComboBox::TestComboBox(sgl::sgui::Widget * parent)
    : sgl::sgui::ComboBox(parent)
{
    using namespace sgl::graphic;

    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("Lato-Regular.ttf", 22, Font::NORMAL);
    SetLabelFont(font);
}

void TestComboBox::OnStateChanged(sgl::sgui::AbstractButton::VisualState state)
{
    // BODY
    const unsigned int texIds[NUM_VISUAL_STATES] =
    {
        IND_TCB_NORMAL,
        IND_TCB_DISABLED,
        IND_TCB_OVER,
        IND_TCB_PUSHED,
        IND_TCB_CHECKED
    };

    auto tm = sgl::graphic::TextureManager::Instance();
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileTestUI, texIds[state]);
    SetBodyTexture(tex);

    // TEXT
    const unsigned int txtColors[NUM_VISUAL_STATES] =
    {
        0xe3e6e8ff,
        0x5c6970ff,
        0xe3e6e8ff,
        0xd5daddff,
        0xe3e6e8ff
    };

   SetLabelColor(txtColors[state]);
}

} // namespace game
