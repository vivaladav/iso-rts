#pragma once

namespace lib
{
namespace core
{

class EventDispatcher;

class EventListener
{
public:
    virtual ~EventListener();

private:
    // -- application events --
    virtual void OnApplicationQuit();

    // -- keyboard events --
    virtual void OnKeyDown();
    virtual void OnKeyUp();

    // -- mouse event --
    virtual void OnMouseButtonDown();
    virtual void OnMouseButtonUp();

    // only EventDispatcher can generate events
    friend class EventDispatcher;
};

} // namespace core
} // namespace lib
