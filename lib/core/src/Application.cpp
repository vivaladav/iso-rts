#include "core/Application.h"

#include "event/DummyListener.h"
#include "event/EventDispatcher.h"

#include <SDL2/SDL.h>

#include <cstdlib>
#include <iostream>

namespace lib
{
namespace core
{

Application::Application(int argc, char * argv[])
{
    ProcessArguments(argc, argv);

    atexit(SDL_Quit);

    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        std::cerr << "SDL init failed:" << SDL_GetError() << std::endl;
        exit(-1);
    }

    mDefaultListener = new DummyListener(this);
    mEventDispatcher = new EventDispatcher(mDefaultListener);
}

Application::~Application()
{
    delete mEventDispatcher;
    delete mDefaultListener;
}

void Application::ProcessArguments(int, char * [])
{
}

void Application::SetEventListener(EventListener * listener)
{
    if(listener)
        mEventDispatcher->SetListener(listener);
    else
        mEventDispatcher->SetListener(mDefaultListener);
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
