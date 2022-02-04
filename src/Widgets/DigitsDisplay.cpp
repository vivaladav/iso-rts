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

DigitsDisplay::DigitsDisplay(int digits, int fontSize, const std::string & suffix,
                             lib::sgui::Widget * parent)
    : lib::sgui::Widget(parent)
    , mDigits(digits)
    , mSuffix(suffix)
    , mFontSize(fontSize)
{
    SetValue(0);
}

void DigitsDisplay::SetFontSize(int val)
{
    // same size already set
    if(mFontSize == val)
        return;

    mFontSize = val;

    UpdateDigits(true);
}

void DigitsDisplay::SetSuffix(const std::string & suffix)
{
    // same suffix already set
    if(mSuffix == suffix)
        return;

    mSuffix = suffix;

    UpdateDigits(true);
}

void DigitsDisplay::SetValue(int val)
{    
    // same value as current -> do nothing
    if(val == mValue)
        return ;

    mValue = val;

    UpdateDigits();
}

void DigitsDisplay::UpdateDigits(bool force)
{
    using namespace lib::graphic;

    int val = mValue;

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

    ss << val << mSuffix;

    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/Lato-Regular.ttf", mFontSize, Font::NORMAL);
    lib::core::Sized size = font->GetTextSize(ss.str().c_str());

    // create zeroes
    const int digitsZero = mDigits - digitsVal;

    if(mNumZeros != digitsZero || force)
    {
        mNumZeros = digitsZero;

        if(mTxtZeros)
            UnregisterRenderable(mTxtZeros);

        delete mTxtZeros;

        if(digitsZero > 0)
        {
            // all digits are 0 (val is 0)
            if(digitsZero == mDigits)
            {
                const std::string str = std::string(digitsZero, '0') + mSuffix;
                mTxtZeros = new Text(str.c_str(), font, true);
            }
            else
            {
                const std::string str(digitsZero, '0');
                mTxtZeros = new Text(str.c_str(), font, true);
            }

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
        const std::string str = std::to_string(mValue) + mSuffix;

        mTxtDigits = new Text(str.c_str(), font, true);
        mTxtDigits->SetColor(0xE3E6e8FF);

        SetSize(size.w, mTxtDigits->GetHeight());
    }
    else
        mTxtDigits = new DummyRenderable;

    RegisterRenderable(mTxtDigits);

    SetPositions();
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
