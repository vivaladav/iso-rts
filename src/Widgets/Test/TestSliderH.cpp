#include "Widgets/Test/TestSliderH.h"

#include "GameTestData.h"

#include <sgl/graphic/Image.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/TextureManager.h>

namespace game
{

TestSliderH::TestSliderH(sgl::sgui::Widget * parent)
    : sgl::sgui::Slider(sgl::sgui::Slider::HORIZONTAL, parent)
{
    UpdateGraphics(GetState());

    UpdatePositions();
}

void TestSliderH::OnStateChanged(sgl::sgui::Slider::VisualState state)
{
    UpdateGraphics(state);
}

void TestSliderH::HandlePositionChanged()
{
    UpdatePositions();
}

void TestSliderH::UpdateGraphics(sgl::sgui::Slider::VisualState state)
{
    auto tm = sgl::graphic::TextureManager::Instance();

    // BACKGROUND
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileTestUI, IND_TSLIH_BG);
    mBg->SetTexture(tex);

    // BAR
    tex = tm->GetSprite(SpriteFileTestUI, IND_TSLIH_BAR);
    mBar->SetTexture(tex);

    // BUTTON
    tex = tm->GetSprite(SpriteFileTestUI, IND_TSLIH_BUTTON);
    mButton->SetTexture(tex);

    // update widget size
    SetSize(mBg->GetWidth(), mButton->GetHeight());
}

void TestSliderH::UpdatePositions()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    const int border = 2;

    // BACKGROUND
    const int bgX = x0;
    const int bgY = y0 + (mButton->GetHeight() - mBg->GetHeight()) * 0.5f;
    mBg->SetPosition(bgX, bgY);

    // BAR
    const int barX = bgX + border;
    const int barY = bgY + border;
    mBar->SetPosition(barX, barY);

    // BUTTON
    const int btnX = x0;
    const int btnY = y0;
    mButton->SetPosition(btnX, btnY);
}

} // namespace game
