#include "sgui/Label.h"

#include "graphic/DummyRenderable.h"
#include "graphic/Font.h"
#include "graphic/Text.h"

#include <cassert>

namespace lib
{
namespace sgui
{

Label::Label(int wid, graphic::Font * font, Widget * parent)
    : Widget(wid, parent)
    , mFont(font)
{
    assert(font);

    CreateText();
}

Label::Label(const char * txt, int wid, graphic::Font * font, Widget * parent)
    : Widget(wid, parent)
    , mStr(txt)
    , mFont(font)
{
    assert(txt);
    assert(font);

    CreateText();
}

Label::~Label()
{
    delete mTxt;
}

void Label::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    mTxt->SetColor(r, g, b, a);
}

void Label::SetText(const char * txt)
{
    assert(txt);

    std::string t(txt);

    if(t == mStr)
        return ;

    delete mTxt;

    mStr = t;

    CreateText();
}

void Label::CreateText()
{
    if(mStr.empty())
        mTxt = new graphic::DummyRenderable;
    else
        mTxt = new graphic::Text(mStr.c_str(), mFont);  // TODO check if Text is valid after creation and fallback on dummy

    mTxt->SetPosition(GetScreenX(), GetScreenY());

    SetSize(mTxt->GetWidth(), mTxt->GetHeight());
}

void Label::OnPositionChanged()
{
    mTxt->SetPosition(GetScreenX(), GetScreenY());
}

void Label::OnRender()
{
    mTxt->Render();
}

} // namespace sgui
} // namespace lib
