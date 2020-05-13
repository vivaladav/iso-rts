#pragma once

namespace lib
{
namespace core
{

class EventDispatcher;
class EventListener;

class Application
{
public:
    Application(int argc, char * argv[]);
    virtual ~Application();

    void SetEventListener(EventListener * listener);

    void Run();

    void Exit();

private:
    virtual void Update() = 0;

private:
    EventListener * mDefaultListener = nullptr;
    EventDispatcher * mEventDispatcher = nullptr;

    bool mRunning = false;

};

} // namespace core
} // namespace lib
