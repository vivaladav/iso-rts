#include "EventDispatcher.h"

#include "core/event/EventListener.h"

#include <SDL2/SDL.h>

#include <cassert>

#include<iostream>

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
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
            break;

            case SDL_MOUSEBUTTONDOWN:
                std::cout << "SDL_MOUSEBUTTONDOWN" << std::endl;
            break;

            case SDL_MOUSEBUTTONUP:
                std::cout << "SDL_MOUSEBUTTONUP" << std::endl;
            break;

            case SDL_KEYDOWN:
            break;

            case SDL_KEYUP:
            break;

            case SDL_QUIT:
                mActiveListener->OnApplicationQuit();
            break;

            default:
            break;
        }
    }
}

} // namespace core
} // namespace lib

