#include "core/event/EventListener.h"

namespace lib
{
namespace core
{

EventListener::~EventListener() { }

void EventListener::OnApplicationQuit() { }

void EventListener::OnKeyDown(const KeyboardEvent &) { }
void EventListener::OnKeyUp(const KeyboardEvent &) { }

void EventListener::OnMouseButtonDown() { }
void EventListener::OnMouseButtonUp() { }

} // namespace core
} // namespace lib
