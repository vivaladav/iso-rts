#include "Widgets/ButtonPanelTab.h"

#include "Widgets/GameUIData.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/graphic/Text.h>
#include <sgl/media/AudioManager.h>
#include <sgl/media/AudioPlayer.h>
#include <sgl/sgui/Image.h>

namespace game
{

ButtonPanelTab::ButtonPanelTab(const char * text, sgl::sgui::Widget * parent)
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

void ButtonPanelTab::HandleMouseOver()
{
    sgl::sgui::AbstractButton::HandleMouseOver();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_over-03.ogg");
}

void ButtonPanelTab::HandleButtonDown()
{
    sgl::sgui::AbstractButton::HandleButtonDown();

    auto player = sgl::media::AudioManager::Instance()->GetPlayer();
    player->PlaySound("UI/button_click-03.ogg");
}

void ButtonPanelTab::OnStateChanged(sgl::sgui::AbstractButton::VisualState state)
{
    UpdateGraphics(state);
}

void ButtonPanelTab::HandlePositionChanged()
{
    UpdatePositions();
}

void ButtonPanelTab::UpdateGraphics(sgl::sgui::AbstractButton::VisualState state)
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

void ButtonPanelTab::UpdatePositions()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    // LABEL
    mLabel->SetPosition(x0 + (GetWidth() - mLabel->GetWidth()) * 0.5f,
                        y0 + (GetHeight() - mLabel->GetHeight()) * 0.5f);
}

} // namespace game
