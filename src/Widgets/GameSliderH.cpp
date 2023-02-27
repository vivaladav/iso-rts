#include "Widgets/GameSliderH.h"

#include <sgl/graphic/Image.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>

namespace game
{

GameSliderH::GameSliderH(sgl::graphic::Texture * texBg, sgl::graphic::Texture * texBar,
                         sgl::graphic::Texture * texButton, sgl::sgui::Widget * parent)
    : sgl::sgui::Slider(sgl::sgui::Slider::HORIZONTAL, parent)
    , mTexBar(texBar)
{
    mBg->SetTexture(texBg);
    mBar->SetTexture(texBar);
    mButton->SetTexture(texButton);

    UpdateGraphics(GetState());

    UpdatePositions();
}

void GameSliderH::OnStateChanged(VisualState state)
{
    UpdateGraphics(state);
}

void GameSliderH::HandlePositionChanged()
{
    UpdatePositions();
}

void GameSliderH::HandleValueChanged(int val)
{
    // update BAR
    const int fullBarW = GetSlidingAreaWidth();
    const int barW = fullBarW * GetValuePerc() / 100;
    mBar->SetWidth(barW);

    UpdatePositions();
}

void GameSliderH::UpdateGraphics(VisualState state)
{
    // BAR
    const int fullBarW = mTexBar->GetWidth();
    const int fullBarH = mTexBar->GetHeight();
    const int barW = fullBarW * GetValue() / 100;
    mBar->SetWidth(barW);

    SetSlidingAreaSize(fullBarW, fullBarH);

    // update widget size
    SetSize(mBg->GetWidth(), mButton->GetHeight());
}

void GameSliderH::UpdatePositions()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    const int border = 2;

    // BACKGROUND
    const int bgX = x0;
    const int bgY = y0 + (mButton->GetHeight() - mBg->GetHeight()) / 2;
    mBg->SetPosition(bgX, bgY);

    // BAR
    const int barX = bgX + border;
    const int barY = bgY + border;
    mBar->SetPosition(barX, barY);
    SetSlidingAreaPosition(barX, barY);

    // BUTTON
    const int btnX = barX + mBar->GetWidth() - (mButton->GetWidth() / 2);
    const int btnY = y0;
    mButton->SetPosition(btnX, btnY);
}

} // namespace game
