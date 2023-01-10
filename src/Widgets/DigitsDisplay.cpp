#include "Widgets/DigitsDisplay.h"

#include <sgl/graphic/Camera.h>
#include <sgl/graphic/DummyRenderable.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Text.h>

#include <sstream>

namespace game
{

DigitsDisplay::DigitsDisplay(int digits, sgl::sgui::Widget * parent)
    : sgl::sgui::Widget(parent)
    , mDigits(digits)
{
    SetValue(0);
}

DigitsDisplay::DigitsDisplay(int digits, int fontSize, const std::string & suffix,
                             sgl::sgui::Widget * parent)
    : sgl::sgui::Widget(parent)
    , mDigits(digits)
    , mSuffix(suffix)
    , mFontSize(fontSize)
{
    SetValue(0);
}

void DigitsDisplay::SetColorZeros(unsigned int color)
{
    if(color == mColorZeros)
        return ;

    mColorZeros = color;

    if(mTxtZeros)
        mTxtZeros->SetColor(mColorZeros);
}

void DigitsDisplay::SetColorDigits(unsigned int color)
{
    if(color == mColorDigits)
        return ;

    mColorDigits = color;

    if(mTxtDigits)
        mTxtDigits->SetColor(mColorDigits);
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
    using namespace sgl::graphic;

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
    Font * font = fm->GetFont("Lato-Regular.ttf", mFontSize, Font::NORMAL);
    sgl::core::Sized size = font->GetTextSize(ss.str().c_str());

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

            mTxtZeros->SetColor(mColorZeros);

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
        mTxtDigits->SetColor(mColorDigits);

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
