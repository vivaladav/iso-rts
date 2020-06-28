#pragma once

#include <string>
#include <vector>

namespace lib
{
namespace core
{

class ApplicationEventListener;
class EventDispatcher;
class KeyboardEventListener;
class MouseEventListener;

class Application
{
public:
    Application(int argc, char * argv[]);
    virtual ~Application();

    // -- command line arguments --
    int GetNumArguments() const;
    const std::vector<std::string> & GetArguments() const;
    virtual void ProcessArguments();

    // -- events --
    void AddApplicationListener(ApplicationEventListener * el);
    void AddKeyboardListener(KeyboardEventListener * el);
    void AddMouseListener(MouseEventListener * el);

    void RemoveApplicationListener(ApplicationEventListener * el);
    void RemoveKeyboardListener(KeyboardEventListener * el);
    void RemoveMouseListener(MouseEventListener * el);

    // -- execution --
    void Run();
    void Exit();

private:
    virtual void Update(float delta) = 0;

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
