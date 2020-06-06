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

// -- mouse event --
void PushButton::HandleMouseButtonUp(const core::MouseButtonEvent & event)
{
    const int x = event.GetX();
    const int y = event.GetY();

    if(x > GetScreenX() && x < (GetScreenX() + mBg->GetWidth()) &&
       y > GetScreenY() && y < (GetScreenY() + mBg->GetHeight()))
        mOnClick();
}

void PushButton::OnRender()
{
    mBg->SetPosition(GetScreenX(), GetScreenY());
    mBg->Render();
}

} // namespace sgui
} // namespace lib
