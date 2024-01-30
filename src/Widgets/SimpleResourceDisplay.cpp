#include "SimpleResourceDisplay.h"

#include "Widgets/DigitsDisplay.h"

#include <sgl/graphic/Texture.h>
#include <sgl/sgui/Image.h>

#include <cmath>

namespace game
{

SimpleResourceDisplay::SimpleResourceDisplay(sgl::graphic::Texture * icon, int digits, sgl::sgui::Widget * parent)
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

    // POSITION WIDGETS
    const int digitsH = mDigits->GetHeight();

    int blockY = 0;

    if(digitsH > iconH)
    {
        const int iconY = std::roundf((digitsH - iconH) * 0.5f);
        mIcon->SetY(iconY);
    }
    // H of right block is < than H of icon
    else
        blockY = std::roundf((iconH - digitsH) * 0.5f);

    mDigits->SetPosition(x1, blockY);
}

void SimpleResourceDisplay::SetValue(int val)
{
    mDigits->SetValue(val);
}

} // namespace game
