#pragma once

namespace lib
{
namespace core
{

class EventDispatcher;
class MouseButtonEvent;
class MouseMotionEvent;

class MouseEventListener
{
public:
    virtual ~MouseEventListener();

private:
    virtual void OnMouseButtonDown(const MouseButtonEvent & event);
    virtual void OnMouseButtonUp(const MouseButtonEvent & event);
    virtual void OnMouseMotion(const MouseMotionEvent & event);

private:
    EventDispatcher * mDispatcher = nullptr;

    // only EventDispatcher can generate events
    friend class EventDispatcher;
};

} // namespace core
} // namespace lib
