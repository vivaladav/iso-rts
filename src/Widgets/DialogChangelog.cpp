#include "DialogChangelog.h"

#include "GameUIData.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/ScrollArea.h>
#include <sgl/sgui/TextArea.h>

namespace game
{

// ===== BUTTON CHANGELOG =====
ButtonChangelog::ButtonChangelog()
{
    using namespace sgl;

    // BACKGROUND
    auto tm = graphic::TextureManager::Instance();
    auto tex = tm->GetSprite(SpriteFileMainMenu, IND_MM_BTN_UPDATES);
    mBg = new graphic::Image(tex);
    RegisterRenderable(mBg);

    SetSize(mBg->GetWidth(), mBg->GetHeight());

    // TEXT
    auto fm = graphic::FontManager::Instance();
    auto font = fm->GetFont("Lato-Regular.ttf", 20, graphic::Font::NORMAL);
    mLabel = new sgui::TextArea(GetWidth(), GetHeight(), font, this);
    mLabel->setTextAlignment(sgui::TextArea::ALIGN_H_CENTER, sgui::TextArea::ALIGN_V_CENTER);
    mLabel->SetText("U\nP\nD\nA\nT\nE\nS");

    UpdateLabel(sgui::AbstractButton::VisualState::NORMAL);
}

void ButtonChangelog::HandlePositionChanged()
{
    PositionElements();
}

void ButtonChangelog::OnStateChanged(sgl::sgui::AbstractButton::VisualState state)
{
    UpdateLabel(state);
}

void ButtonChangelog::PositionElements()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    mBg->SetPosition(x0, y0);
}

void ButtonChangelog::UpdateLabel(sgl::sgui::AbstractButton::VisualState state)
{
    const unsigned int colorsLabel[NUM_VISUAL_STATES] =
    {
        0xb9ced9ff,
        0x35464dff,
        0xcfe6f2ff,
        0xaec2ccff,
        0xaec2ccff
    };

    mLabel->SetColor(colorsLabel[state]);
}

// ===== BUTTON CLOSE CHANGELOG =====
class ButtonCloseChangelog : public sgl::sgui::AbstractButton
{
public:
    ButtonCloseChangelog(sgl::sgui::Widget * parent)
        : sgl::sgui::AbstractButton(parent)
    {
        using namespace sgl;

        // BACKGROUND
        auto tm = graphic::TextureManager::Instance();
        auto tex = tm->GetSprite(SpriteFileMainMenu, IND_MM_BTN_CLOSE);
        mBg = new graphic::Image(tex);
        RegisterRenderable(mBg);

        SetSize(mBg->GetWidth(), mBg->GetHeight());
    }

private:
    void HandlePositionChanged() override
    {
        PositionElements();
    }

    void PositionElements()
    {
        const int x0 = GetScreenX();
        const int y0 = GetScreenY();

        mBg->SetPosition(x0, y0);
    }

private:
    sgl::graphic::Image * mBg = nullptr;
};

// ===== CHANGELOG AREA =====
class ChangelogArea : public sgl::sgui::ScrollArea
{
public:
    ChangelogArea(sgl::sgui::Widget * parent)
        : sgl::sgui::ScrollArea(460, 360, parent)
    {

    }

private:
    void HandleNewContent()
    {
        const int contX = 20;
        const int contY = 20;
        const int contW = 380;
        const int contH = 320;

        sgl::sgui::Widget * cont = GetContent();
        cont->SetPosition(contX, contY);
        cont->SetVisibleArea(0, 0, contW, contH);
    }
};

// ===== DIALOG CHANGELOG =====
DialogChangelog::DialogChangelog()
{
    using namespace sgl;

    // BACKGROUND
    auto tm = graphic::TextureManager::Instance();
    auto tex = tm->GetSprite(SpriteFileMainMenu, IND_MM_DIALOG_UPDATES);
    mBg = new graphic::Image(tex);
    RegisterRenderable(mBg);

    SetSize(mBg->GetWidth(), mBg->GetHeight());

    // TITLE
    auto fm = graphic::FontManager::Instance();
    auto font = fm->GetFont("Lato-Regular.ttf", 28, graphic::Font::NORMAL);
    mTitle = new graphic::Text("NEWS & UPDATES", font);
    mTitle->SetColor(0xe3e5e5ff);
    RegisterRenderable(mTitle);

    // BUTTON CLOSE
    mButtonClose = new ButtonCloseChangelog(this);
    mButtonClose->SetX(GetWidth() - mButtonClose->GetWidth());

    // CONTENT AREA
    mContArea = new ChangelogArea(this);
}

void DialogChangelog::SetContent(sgl::sgui::Widget * cont)
{
    mContArea->SetContent(cont);
}

unsigned int DialogChangelog::AddOnCloseClickFunction(const std::function<void()> & f)
{
    return mButtonClose->AddOnClickFunction(f);
}

void DialogChangelog::HandlePositionChanged()
{
    PositionElements();
}

void DialogChangelog::PositionElements()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();
    const int marginL = 20;
    const int marginT = 5;

    // BACKGROUND
    mBg->SetPosition(x0, y0);

    // TITLE
    const int titleX = x0 + marginL;
    const int titleY = y0 + marginT;
    mTitle->SetPosition(titleX, titleY);

    // CONTENT
    const int contX = marginL;
    const int contY = 65;
    mContArea->SetPosition(contX, contY);
}

} // namespace game
