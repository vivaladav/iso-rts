#pragma once

namespace lib
{
namespace core
{

class Application
{
public:
    Application(int argc, char * argv[]);
    virtual ~Application();

    void Run();

    void Exit();

private:
    virtual void Update() = 0;

private:
    bool mRunning = false;

};

} // namespace core
} // namespace lib
