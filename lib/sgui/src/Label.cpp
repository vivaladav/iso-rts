#include "sgui/Label.h"

#include "graphic/Font.h"
#include "graphic/Text.h"

#include <cassert>

namespace lib
{
namespace sgui
{

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
        return ;

    mTxt = new graphic::Text(mStr.c_str(), mFont);
    mTxt->SetPosition(GetScreenX(), GetScreenY());

    SetSize(mTxt->GetWidth(), mTxt->GetHeight());
}

void Label::OnPositionChanged()
{
    if(mTxt)
        mTxt->SetPosition(GetScreenX(), GetScreenY());
}

void Label::OnRender()
{
    if(mTxt)
        mTxt->Render();
}

} // namespace sgui
} // namespace lib
