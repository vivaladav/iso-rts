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
    , mFont(font)
{
    assert(txt);
    assert(font);

    mTxt = new graphic::Text(txt, font);
    mTxt->SetPosition(GetScreenX(), GetScreenY());

    SetSize(mTxt->GetWidth(), mTxt->GetHeight());
}

void Label::OnPositionChanged()
{
    mTxt->SetPosition(GetScreenX(), GetScreenY());
}

void Label::OnRender() { mTxt->Render(); }

} // namespace sgui
} // namespace lib
