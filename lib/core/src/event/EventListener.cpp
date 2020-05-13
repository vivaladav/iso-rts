#include "core/event/EventListener.h"

namespace lib
{
namespace core
{

EventListener::~EventListener() { }

void EventListener::OnApplicationQuit() { }

void EventListener::OnKeyDown() { }
void EventListener::OnKeyUp() { }

void EventListener::OnMouseButtonDown() { }
void EventListener::OnMouseButtonUp() { }

} // namespace core
} // namespace lib
