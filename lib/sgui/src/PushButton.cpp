#include "sgui/PushButton.h"

#include "core/event/MouseButtonEvent.h"
#include "graphic/Image.h"

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

    SetSize(mBg->GetWidth(), mBg->GetHeight());
}

void PushButton::Render()
{
    mBg->SetPosition(GetX(), GetY());
    mBg->Render();
}

// -- mouse event --
void PushButton::OnMouseButtonUp(const core::MouseButtonEvent & event)
{
    const int x = event.GetX();
    const int y = event.GetY();

    if(x > GetX() && x < (GetX() + mBg->GetWidth()) &&
       y > GetY() && y < (GetY() + mBg->GetHeight()))
        mOnClick();
}

} // namespace sgui
} // namespace lib
