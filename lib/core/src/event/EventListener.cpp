#include "core/event/EventListener.h"

namespace lib
{
namespace core
{

EventListener::~EventListener() { }

void EventListener::OnApplicationQuit() { }

void EventListener::OnKeyDown(const KeyboardEvent &) { }
void EventListener::OnKeyUp(const KeyboardEvent &) { }

void EventListener::OnMouseButtonDown(const MouseButtonEvent &) { }
void EventListener::OnMouseButtonUp(const MouseButtonEvent &) { }
void EventListener::OnMouseMotion(const MouseMotionEvent &) { }

} // namespace core
} // namespace lib
