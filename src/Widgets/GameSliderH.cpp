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

void GameSliderH::OnStateChanged(sgl::sgui::Slider::VisualState state)
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
    const int fullBarW = GetBarFullWidth();
    const int barW = fullBarW * GetValuePerc() / 100;
    mBar->SetWidth(barW);

    // update BUTTON
    const int barX1 = mBar->GetX() + barW;

    const int btnW = mButton->GetWidth();
    int btnX = barX1 - (btnW * 0.5f);

    const int limitX = mBg->GetX() + mBg->GetWidth();

    if(btnX + btnW > limitX)
        btnX = limitX - btnW;

    mButton->SetX(btnX);
}

void GameSliderH::UpdateGraphics(sgl::sgui::Slider::VisualState state)
{
    // BAR
    const int fullBarW = mTexBar->GetWidth();
    const int fullBarH = mTexBar->GetHeight();
    const int barW = fullBarW * GetValue() / 100;
    mBar->SetWidth(barW);

    SetBarFullSize(fullBarW, fullBarH);

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
