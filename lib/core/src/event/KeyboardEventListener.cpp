#include "core/event/KeyboardEventListener.h"

namespace lib
{
namespace core
{

KeyboardEventListener::~KeyboardEventListener() { }

void KeyboardEventListener::OnKeyDown(const KeyboardEvent &) { }
void KeyboardEventListener::OnKeyUp(const KeyboardEvent &) { }

} // namespace core
} // namespace lib
