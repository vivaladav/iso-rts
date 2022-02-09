#include "ResourceDisplay.h"

#include "Widgets/DigitsDisplay.h"
#include "Widgets/ProgressBarResources.h"

#include <sgl/graphic/Texture.h>
#include <sgl/sgui/Image.h>

#include <cmath>

namespace game
{

ResourceDisplay::ResourceDisplay(sgl::graphic::Texture * icon, int digits, sgl::sgui::Widget * parent)
    : sgl::sgui::Widget(parent)
{
    // ICON
    mIcon = new sgl::sgui::Image(icon, this);
    const int iconW = mIcon->GetWidth();
    const int iconH = mIcon->GetHeight();

    const int marginH = 10;
    const int marginV = 6;
    const int x1 = iconW + marginH;

    // DIGITS
    mDigits = new DigitsDisplay(digits, this);
    const int digitsW = mDigits->GetWidth();
    const int digitsH = mDigits->GetHeight();

    // BAR
    const float min = 0.f;
    const float max = 1000.;
    mBar = new ProgressBarResources(min, max, this);
    const int barW = mBar->GetWidth();
    const int barH = mBar->GetHeight();

    // POSITION WIDGETS
    const int rightBlockH = digitsH + barH + marginV;

    int blockY = 0;

    if(rightBlockH > iconH)
    {
        const int iconY = std::roundf((rightBlockH - iconH) * 0.5f);
        mIcon->SetY(iconY);
    }
    // H of right block is < than H of icon
    else
        blockY = std::roundf((iconH - rightBlockH) * 0.5f);

    int digitsX = x1;
    int barX = x1;

    if(digitsW > barW)
        barX += std::roundf((digitsW - barW) * 0.5f);
    else
        digitsX += std::roundf((barW - digitsW) * 0.5f);

    mDigits->SetPosition(digitsX, blockY);

    const int barY = blockY + digitsH + marginV;
    mBar->SetPosition(barX, barY);
}

void ResourceDisplay::SetValue(int val)
{
    mDigits->SetValue(val);
    mBar->SetValue(val);
}

void ResourceDisplay::SetValueMinMax(int min, int max)
{
    mBar->SetMinMax(min, max);
}

} // namespace game
