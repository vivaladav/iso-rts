#include "core/event/KeyboardEventListener.h"

#include "EventDispatcher.h"

namespace lib
{
namespace core
{

KeyboardEventListener::~KeyboardEventListener()
{
    if(mDispatcher)
        mDispatcher->RemoveKeyboardListener(this);
}

void KeyboardEventListener::OnKeyDown(KeyboardEvent &) { }
void KeyboardEventListener::OnKeyUp(KeyboardEvent &) { }

} // namespace core
} // namespace lib
