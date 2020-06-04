#pragma once

namespace lib
{
namespace core
{

class EventDispatcher;

class ApplicationEventListener
{
public:
    virtual ~ApplicationEventListener();

protected:
    virtual void OnApplicationQuit();

    // EventDispatcher can generate events
    friend class EventDispatcher;
};

} // namespace core
} // namespace lib
