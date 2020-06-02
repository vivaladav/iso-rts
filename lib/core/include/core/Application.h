#pragma once

#include <string>
#include <vector>

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

    int GetNumArguments() const;
    const std::vector<std::string> & GetArguments() const;
    virtual void ProcessArguments();

    void AddEventListener(EventListener * listener);
    void RemoveEventListener(EventListener * listener);

    void Run();

    void Exit();

private:
    virtual void Update() = 0;

private:
    int mArgc = 0;
    std::vector<std::string> mArgv;

    EventDispatcher * mEventDispatcher = nullptr;

    bool mRunning = false;

};

inline int Application::GetNumArguments() const { return mArgc; }
inline const std::vector<std::string> & Application::GetArguments() const { return mArgv; }

} // namespace core
} // namespace lib
