#include "sgui/PushButton.h"

#include "core/event/MouseButtonEvent.h"
#include "graphic/Image.h"
#include "graphic/Text.h"

#include <cassert>

namespace lib
{
namespace sgui
{

PushButton::PushButton(int wid, Widget * parent)
    : Widget(wid, parent)
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

    // update button size if bigger (this shouldn't happen)
    const int w = mBg->GetWidth();
    const int h = mBg->GetHeight();

    if(w > GetWidth() || h > GetHeight())
        SetSize(w, h);
}

void PushButton::OnPositionChanged()
{
    mBg->SetPosition(GetScreenX(), GetScreenY());
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
