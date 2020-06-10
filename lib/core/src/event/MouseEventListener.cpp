#include "core/event/MouseEventListener.h"

#include "EventDispatcher.h"

namespace lib
{
namespace core
{

MouseEventListener::~MouseEventListener()
{
    if(mDispatcher)
        mDispatcher->RemoveMouseListener(this);
}

void MouseEventListener::OnMouseButtonDown(const MouseButtonEvent &) { }
void MouseEventListener::OnMouseButtonUp(const MouseButtonEvent &) { }
void MouseEventListener::OnMouseMotion(const MouseMotionEvent &) { }

} // namespace core
} // namespace lib
