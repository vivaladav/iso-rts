#pragma once

namespace lib
{
namespace core
{

class EventDispatcher;
class KeyboardEvent;
class MouseButtonEvent;
class MouseMotionEvent;

class EventListener
{
public:
    virtual ~EventListener();

private:
    // -- application events --
    virtual void OnApplicationQuit();

    // -- keyboard events --
    virtual void OnKeyDown(const KeyboardEvent & event);
    virtual void OnKeyUp(const KeyboardEvent & event);

    // -- mouse event --
    virtual void OnMouseButtonDown(const MouseButtonEvent & event);
    virtual void OnMouseButtonUp(const MouseButtonEvent & event);
    virtual void OnMouseMotion(const MouseMotionEvent & event);

    // only EventDispatcher can generate events
    friend class EventDispatcher;
};

} // namespace core
} // namespace lib
