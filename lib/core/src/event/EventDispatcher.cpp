#include "EventDispatcher.h"

#include "core/event/EventListener.h"
#include "core/event/KeyboardEvent.h"

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
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
            break;

            case SDL_MOUSEBUTTONDOWN:
            break;

            case SDL_MOUSEBUTTONUP:
            break;

            case SDL_KEYDOWN:
            {
                KeyboardEvent e(event.key.keysym.sym);
                mActiveListener->OnKeyDown(e);
            }
            break;

            case SDL_KEYUP:
            {
                KeyboardEvent e(event.key.keysym.sym);
                mActiveListener->OnKeyUp(e);
            }
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

