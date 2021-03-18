#include "ResourceDisplay.h"

#include "Widgets/DigitsDisplay.h"
#include "Widgets/ProgressBarResources.h"

#include <graphic/Image.h>
#include <graphic/Texture.h>
#include <graphic/TextureManager.h>
#include <sgui/Image.h>

#include <cmath>

namespace game
{

ResourceDisplay::ResourceDisplay(const char * icon, lib::sgui::Widget * parent)
    : lib::sgui::Widget(parent)
{
    // ICON
    mIcon = new lib::sgui::Image(icon, this);
    const int iconW = mIcon->GetWidth();
    const int iconH = mIcon->GetHeight();

    const int marginH = 10;
    const int marginV = 0;
    const int x1 = iconW + marginH;

    // DIGITS
    mDigits = new DigitsDisplay(5, this);
    mDigits->SetValue(125);
    const int digitsW = mDigits->GetWidth();
    const int digitsH = mDigits->GetHeight();

    // BAR
    mBar = new ProgressBarResources(0.f, 1000.f, this);
    mBar->SetValue(300.f);
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

} // namespace game
