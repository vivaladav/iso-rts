#include "core/event/MouseEventListener.h"

namespace lib
{
namespace core
{

MouseEventListener::~MouseEventListener() { }

void MouseEventListener::OnMouseButtonDown(const MouseButtonEvent &) { }
void MouseEventListener::OnMouseButtonUp(const MouseButtonEvent &) { }
void MouseEventListener::OnMouseMotion(const MouseMotionEvent &) { }

} // namespace core
} // namespace lib
