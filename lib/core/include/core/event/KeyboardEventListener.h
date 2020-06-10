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

private:
    virtual void OnKeyDown(const KeyboardEvent & event);
    virtual void OnKeyUp(const KeyboardEvent & event);

private:
    EventDispatcher * mDispatcher = nullptr;

    // only EventDispatcher can generate events
    friend class EventDispatcher;
};

} // namespace core
} // namespace lib
