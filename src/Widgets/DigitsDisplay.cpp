#include "Widgets/DigitsDisplay.h"

#include <graphic/Camera.h>
#include <graphic/DummyRenderable.h>
#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Text.h>

#include <sstream>

namespace game
{

DigitsDisplay::DigitsDisplay(int digits, lib::sgui::Widget * parent)
    : lib::sgui::Widget(parent)
    , mDigits(digits)
{
    SetValue(0);
}

void DigitsDisplay::SetValue(int val)
{
    using namespace lib::graphic;

    // same value as current -> do nothing
    if(val == mValue)
        return ;

    mValue = val;

    // extract number of digits of val
    int digitsVal = 0;

    while(val > 0)
    {
        ++digitsVal;
        val /= 10;
    }

    // TODO handle overflow
    if(digitsVal > mDigits)
        return ;

    // get size for whole string
    std::ostringstream ss;
    ss.width(mDigits);
    ss.fill('0');

    ss << val;

    Font * font = FontManager::Instance()->GetFont("data/fonts/Lato-Regular.ttf", 19, Font::NORMAL);
    lib::core::Sized size = font->GetTextSize(ss.str().c_str());

    // create zeroes
    const int digitsZero = mDigits - digitsVal;

    if(mNumZeros != digitsZero)
    {
        mNumZeros = digitsZero;

        if(mTxtZeros)
            UnregisterRenderable(mTxtZeros);

        delete mTxtZeros;

        if(digitsZero)
        {
            const std::string str(digitsZero, '0');

            mTxtZeros = new Text(str.c_str(), font, true);
            mTxtZeros->SetColor(0x454f54FF);

            SetSize(size.w, mTxtZeros->GetHeight());
        }
        else
            mTxtZeros = new DummyRenderable;

        RegisterRenderable(mTxtZeros);
    }

    // create value digits
    if(mTxtDigits)
        UnregisterRenderable(mTxtDigits);

    delete mTxtDigits;

    if(digitsVal)
    {
        const std::string str(std::to_string(mValue));

        mTxtDigits = new Text(str.c_str(), font, true);
        mTxtDigits->SetColor(0xE3E6e8FF);

        SetSize(size.w, mTxtDigits->GetHeight());
    }
    else
        mTxtDigits = new DummyRenderable;

    RegisterRenderable(mTxtDigits);

    SetPositions();
}

void DigitsDisplay::OnRender()
{
    mTxtZeros->Render();
    mTxtDigits->Render();
}

void DigitsDisplay::HandlePositionChanged()
{
    SetPositions();
}

void DigitsDisplay::SetPositions()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    mTxtZeros->SetPosition(x0, y0);

    mTxtDigits->SetPosition(x0 + GetWidth() - mTxtDigits->GetWidth(), y0);
}

} // namespace game
