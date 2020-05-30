#include "core/Application.h"

#include "event/EventDispatcher.h"

#include <SDL2/SDL.h>

#include <cstdlib>
#include <iostream>

namespace lib
{
namespace core
{

Application::Application(int argc, char * argv[])
    : mEventDispatcher(new EventDispatcher)
{
    ProcessArguments(argc, argv);

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

void Application::ProcessArguments(int, char * [])
{
}

void Application::AddEventListener(EventListener * listener)
{
    mEventDispatcher->AddListener(listener);
}

void Application::RemoveEventListener(EventListener * listener)
{
    mEventDispatcher->RemoveListener(listener);
}

void Application::Run()
{
    mRunning = true;

    while(mRunning)
    {
        mEventDispatcher->Update();

        Update();

        SDL_Delay(16);
    }
}

void Application::Exit()
{
    mRunning = false;
}

} // namespace core
} // namespace lib
