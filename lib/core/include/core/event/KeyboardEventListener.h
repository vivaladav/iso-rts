#pragma once

namespace lib
{
namespace core
{

class EventDispatcher;
class KeyboardEvent;

class KeyboardEventListener
{
public:
    virtual ~KeyboardEventListener();

protected:
    virtual void OnKeyDown(const KeyboardEvent & event);
    virtual void OnKeyUp(const KeyboardEvent & event);

    // EventDispatcher can generate events
    friend class EventDispatcher;
};

} // namespace core
} // namespace lib
