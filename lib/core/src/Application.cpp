#include "core/Application.h"

#include "event/EventDispatcher.h"

#include <SDL2/SDL.h>

#include <cmath>
#include <chrono>
#include <iostream>

namespace lib
{
namespace core
{

Application::Application(int argc, char * argv[])
    : mArgc(argc)
    , mEventDispatcher(new EventDispatcher)
{
    // -- STORE ARGUMENTS --
    for(int i = 0; i < argc; ++i)
        mArgv.emplace_back(argv[i]);

    // -- INIT SDL 2 --
    atexit(SDL_Quit);

    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
        exit(-1);
    }
}

Application::~Application()
{
    delete mEventDispatcher;
}

// ===== command line arguments =====
void Application::ProcessArguments()
{
}

// ===== events =====
void Application::AddApplicationListener(ApplicationEventListener * listener)
{
    mEventDispatcher->AddApplicationListener(listener);
}

void Application::AddKeyboardListener(KeyboardEventListener * listener)
{
    mEventDispatcher->AddKeyboardListener(listener);
}

void Application::AddMouseListener(MouseEventListener * listener)
{
    mEventDispatcher->AddMouseListener(listener);
}

void Application::RemoveApplicationListener(ApplicationEventListener * listener)
{
    mEventDispatcher->RemoveApplicationListener(listener);
}

void Application::RemoveKeyboardListener(KeyboardEventListener * listener)
{
    mEventDispatcher->RemoveKeyboardListener(listener);
}

void Application::RemoveMouseListener(MouseEventListener * listener)
{
    mEventDispatcher->RemoveMouseListener(listener);
}

// ===== execution =====
void Application::Run()
{
    mRunning = true;

    const float targetFrameTime = 1.f / 60.f;

    float delta = targetFrameTime;

    while(mRunning)
    {
        auto t0 = std::chrono::high_resolution_clock::now();

        mEventDispatcher->Update();

        Update(delta);

        auto t1 = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> diff = t1 - t0;

        const float frameTime = diff.count();
        const float delayTime = targetFrameTime - frameTime;

        SDL_Delay(static_cast<int>(roundf(delayTime * 1000.f)));

        delta = frameTime + delayTime;
    }
}

void Application::Exit()
{
    mRunning = false;
}

} // namespace core
} // namespace lib
