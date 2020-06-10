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

private:
    virtual void OnApplicationQuit();

private:
    EventDispatcher * mDispatcher = nullptr;

    // only EventDispatcher can generate events
    friend class EventDispatcher;
};

} // namespace core
} // namespace lib
