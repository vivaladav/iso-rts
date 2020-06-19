#include "sgui/PushButton.h"

#include "core/event/MouseButtonEvent.h"
#include "graphic/Image.h"
#include "graphic/Text.h"
#include "sgui/Stage.h"

#include <cassert>

namespace lib
{
namespace sgui
{

PushButton::PushButton(int wid, Widget * parent)
    : Widget(wid, parent)
    , mOnClick([]{})
{
}

PushButton::~PushButton()
{
    delete mBg;
}

void PushButton::SetBackground(const char * file)
{
    assert(file);

    mBg = new graphic::Image(file);
    mBg->SetPosition(GetScreenX(), GetScreenY());

    // update button size if bigger (this shouldn't happen)
    const int w = mBg->GetWidth();
    const int h = mBg->GetHeight();

    if(w > GetWidth() || h > GetHeight())
        SetSize(w, h);
}

void PushButton::SetLabel(const char * text)
{
    graphic::Font * font = Stage::Instance()->GetDefaultFont();

    mLabel = new graphic::Text(text, font);

    PositionLabel();
}

void PushButton::SetLabelColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    if(mLabel)
        mLabel->SetColor(r, g, b, a);
}

void PushButton::OnPositionChanged()
{
    if(mBg)
        mBg->SetPosition(GetScreenX(), GetScreenY());

    if(mLabel)
        PositionLabel();
}

void PushButton::PositionLabel()
{
    const int x = GetScreenX() + (GetWidth() - mLabel->GetWidth()) * 0.5f;
    const int y = GetScreenY() + (GetHeight() - mLabel->GetHeight()) * 0.5f;

    mLabel->SetPosition(x, y);
}
// -- mouse event --
void PushButton::HandleMouseButtonUp(const core::MouseButtonEvent &)
{
    mOnClick();
}

void PushButton::OnRender()
{
    mBg->Render();

    if(mLabel)
        mLabel->Render();
}

} // namespace sgui
} // namespace lib
