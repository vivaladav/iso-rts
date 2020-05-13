#include "EventDispatcher.h"

#include "core/event/EventListener.h"

#include <SDL2/SDL.h>

#include <cassert>

namespace lib
{
namespace core
{

EventDispatcher::EventDispatcher(EventListener * listener) : mActiveListener(listener)
{
    assert(listener);
}

void EventDispatcher::SetListener(EventListener * listener)
{
    assert(listener);

    mActiveListener = listener;
}

void EventDispatcher::Update()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            mActiveListener->OnApplicationQuit();
        else if(event.type == SDL_KEYUP)
        {
//            if(SDLK_ESCAPE == event.key.keysym.sym)
//                mRunning = false;
        }
    }
}


} // namespace core
} // namespace lib

