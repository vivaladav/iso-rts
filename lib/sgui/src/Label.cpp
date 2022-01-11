#include "sgui/Label.h"

#include "graphic/DummyRenderable.h"
#include "graphic/Font.h"
#include "graphic/Text.h"

#include <cassert>

namespace lib
{
namespace sgui
{

Label::Label(graphic::Font * font, Widget * parent)
    : Widget(parent)
    , mFont(font)
{
    assert(font);

    CreateText();
}

Label::Label(const char * txt, graphic::Font * font, Widget * parent)
    : Widget(parent)
    , mStr(txt)
    , mFont(font)
{
    assert(txt);
    assert(font);

    CreateText();
}

void Label::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    a = MixAlphaAndAlpha(a);

    mTxt->SetColor(r, g, b, a);
}

void Label::SetColor(unsigned int color)
{
    color = MixColorAndAlpha(color);

    mTxt->SetColor(color);
}

void Label::SetText(const char * txt)
{
    assert(txt);

    std::string t(txt);

    if(t == mStr)
        return ;

    const unsigned int color = mTxt->GetColor();

    UnregisterRenderable(mTxt);

    delete mTxt;

    mStr = t;

    CreateText();

    mTxt->SetColor(color);
}

void Label::CreateText()
{
    if(mStr.empty())
        mTxt = new graphic::DummyRenderable;
    else
        mTxt = new graphic::Text(mStr.c_str(), mFont);  // TODO check if Text is valid after creation and fallback on dummy

    RegisterRenderable(mTxt);

    mTxt->SetPosition(GetScreenX(), GetScreenY());

    SetSize(mTxt->GetWidth(), mTxt->GetHeight());
}

void Label::HandlePositionChanged()
{
    mTxt->SetPosition(GetScreenX(), GetScreenY());
}

} // namespace sgui
} // namespace lib
